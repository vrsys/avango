/* 
 * Contents: Implementation of HOT messages
 *
 * Author:  Alexey Vaysburd, November 1996
 */

#include <string.h>
#include <stdlib.h>
#include "hot_error.h"
#include "hot_msg.h"
#include "hot_thread.h"
#include "hot_sys.h"
#include "hot_mem.h"
 
struct hot_msg_ctl;
struct hot_msg_body;

#define HOT_MSG_HANDLE_MAGIC 0x9703bd64
#define HOT_MSG_CTL_MAGIC    0xba478cef

/* Message handle (obscured to hot_msg_t).
 */
struct hot_msg_handle {
    struct hot_msg_ctl *ctl;	/* Points to the control struct */
    unsigned offset;
    hot_msg_readDirection_t read_dir; /* Direction of reading */
    int index;
    unsigned int magic;  
};

/* We support up to 32 aliases to a message (in practice, 1-2 suffice).
 */
#define HOT_MSG_MAX_ALIASES (sizeof(unsigned) * 8)

#define HOT_BIT(n) (1 << (n))

/* Message control block.
 */
struct hot_msg_ctl {
    hot_lock_t mutex; 
    struct hot_msg_body *next;	/* next cached msg in the free list */
    struct hot_msg_handle handles[HOT_MSG_MAX_ALIASES];
    int nused;			/* # used handles (this is really the refcnt)*/
    unsigned used_flags;	/* n'th bit is set iff n'th handle is used */
    int writable;
    unsigned size;		/* current size of the message */
    void (*on_release)(void*);	/* to be invoked upon msg release */
    void *env;			/* argument for on_release */
    int need_release;		/* set if on_release needs to be called */
    char *data;			/* points to the beginning of data buffer */
    unsigned alloc_size;	/* size of the allocated data buffer */
    unsigned int magic;		/* MUST BE THE LAST FIELD, FOR DATA ALIGNMENT */
};

/* Size of a message buffer.  Max supported message size is
 * HOT_MSG_BUF minus size of the control block.
 */ 
#define HOT_MSG_BUF 4096
#define HOT_MSG_MAX_PREALLOC_SIZE (HOT_MSG_BUF - sizeof(struct hot_msg_ctl))

/* Prealloc 250 messages at the initialization time.
 */
#define HOT_MSG_PREALLOC 4096

/* Run-time "typechecking"
 */
#define HOT_MSG_PACKED_MAGIC 0xabdfbabc

/* Message body.  The size is exactly HOT_MSG_BUF.
 */
struct hot_msg_body {
  struct hot_msg_ctl ctl;	/* MUST BE THE FIRST FIELD */
  char data[HOT_MSG_MAX_PREALLOC_SIZE];	/* space for message data */
};

/* Global variables.
 */
static struct {
  int initialized;
  struct hot_msg_body *free_list;
  unsigned nfree;			/* # free messages */
  hot_lock_t mutex; 
  hot_mem_t memory;
} global;

/* Return the first power of two larger than the given number.
 */
/* ijh: HP cc does not have inline */
#ifdef INLINE_PRAGMA
#pragma inline hot_power2
#endif
MSG_INLINE
static
int hot_power2(unsigned n) {
    unsigned p2 = 1;
    while (p2 <= n) p2 <<= 1;
    return p2;
}

/* Release the data buffer of a big message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_ReleaseBigMsg
#endif
MSG_INLINE
static void hot_msg_ReleaseBigMsg(void *arg) {
    struct hot_msg_ctl *ctl = (struct hot_msg_ctl*) arg;
    if (ctl == NULL || ctl->data == NULL)
	hot_sys_Panic("hot_msg_ReleaseBigMsg:  internal inconsistency");
    hot_mem_Free(ctl->data);
}

/* Fit a message buffer to the specified size.  Allocate more memory if needed.
 * The message is locked at this point.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_FitToSize
#endif
MSG_INLINE
static void hot_msg_FitToSize(struct hot_msg_ctl *ctl, unsigned new_size) {
    if (new_size <= ctl->alloc_size) 
	return;

    ctl->alloc_size = hot_power2(new_size);
    
    /* The preallocated buffer is being exceeded for the first time.
     * Allocate a new buffer (the size is the first power of 2 bigger
     * than the new message size) and copy the current msg contents in it.
     */
    if (!ctl->need_release) {
	char *old_data = ctl->data;      
	ctl->data = hot_mem_Alloc(global.memory, ctl->alloc_size);
	if (ctl->data == NULL)
	    hot_sys_Panic("hot_msg_Write: malloc returned NULL");
	
	/* The beginning of a message buffer must be 4-byte aligned.
	 */
	assert(((unsigned)ctl->data) % 4 == 0);
	
	ctl->need_release = 1;
	ctl->on_release = hot_msg_ReleaseBigMsg;
	ctl->env = (void*) ctl;
	
	/* Copy the current message contents to the new location.
	 */
	memcpy(ctl->data, old_data, ctl->size);
    }
    
    else {
	ctl->data = hot_mem_Realloc(ctl->data, ctl->alloc_size);
	if (ctl->data == NULL)
	    hot_sys_Panic("hot_msg_Write: realloc returned NULL");

	/* The beginning of a message buffer must be 4-byte aligned.
	 */
	assert(((unsigned)ctl->data) % 4 == 0);
    }
}

/* Initialize the message module.  We check for initialization at
 * message creation time.
 */
static void hot_msg_Init(void) {
  int i;
  hot_err_t err;

  global.initialized = 1;
  if ((err = hot_lck_Create(&global.mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  if ((err = hot_lck_Lock(global.mutex)) != HOT_OK)
     hot_sys_Panic(hot_err_ErrString(err));

  global.memory = hot_mem_AddChannel("hot_msg");

  /* Preallocate some messages and put them on free list.
   */
  global.free_list = (struct hot_msg_body*) hot_mem_Alloc(
      global.memory, HOT_MSG_PREALLOC * sizeof(struct hot_msg_body));
  if (global.free_list == NULL)
    hot_sys_Panic("hot_msg_init:malloc");

  for (i = 0; i < HOT_MSG_PREALLOC; i++) {
      unsigned int k;
      global.free_list[i].ctl.magic = 0;
      for (k = 0; k < HOT_MSG_MAX_ALIASES; k++) {
	  global.free_list[i].ctl.handles[k].magic = 0;
      }
      global.free_list[i].ctl.next = (i+1 < HOT_MSG_PREALLOC) ?
	  &global.free_list[i+1] : NULL;
      if ((err = hot_lck_Create(&global.free_list[i].ctl.mutex)) != HOT_OK)
	  hot_sys_Panic(hot_err_ErrString(err));
  }

  global.nfree = HOT_MSG_PREALLOC;

  if ((err = hot_lck_Unlock(global.mutex)) != HOT_OK)
     hot_sys_Panic(hot_err_ErrString(err));
}

/* Create a message.  Returns the new message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Create
#endif
MSG_INLINE
hot_msg_t hot_msg_Create(void) {
  struct hot_msg_body *body;
  struct hot_msg_ctl *ctl;
  struct hot_msg_handle *handle;
  hot_err_t err;
  unsigned int k;
  int new_msg = 0;

  if (!global.initialized)
    hot_msg_Init();

  if ((err = hot_lck_Lock(global.mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  /* Use a message from the free list, if there's any.  Otherwise,
   * allocate a new msg.
   */
  if (global.nfree) {
    body = global.free_list;
    if (body == NULL)
      hot_sys_Panic("hot_msg_Create:  internal inconsistency");
    global.free_list = body->ctl.next;    
    global.nfree--;
  }
  else {
    printf("** WARNING: hot_msg_Create: No messages left on the free list.\n");
    printf("** This might be caused by a memory leak\n");
    
    body = (struct hot_msg_body *) hot_mem_Alloc(
        global.memory, sizeof(struct hot_msg_body));
    if (body == NULL)      
      hot_sys_Panic("hot_msg_Create: malloc");
    new_msg = 1;
  }

  if ((err = hot_lck_Unlock(global.mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  /* Initialize control data and handle.
   */
  ctl = &body->ctl;

  if (new_msg) {
      /* If this is a newly allocated msg, initialize the lock.  Otherwise,
       * reuse the existing lock.
       */
      if ((err = hot_lck_Create(&ctl->mutex)) != HOT_OK)
	  hot_sys_Panic(hot_err_ErrString(err));
  }

  ctl->nused = 1;
  ctl->used_flags = HOT_BIT(0);
  ctl->writable = 1;
  ctl->size = 0;
  ctl->need_release = 0;
  /* ijh: HP cc complains so let's split this up */
  /*ctl->env = ctl->on_release = NULL;*/
  ctl->env = NULL;
  ctl->on_release = NULL;  
  ctl->data = &body->data[0];

  /* The beginning of a message buffer is 4-byte aligned.
   */
  assert(((unsigned)ctl->data) % 4 == 0);

  ctl->alloc_size = HOT_MSG_MAX_PREALLOC_SIZE;
  ctl->magic = HOT_MSG_CTL_MAGIC;

  for (k = 1; k < HOT_MSG_MAX_ALIASES; k++) {
      ctl->handles[k].magic = 0;
  }
  
  handle = &ctl->handles[0];
  handle->ctl = ctl;
  handle->offset = 0;
  handle->read_dir = HOT_MSG_READ_LEFT;
  handle->index = 0;
  handle->magic = HOT_MSG_HANDLE_MAGIC;

  /*printf("HOT_MSG: CREATE: %p : %d\n", ctl, handle->index);
   */

  return (hot_msg_t) handle;
}

/* Release a message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Release
#endif
MSG_INLINE
hot_err_t hot_msg_Release(hot_msg_t *msgp) {
    struct hot_msg_handle *handle;
    struct hot_msg_ctl *ctl;
    int nused;
    hot_err_t err;
    
    /* Consistency check.
     */
    if (msgp == NULL || *msgp == NULL)
	return hot_err_Create(0, "bad handle");
    
    handle = (struct hot_msg_handle*) *msgp;

    /*printf("HOT_MSG: RELEASE: %p : %d\n", handle->ctl, handle->index);
     */

    if (handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");
    
    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");
    
    *msgp = NULL;
    
    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    /* Release the handle
     */
    ctl->used_flags &= ~(HOT_BIT(handle->index));
    nused = --ctl->nused;
    handle->magic = 0;
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    /* If there are no more references to this message, put it on the free list.
     */
    if (nused == 0) {
	ctl->magic = 0;
	
	/* Call the on_release function if necessary.
	 */
	if (ctl->need_release && (ctl->on_release != NULL))
	    (*ctl->on_release)(ctl->env);

	/* 
	 * Recycle msg mutexes.
	 *
	 * if ((err = hot_lck_Destroy(ctl->mutex)) != HOT_OK)
	 *     hot_sys_Panic(hot_err_ErrString(err));
	 */

	if ((err = hot_lck_Lock(global.mutex)) != HOT_OK)
	    hot_sys_Panic(hot_err_ErrString(err));
	
	ctl->next = global.free_list;
	/* Here assuming ctl is the first field in the hot_msg_body struct.
	 */
	global.free_list = (struct hot_msg_body*) ctl;
	global.nfree++;
	
	if ((err = hot_lck_Unlock(global.mutex)) != HOT_OK)
	    hot_sys_Panic(hot_err_ErrString(err));
    }
    
    return HOT_OK;
}

/* Return total size of the message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Size
#endif
MSG_INLINE
hot_err_t hot_msg_Size(hot_msg_t msg, /*OUT*/ unsigned *sizep) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl;
  unsigned size; 
  hot_err_t err;

  /* Consistency check.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");
  
  ctl = handle->ctl;
  if (ctl->magic != HOT_MSG_CTL_MAGIC)
      return hot_err_Create(0, "bad message");
  
  if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  size = ctl->size;

  if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
  
  *sizep = size;
  return HOT_OK;
}

/* Set the read direction.  By default, messages are read from left
 * to right (first written/last read).  This can changed to read
 * from left to right (first written/first read).
 */
hot_err_t hot_msg_ReadDirection(hot_msg_t msg, hot_msg_readDirection_t dir) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;

  /* Consistency checks.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");

  if (handle->ctl->magic != HOT_MSG_CTL_MAGIC)
    return hot_err_Create(0, "bad message");
  
  handle->read_dir = dir;
  return HOT_OK;
}

/* Move message pointer to the right to the next alignment boundary.
 * The beginning of message data buffers is always 4-byte aligned.
 */ 
hot_err_t hot_msg_Align(hot_msg_t msg, unsigned nbytes) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl;
  hot_err_t result = HOT_OK;
  hot_err_t err;
  unsigned pad;

  /* Consistency check.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");
  
  ctl = handle->ctl;
  if (ctl->magic != HOT_MSG_CTL_MAGIC)
    return hot_err_Create(0, "bad message");
  
  pad = nbytes - ((unsigned)handle->offset % nbytes);
  if (pad == nbytes) pad = 0;

  if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  /* If message is not writable, alignment must not increase the offset
   * beyond the current boundary.
   */
  if (!ctl->writable) {
    if (handle->offset + pad > ctl->size) {
      result = hot_err_Create(0, "message boundary violation");
      goto return_point;
    }

    handle->offset += pad;
  }
  else {
    /* Consistency check.
     */
    if (ctl->size != handle->offset) {
      result = hot_err_Create(0, "internal integrity violation");
      goto return_point;
    }
    
    /* Fit message buffer to the new size.
     */ 
    hot_msg_FitToSize(ctl, handle->offset + pad);

    ctl->size += pad;
    handle->offset = ctl->size;
  }

return_point:
  
  if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
    return result;
}

/* Seek to the specified position (for reads only).
 * The message becomes read-only.  
 * 
 * Message is written from left to right and read from right to left.
 * Seeks with positive offsets are to the right, and seeks with negative
 * offsets are to the left.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Seek
#endif
MSG_INLINE
hot_err_t hot_msg_Seek(hot_msg_t msg, hot_msg_seek_t whence, unsigned offset) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl;
  int size, start, new_offset;
  hot_err_t err;

  /* Consistency check.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");

  ctl = handle->ctl;
  if (ctl->magic != HOT_MSG_CTL_MAGIC)
      return hot_err_Create(0, "bad message");
  
  if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  ctl->writable = 0;
  size = ctl->size;

  if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));

  switch (whence) {
  case HOT_MSG_SEEK_FROM_HEAD: 
    start = size;
    break;
  case HOT_MSG_SEEK_FROM_TAIL:
    start = 0;
    break;
  case HOT_MSG_SEEK_FROM_CURRENT:
    start = handle->offset;
    break;
  default:
    return hot_err_Create(0, "bad argument (whence)");
  }

  new_offset = start + offset;
  if ((new_offset < 0) || (new_offset > size))
    return hot_err_Create(0, "bad offset");

  handle->offset = new_offset;
  return HOT_OK;
}

/* Get the current read offset in the message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_GetPos
#endif
MSG_INLINE
hot_err_t hot_msg_GetPos(hot_msg_t msg, /*OUT*/ unsigned *posp) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;

  /* Consistency check.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");

  if (handle->ctl->magic != HOT_MSG_CTL_MAGIC)
      return hot_err_Create(0, "bad message");
    
  *posp = handle->offset;
  return HOT_OK;
}

/* Set read-offset pointer to the specified position.
 * The message becomes read-only.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_SetPos
#endif
MSG_INLINE
hot_err_t hot_msg_SetPos(hot_msg_t msg, unsigned offset) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl;
  hot_err_t result = HOT_OK;
  hot_err_t err;

  /* Consistency check.
   */
  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
      return hot_err_Create(0, "bad handle");

  ctl = handle->ctl;
  if (ctl->magic != HOT_MSG_CTL_MAGIC)
      return hot_err_Create(0, "bad message");
  
  if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  ctl->writable = 0;

  if (offset < 0 || offset > ctl->size) {
    result = hot_err_Create(0, "bad offset");
    goto return_point;
  }

  handle->offset = offset;

return_point:

  if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  return result;
}

/* Make an alias of the specified message.  Both the original message
 * and the alias become read-only.  The alias has its own read-offset pointer. 
 *
 * In case of success, the pointer to the alias is returned in aliasp.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Alias
#endif
MSG_INLINE
hot_err_t hot_msg_Alias(hot_msg_t msg, /*OUT*/ hot_msg_t *aliasp) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    hot_err_t result = HOT_OK;
    struct hot_msg_handle *alias;
    hot_err_t err;
    
    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");
    
    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");
    
    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    if (ctl->nused == HOT_MSG_MAX_ALIASES)
	result = hot_err_Create(0, "too many aliases");
    else {
	unsigned int i;
	for (i = 0; i < HOT_MSG_MAX_ALIASES; i++) {
	    /* Use the first available handle.
	     */
	    if ((ctl->used_flags & HOT_BIT(i)) == 0) {
		ctl->used_flags |= HOT_BIT(i);
		ctl->nused++;
		alias = &ctl->handles[i];
		alias->offset = handle->offset;
		alias->ctl = ctl;
		alias->read_dir = handle->read_dir;
		alias->index = i;
		alias->magic = HOT_MSG_HANDLE_MAGIC;
		*aliasp = (hot_msg_t) alias;
		break;
	    }
	}
    
	if (i >= HOT_MSG_MAX_ALIASES)
	    hot_sys_Panic("hot_msg_Alias:  internal inconsistency");
    }
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    /*printf("HOT_MSG: ALIAS: %p : %d\n", ctl, alias->index);
     */

    return result;
}

/* Make a writeable copy of the specified message.
 *
 * In case of success, the pointer to the copy is returned in copyp.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Copy
#endif
MSG_INLINE
hot_err_t hot_msg_Copy(hot_msg_t msg, /*OUT*/ hot_msg_t *copyp) {
  struct hot_msg_handle *handle_src = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl_src;
  struct hot_msg_handle *handle_dest;
  struct hot_msg_ctl *ctl_dest;
  hot_err_t err;

  /* Consistency check.
   */
  if (handle_src == NULL || handle_src->magic != HOT_MSG_HANDLE_MAGIC)
    return hot_err_Create(0, "bad handle");
  ctl_src = handle_src->ctl;
  if (ctl_src->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");

  /* Allocate a new message.
   */
  handle_dest = (struct hot_msg_handle*) hot_msg_Create();  
  ctl_dest = handle_dest->ctl;

  /* Fit copy's buffer to the source's size.
   */ 
  hot_msg_FitToSize(ctl_dest, handle_src->offset);

  if ((err = hot_lck_Lock(ctl_src->mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  /* Copy the tail of the source into the dest.
   */
  memcpy(ctl_dest->data, ctl_src->data, handle_src->offset);
  handle_dest->offset = ctl_dest->size = handle_src->offset;

  if ((err = hot_lck_Unlock(ctl_src->mutex)) != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
  
  *copyp = (hot_msg_t) handle_dest;
  return HOT_OK;
}

/* Create a read-only message with the specified buffer as its contents.
 * When the message is to be released, the specified function is invoked.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_CreateDontCopy
#endif
MSG_INLINE
hot_msg_t hot_msg_CreateDontCopy(void *data,
				 unsigned nbytes,
				 void (*on_release)(void*), 
				 void *env) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) hot_msg_Create();
  struct hot_msg_ctl *ctl = handle->ctl;
  ctl->env = env;
  ctl->need_release = 1;
  ctl->on_release = on_release;
  ctl->data = data;

  /* The beginning of a message buffer must be 4-byte aligned.
   */
  assert(((unsigned)ctl->data) % 4 == 0);

  handle->offset = ctl->size = nbytes;
  ctl->writable = 0;

  return (hot_msg_t) handle;
}

/* Write a buffer to the message.  The total size of the message is
 * limited to under 4K in the current implementation.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Write 
#endif
MSG_INLINE
hot_err_t hot_msg_Write(hot_msg_t msg, void *data, unsigned nbytes) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    hot_err_t result = HOT_OK;
    hot_err_t err;
    
    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");
    
    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");
    
    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    /* Message must be writable.
     */
    if (!ctl->writable) {
	result = hot_err_Create(0, "message not writable");
	goto return_point;
    }
    
    /* Consistency check.
     */
    if (ctl->size != handle->offset) {
	result = hot_err_Create(0, "internal integrity violation");
	goto return_point;
    }
    
    /* Fit message buffer to the new size.
     */ 
    hot_msg_FitToSize(ctl, handle->offset + nbytes);
    
    /* Copy the new data.
     */
    memcpy(ctl->data + ctl->size, data, nbytes);
    ctl->size += nbytes;
    handle->offset = ctl->size;
    
  return_point:
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    return result;
}

/* Read data from the message.  In case of success, the data is copied to the
 * specified buffer preallocated by the caller.
 * The message becomes read-only.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Read 
#endif
MSG_INLINE
hot_err_t hot_msg_Read(hot_msg_t msg, void *buf, unsigned nbytes) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    hot_err_t result = HOT_OK;
    hot_err_t err;
    
    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");
    
    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");
    
    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    /* Copy the data.  Message becomes read-only.
     */
    switch (handle->read_dir) {
    case HOT_MSG_READ_LEFT:
      /* Check for message boundary violations.
       */
      if (nbytes > handle->offset) {
	result = hot_err_Create(0, "message boundary violation");
	goto return_point;
      }
      
      handle->offset -= nbytes;
      memcpy(buf, ctl->data + handle->offset, nbytes);
      break;

    case HOT_MSG_READ_RIGHT:
      /* Check for message boundary violations.
       */
      if (handle->offset + nbytes > ctl->size) {
	result = hot_err_Create(0, "message boundary violation");
	goto return_point;
      }

      memcpy(buf, ctl->data + handle->offset, nbytes);
      handle->offset += nbytes;
      break;

    default:
      result = hot_err_Create(0, "invalid read direction");
      goto return_point;
    }

    ctl->writable = 0;
    
  return_point:
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    return result;
}

/* Read the message without copying data.  Current offset changes as in read. 
 * In case of success, pointer to data is returned in datap.  
 * The message becomes read-only.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Look 
#endif
MSG_INLINE
hot_err_t hot_msg_Look(hot_msg_t msg, unsigned nbytes, /*OUT*/ void **datap) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    hot_err_t result = HOT_OK;
    hot_err_t err;
    
    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");

    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");

    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    switch (handle->read_dir) {
    case HOT_MSG_READ_LEFT:
      /* Check for message boundary violations.
       */
      if (nbytes > handle->offset) {
	result = hot_err_Create(0, "message boundary violation");
	goto return_point;
      }
      
      handle->offset -= nbytes;
      *datap = ctl->data + handle->offset;
      break;

    case HOT_MSG_READ_RIGHT:
      /* Check for message boundary violations.
       */
      if (handle->offset + nbytes > ctl->size) {
	result = hot_err_Create(0, "message boundary violation");
	goto return_point;
      }

      *datap = ctl->data + handle->offset;
      handle->offset += nbytes;
      break;

    default:
      result = hot_err_Create(0, "invalid read direction");
      goto return_point;
    }

    ctl->writable = 0;
    
  return_point:
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    return result;    
}

/* Pack messages into the specified destination.  
 *
 * Only tails of messages are packed (unread portions).  Note that the 
 * tail of a writeable message is the entire message.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Pack
#endif
MSG_INLINE
hot_err_t hot_msg_Pack(hot_msg_t msg, hot_msg_t *msgs, unsigned nmsgs) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    hot_err_t err = HOT_OK;
    unsigned magic = HOT_MSG_PACKED_MAGIC;
    unsigned new_size;
    unsigned int i;
    hot_uint32 tmp ;
    
    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad destination handle");
    if (msgs == NULL)
	return hot_err_Create(0, "NULL source msgs");

    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad destination message");

    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));

    /* Destination message must be writable.
     */
    if (!ctl->writable) {
	err = hot_err_Create(0, "destination message not writable");
	goto return_point;
    }
    
    /* Compute the new size of the destination message. 
     */
    new_size = ctl->size;
    for (i = 0; i < nmsgs; i++) {
	struct hot_msg_handle *src_handle = (struct hot_msg_handle*) msgs[i];
	if (src_handle == NULL) {
	    err = hot_err_Create(0, "NULL source message");
	    goto return_point;
	}
	new_size += src_handle->offset;
    }
    new_size += sizeof(nmsgs) + sizeof(magic) + nmsgs * sizeof(handle->offset);

    /* Fit message buffer to the new size.
     */ 
    hot_msg_FitToSize(ctl, new_size);
    
    /* Go over all messages.  For each message, we save its size
     * and the contents. 
     */
    for (i = 0; i < nmsgs; i++) {
	struct hot_msg_handle *m_handle = (struct hot_msg_handle*) msgs[i];
	struct hot_msg_ctl *m_ctl = m_handle->ctl;

	if ((err = hot_lck_Lock(m_ctl->mutex)) != HOT_OK)
	    hot_sys_Panic(hot_err_ErrString(err));

	/* Copy the data and msg size.
	 */
	memcpy(ctl->data + ctl->size, m_ctl->data, m_handle->offset);
	ctl->size += m_handle->offset;
	tmp = htonl(m_handle->offset) ;
	memcpy(ctl->data + ctl->size, &tmp, 
	       sizeof(tmp));
	ctl->size += sizeof(m_handle->offset);    
      
	if ((err = hot_lck_Unlock(m_ctl->mutex)) != HOT_OK)
	    hot_sys_Panic(hot_err_ErrString(err));
    }

    /* Save the number of messages and the magic number.
     */
    tmp = htonl(nmsgs) ;
    memcpy(ctl->data + ctl->size, &tmp, sizeof(tmp));
    ctl->size += sizeof(tmp);
    tmp = htonl(magic) ;
    memcpy(ctl->data + ctl->size, &tmp, sizeof(tmp));
    ctl->size += sizeof(tmp);

    handle->offset = ctl->size;	

return_point:

    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    return err;
}

/* Unpack a message.  The message becomes read-only.
 * In case of success, returns the pointer an array of unpacked 
 * messages in msgsp and array size in *nmsgsp.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_Unpack
#endif
MSG_INLINE
hot_err_t hot_msg_Unpack(hot_msg_t msg, /*OUT*/ hot_msg_t **msgsp, 
			 /*OUT*/ unsigned *nmsgsp) {
    struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
    struct hot_msg_ctl *ctl;
    struct hot_msg_handle **msgs = NULL ; /* to avoid -Wall warning */
    unsigned nmsgs = 0 ;	/* to avoid -Wall warning */
    hot_err_t result = HOT_OK;
    unsigned int i ;
    int cancel = 0;
    unsigned magic;
    hot_err_t err;
    hot_uint32 tmp ;
    int j;

    /* Consistency check.
     */
    if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
	return hot_err_Create(0, "bad handle");

    ctl = handle->ctl;
    if (ctl->magic != HOT_MSG_CTL_MAGIC)
	return hot_err_Create(0, "bad message");
    
    /* Consistency check.
     */
    if (handle->offset < sizeof(magic)) 
	return hot_err_Create(0, "bad source message");
    
    if ((err = hot_lck_Lock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    ctl->writable = 0;
    
    /* Do some "typechecking".
     */
    memcpy(&tmp, ctl->data + (handle->offset -= sizeof(tmp)), sizeof(tmp));
    magic = ntohl(tmp) ;
    if (magic != HOT_MSG_PACKED_MAGIC) {
	result = hot_err_Create(0, "message is not packed");
	handle->offset += sizeof(magic);
	goto return_point;
    }
    
    /* Allocate space for included messages.
     */
    memcpy(&tmp, ctl->data + (handle->offset -= sizeof(tmp)), sizeof(tmp));
    nmsgs = ntohl(tmp); 
    msgs = (struct hot_msg_handle**) 
      hot_mem_Alloc(global.memory, nmsgs * sizeof(struct hot_msg_handle*));
    if (msgs == NULL) {
	hot_sys_Panic("hot_msg_Unpack:  malloc");
    }
    for (i = 0; i < nmsgs; i++) {
	msgs[i] = (struct hot_msg_handle*) hot_msg_Create();
    }
    
    /* Fill in the messages.
     */
    for (j = nmsgs - 1; j >= 0; j--) {
	struct hot_msg_handle *m_handle = msgs[j];
	struct hot_msg_ctl *m_ctl = m_handle->ctl;
	
	/* Consistency check.
	 */
	if (sizeof(m_ctl->size) > handle->offset) {
	    cancel = 1;     
	    result = hot_err_Create(0, "bad source message");
	    goto return_point;
	}
	
	/* Get the size of the msg.
	 */
	memcpy(&tmp, ctl->data + (handle->offset -= sizeof(tmp)),
	       sizeof(tmp));
	m_ctl->size = ntohl(tmp) ;

	m_handle->offset = m_ctl->size;
	
	/* Consistency check.
	 */
	if (m_ctl->size > handle->offset) {
	    cancel = 1;     
	    result = hot_err_Create(0, "bad source message");
	    goto return_point;
	}
	
	/* Get the data.
	 */
	memcpy(m_ctl->data, ctl->data + (handle->offset -= m_ctl->size), 
	       m_ctl->size);
    }
    
    *msgsp = (hot_msg_t*) msgs;
    *nmsgsp = nmsgs;
    
return_point:
    
    if (cancel) {
	/* Release all allocated messages.
	 */
	for (i = 0; i < nmsgs; i++)
	    hot_msg_Release((hot_msg_t) msgs[i]);
	hot_mem_Free(msgs);
    }
    
    if ((err = hot_lck_Unlock(ctl->mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
    
    return result;
}

/* Returns non-zero iff the message is writable.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_IsWritable
#endif
MSG_INLINE
int hot_msg_IsWritable(hot_msg_t msg) {
  struct hot_msg_handle *handle = (struct hot_msg_handle*) msg;
  struct hot_msg_ctl *ctl;

  if (handle == NULL || handle->magic != HOT_MSG_HANDLE_MAGIC)
    return 0;
  else {
    ctl = handle->ctl;
    return ((ctl->magic == HOT_MSG_CTL_MAGIC) && ctl->writable);
  }
}

/* Write an unsigned 32-bit integer to the message.  
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_WriteUint32
#endif
MSG_INLINE
hot_err_t hot_msg_WriteUint32(hot_msg_t msg, hot_uint32 *u) {
    hot_uint32 net;
    net = htonl(*u);
    return hot_msg_Write(msg, &net, 4);
}

/* Read an unsigned 32-bit integer from the message.  
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_msg_ReadUint32
#endif
MSG_INLINE
hot_err_t hot_msg_ReadUint32(hot_msg_t msg, hot_uint32 *u) {
    hot_uint32 net;
    hot_err_t err = hot_msg_Read(msg, &net, 4);
    *u = ntohl(net);
    return err;
}
