/*
 * Contents: C side of the HOT C interface to Ensemble 
 * in the outboard mode.
 *
 * Author:  Alexey Vaysburd, Dec. 1996.  
 * Outboard mode: July 97.
 */ 
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#ifdef _WIN32
void Perror(char *s);
#include <winsock2.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/wait.h>
#include <sys/time.h>
#define Perror(s) perror(s)
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include "hot_sys.h"
#include "hot_error.h"
#include "hot_thread.h"
#include "hot_msg.h"
#include "hot_mem.h"
#include "hot_ens.h"

static void trace(const char *s, ...) {
  va_list args;
  static int debugging = -1 ;

  va_start(args, s);

  if (debugging == -1) {
      debugging = (getenv("ENS_HOT_TRACE") != NULL) ;
  }
  
  if (!debugging) return ;

  fprintf(stderr, "HOT_OUTBOARD(C):");
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
}

/*************************** Memory channels *******************************/

static hot_mem_t ch_context, 
  ch_endpt_array, 
  ch_bool_array, 
  ch_read_buffer, 
  ch_read_message;

/*********************** Outboard configurations *******************************/

typedef hot_err_t (*hot_io_init)(void*, char * argv[], int*);
typedef int (*hot_io_write)(int, void*, int);
typedef int (*hot_io_read)(int, void*, int);

/* Configuration for Comminication with Ensemble.
 * I/O can go through a pipe or sockets etc.
 * (if an Ensemble daemon is running on a different machine).
 */
typedef struct {
  const char *name;
  hot_io_init init;
  hot_io_read read;
  hot_io_write write;
} hot_io_t;

static int tcp_recv(int s, void *buf_void, int len) {
  int result, ofs;
  char *buf = buf_void ;

  trace("tcprecv: %d 0x%x %d",s,buf,len);
  for (ofs=0 ; ofs < len ; ofs += result) {
    result = recv(s, buf+ofs, len-ofs, 0);
    trace("tcprecv: got %d bytes", result);

    if (result == 0) {
      hot_sys_Panic("lost connection to outboard") ;
    }

    if (result == -1) {
#ifdef EINTR
	if (errno == EINTR) continue ;
#endif
      Perror("recv");
      hot_sys_Panic("lost connection to outboard") ;
    }
  }

  return len;
}

static int tcp_send(int s, void *buf_void, int len) {
  int result, ofs;
  char *buf = buf_void ;

  trace("tcpsend: %d 0x%x %d",s,buf,len);
  for (ofs=0 ; ofs < len ; ofs += result) {
    result = send(s, buf+ofs, len-ofs, 0);
    if (result == -1) {
#ifdef EINTR
      if (errno == EINTR) continue ;
#endif
      Perror("send");
      hot_sys_Panic("lost connection to outboard") ;
    }
  }
  return len;
}

#ifndef WIN32
/* Need a well-known port for connect to server */
#define OUTBOARD_TCP_PORT 5002

hot_err_t ens_tcp_init(void *env, char * argv[], /*OUT*/ int *fd) {
    int result ;
    int sinlen ;
    struct hostent *hep ;
    char  my_host[64];
    struct sockaddr_in sin;

    trace("ens_tcp_init");
    /* Get my host name in preparation for connect */
    result = gethostname(my_host, 64);
    if (result < 0) {
      hot_sys_Panic("gethostname");
    }
      
    /* Use my host name to get hostent in preparation for connect */

    memset(&sin, 0, sizeof(sin)) ;
    hep = gethostbyname(my_host);
    if (hep == (struct hostent *)NULL) {
      // try localhost
      inet_aton("127.0.0.1",&(sin.sin_addr));
      //hot_sys_Panic("gethostbyname");
    } else {
    /* Copy hostent's addr into sin_addr.
     */
      memcpy(&(sin.sin_addr),hep->h_addr,hep->h_length);
    }

    sin.sin_family = AF_INET;
    sin.sin_port = ntohs(OUTBOARD_TCP_PORT);
    sinlen = sizeof(sin);  
    
    trace("TCP connect to %s port %d", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port)); fflush(stdout);
    
    /* Now call connect in a loop, waiting for accept */
    
    {
      int sock ;
      
      /* Create our socket.
       */
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock < 0) {
	hot_sys_Panic("socket");
      }
      
      result = connect(sock, (struct sockaddr *)&sin, sinlen) ;
      if (result == -1) {
	Perror("connect") ;
	hot_sys_Panic("connect") ;
      }
      
      *fd = (int)sock;
    }
    
    trace("TCP connected");
    
    return HOT_OK;
}

#endif

#ifndef WIN32
/* Communication with Ensemble through TCP sockets.  
 * A server should already exist.
 */
static hot_io_t hot_io_tcp = { 
  "TCP",
  ens_tcp_init,
  (hot_io_read) tcp_recv,
  (hot_io_write) tcp_send,
};
#endif /*WIN32*/

#ifndef WIN32

hot_err_t ens_fork_tcp_init(void *env, char * argv[], int*) ;

/* Communication with Ensemble through a pipe.  
 * Ensemble is forked in a separate process.
 */
static hot_io_t hot_io_tcp_fork = { 
  "FORK",
  ens_fork_tcp_init,
  (hot_io_read) tcp_recv,
  (hot_io_write) tcp_send, 
} ;

#endif

#ifdef WIN32
hot_err_t ens_spawn_tcp_init(void *env, char * argv[], int*);

static hot_io_t hot_io_tcp_spawn = {
  "SPAWN",
  ens_spawn_tcp_init,
  (hot_io_read) tcp_recv,
  (hot_io_write) tcp_send,
};
#endif

static hot_io_t *hot_io_conf[10] = {
#ifndef WIN32
  /* The normal configuration is to fork an outboard process that
   * uses TCP to talk with hot_outboard_c.c. If you want to create
   * outboard by hand, modify hot_test to use "TCP" instead of
   * "FORK" as outboard. This also requires starting outboard manually
   * with the -tcp options, e.g.:
   *    ./outboard -tcp_port 5002
   */
  &hot_io_tcp_fork,
  &hot_io_tcp,
#endif
#ifdef WIN32
  /* NT can only use TCP */
  &hot_io_tcp_spawn,
#endif
  NULL
}; 

static hot_io_t* hot_io_get(char *name) {
  int i;
  for (i = 0; hot_io_conf[i]; i++)
    if (strcmp(hot_io_conf[i]->name, name) == 0)
      return hot_io_conf[i];
  return NULL;
}

/*******************************************************************************/

typedef hot_uint32 hot_contextID_t ;
typedef hot_gctx_t hot_context_t ;

/* This describes a group context.
 */
struct hot_gctx {
  hot_contextID_t id ;		/* unique integer identifier */
  hot_context_t next;		/* linked list of alive members */
  hot_ens_cbacks_t conf;	/* application callbacks */
  void *env;			/* application state */
  unsigned nmembers ;		/* #members */
  hot_endpt_t *view ;		/* view of the group */
  int blocked;			/* is group blocked? */
  int joining ;			/* are we joining? */
  int leaving ;			/* are we leaving? */
} ;

 /****************************************************************************/

/* Global state.
 */
static struct {
  hot_lock_t mutex;		/* global mutex lock  */
  int in_critical;		/* Set when mutex is locked */
  int initialized;		/* Set when initialized. */
  hot_context_t g_alive ;	/* allocated group contexts */
  int fd;			/* fd for communication w/ Ensemble */
  hot_io_t *io;		/* Outboard configuration */
  
  hot_lock_t r_mutex;		/* global mutex for reading */
  int in_r_critical;		/* Set when mutex is locked */
  char *read_buf ;
  int read_size ;
  int read_pos ;
  int read_hdr_len ;
  int read_iov_len ;
  unsigned read_act_size ;
  
  hot_lock_t w_mutex;		/* global mutex for writing */
  int in_w_critical;		/* Set when mutex is locked */
  char *write_buf;		/* Buffer where we are writing to */
  int write_size;		/* Buffer size */
  int write_pos;		/* Current position in buffer */
  int write_hdr_len  ;
  int write_iov_len  ;
} g ;

/*************************** Mutexes ***************************************/
#ifdef INLINE_PRAGMA
#pragma inline begin_critical
#endif
MSG_INLINE
static void begin_critical(void) {
  hot_err_t err ;
  if ((err = hot_lck_Lock(g.mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err)) ;
  assert(!g.in_critical) ;
  g.in_critical = 1 ;
}

#ifdef INLINE_PRAGMA
#pragma inline end_critical
#endif
MSG_INLINE
static void end_critical(void) {
  hot_err_t err ;
  assert(g.in_critical) ;
  g.in_critical = 0 ;
  if ((err = hot_lck_Unlock(g.mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
}

#ifdef INLINE_PRAGMA
#pragma inline begin_write
#endif
MSG_INLINE
static void begin_write(void) {
  hot_err_t err ;
  if ((err = hot_lck_Lock(g.w_mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err)) ;
  assert(!g.in_w_critical) ;
  g.in_w_critical = 1 ;
  g.write_hdr_len = 0 ;
  g.write_iov_len = 0 ;
}

#ifdef INLINE_PRAGMA
#pragma inline do_write
#endif
MSG_INLINE
static void do_write(void *buf,int len) {
  assert(g.in_w_critical) ;

  if (len > g.write_size - g.write_pos) {
    if (g.write_size <= 1024)
      g.write_size = 1024 ;
    while (len > g.write_size - g.write_pos)
      g.write_size *= 2 ;
    if (!g.write_buf)
      g.write_buf = hot_mem_Alloc(ch_read_buffer, g.write_size) ;
    else
      g.write_buf = hot_mem_Realloc(g.write_buf, g.write_size) ;
  }

  memcpy(((char*)g.write_buf)+g.write_pos, buf, len) ;
  g.write_pos += len ;
}

/* Take note to first write the ML length, and then
 * the iovec length. 
 */
#ifdef INLINE_PRAGMA
#pragma inline end_write
#endif
MSG_INLINE
static void end_write(void) {
  hot_err_t err ;
  hot_uint32 size ;
  int ret ;

  assert(g.in_w_critical) ;
  g.in_w_critical = 0 ;

  /* Compute header length, and ML header length.
   * In case these arguments have not been set, then
   * this must be a case where send_msg has not been
   * called. Hence, the iovec length is zero, and the
   * ML header length is equal to our current position
   * in the write buffer. 
   */
  if (g.write_hdr_len == 0 && g.write_iov_len == 0) {
    g.write_hdr_len = g.write_pos;
    g.write_iov_len = 0;
  }
  trace("  **** write_hdr_len=%d write_iov_len=%d  ****",
	g.write_hdr_len, g.write_iov_len);
  
  /* Write header length
   */
  size = htonl(g.write_hdr_len) ;
  ret = (*g.io->write)(g.fd, &size, sizeof(size)) ;
  assert(ret == sizeof(size));

  /* Write iovec length
   */
  size = htonl(g.write_iov_len) ;
  ret = (*g.io->write)(g.fd, &size, sizeof(size)) ;
  assert(ret == sizeof(size));
  
  ret = (*g.io->write)(g.fd, g.write_buf, g.write_pos) ;
  assert(ret == g.write_pos);
  
  g.write_pos = 0 ;
  
  /* Shrink the buffer if it is >64K.
   */
  if (g.write_size > 1<<16) {
    g.write_size = 1<<12 ;
    g.write_buf = hot_mem_Realloc(g.write_buf, g.write_size) ;
  }

  if ((err = hot_lck_Unlock(g.w_mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
}

#ifdef INLINE_PRAGMA
#pragma inline begin_read
#endif
MSG_INLINE
static void begin_read(void) {
  hot_err_t err ;
  int ret ;
  hot_uint32 net_size ;
  hot_uint32 size ;

  if ((err = hot_lck_Lock(g.r_mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err)) ;
  assert(!g.in_r_critical) ;
  g.in_r_critical = 1 ;

  /* Read the length, this is the total ML and iovec length.
   * A total of 8 bytes should be read. 
   */
  ret = (*g.io->read)(g.fd, &net_size, sizeof(net_size)) ;
  assert(ret == sizeof(net_size));
  g.read_hdr_len = ntohl(net_size) ;
  ret = (*g.io->read)(g.fd, &net_size, sizeof(net_size)) ;
  assert(ret == sizeof(net_size));
  g.read_iov_len = ntohl(net_size);
  size = g.read_hdr_len + g.read_iov_len;
  
  /* Ensure there is enough room in the read buffer.
   */
  if (size > g.read_act_size) {
    if (g.read_act_size <= 1024)
      g.read_act_size = 1024 ;
    while(size > g.read_act_size)
      g.read_act_size *= 2 ;
    if (g.read_buf) {
      g.read_buf = hot_mem_Realloc(g.read_buf, g.read_act_size) ;
    } else {
      g.read_buf = hot_mem_Alloc(ch_read_buffer, g.read_act_size) ;
    }
  }

  /* Keep reading until we have the entire message.
   * This loop actually isn't needed because the read
   * and write functions loop as well.
   */
  g.read_pos = 0 ;
  g.read_size = size ;
  while (g.read_pos < g.read_size) {
    ret = (*g.io->read)(g.fd, ((char*)g.read_buf)+g.read_pos,g.read_size-g.read_pos);
    assert(ret != -1);
    g.read_pos += ret ;
  }
  g.read_pos = 0 ;
}

#ifdef INLINE_PRAGMA
#pragma inline do_read
#endif
MSG_INLINE
static void do_read(void *buf,int len) {
  assert(g.in_r_critical) ;
  assert(len <= g.read_size - g.read_pos) ;
  memcpy(buf,((char*)g.read_buf)+g.read_pos,len) ;
  g.read_pos += len ;
}

#ifdef INLINE_PRAGMA
#pragma inline end_read
#endif
MSG_INLINE
static void end_read(void) {
  hot_err_t err ;
  assert(g.in_r_critical) ;

  assert(g.read_pos == g.read_size) ;

  g.read_pos = 0 ;
  g.read_size = 0 ;

  g.in_r_critical = 0 ;
  if ((err = hot_lck_Unlock(g.r_mutex)) != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
}

/*********************** Endpoint Arrays ******************************/

static hot_endpt_t* alloc_endpt_array(
    hot_mem_t ch,
    unsigned nendpts
) {
    int size = nendpts * sizeof(hot_endpt_t);
    assert(nendpts > 0);    
    return hot_mem_Alloc(ch, size);
}
    
static void release_endpt_array(
    hot_mem_t ch,
    hot_endpt_t *endpts,
    unsigned nendpts
) {
    assert(endpts);
    assert(nendpts > 0);
    hot_mem_Free(endpts);
}

/*********************** Bool Arrays ******************************/

static hot_bool_t* alloc_bool_array(
    hot_mem_t ch,
    unsigned nbool
) {
    int size = nbool * sizeof(hot_bool_t);
    assert(nbool > 0);    
    return hot_mem_Alloc(ch, size);
}
    
static void release_bool_array(
    hot_mem_t ch,
    hot_bool_t *bools,
    unsigned nbool
) {
    assert(bools);
    assert(nbool > 0);
    
    hot_mem_Free(bools);
}

/*************************** Ensemble I/O  ********************************/

/* Downcalls into Ensemble.  Must match the ML side.
 */
typedef enum {
    DN_JOIN,
    DN_CAST,
    DN_SEND_BLOCKED,
    DN_SUSPECT,
    DN_XFERDONE,
    DN_PROTOCOL,
    DN_PROPERTIES,
    DN_LEAVE,
    DN_PROMPT,
    DN_REKEY,
    DN_BLOCK_OK,
    DN_SEND
} hot_dnType_t;

static const char *string_of_dntype(hot_dnType_t t) {
    switch (t) {
#define CASE(s) case s: return #s ; break
        CASE(DN_JOIN) ;
        CASE(DN_CAST) ;
        CASE(DN_SEND) ;
        CASE(DN_SUSPECT) ;
	CASE(DN_XFERDONE) ;
        CASE(DN_PROTOCOL) ;
        CASE(DN_PROPERTIES) ;
        CASE(DN_LEAVE) ;
        CASE(DN_PROMPT) ;
	CASE(DN_REKEY) ;
        CASE(DN_BLOCK_OK) ;
        CASE(DN_SEND_BLOCKED) ;
#undef CASE
    default:
      assert(0) ;
      return "BAD_DNTYPE" ;
    }
}

/* Callbacks from Ensemble.  Must match the ML side.
 */
typedef enum {
    CB_VIEW,
    CB_CAST,
    CB_SEND,
    CB_HEARTBEAT,
    CB_BLOCK,
    CB_EXIT
} hot_cbType_t;

/* Magic number for checksums. Must match the ML side.
 */
#define HOT_MAGIC 3657185909
#define INT_SIZE sizeof(hot_uint32)

/* Mutual exclusion:  all read operations are assumed to be performed within
 * a single thread, hence no mutual exclusion.  Write operations
 * (including check_write_errors()) should only be performed within 
 * begin_write()/end_write() blocks.
 */
#ifdef INLINE_PRAGMA
#pragma inline write_int
#endif
HOT_INLINE
static void write_int(
    hot_uint32 u
) {
    hot_uint32 tmp = htonl(u);
    do_write(&tmp,INT_SIZE) ;
    trace("write_int: %d", u);
}

#ifdef INLINE_PRAGMA
#pragma inline write_bool
#endif
HOT_INLINE
static void write_bool(
    hot_bool_t b
) {
    hot_uint32 u32 = b;
    write_int(u32);
}

#ifdef INLINE_PRAGMA
#pragma inline write_buffer 
#endif
HOT_INLINE
static void write_buffer(
    void *body,
    unsigned size
) {
    hot_uint32 u32 = htonl(size);

    assert(body);

    do_write(&u32, INT_SIZE) ;
    do_write(body, size) ;

    trace("write_buf_len: %d", size);
    trace("write_buffer: %s", (char*) body);
}

#ifdef INLINE_PRAGMA
#pragma inline write_string 
#endif
HOT_INLINE
static void write_string(
    char *s
) {
    assert(s);
    write_buffer(s,strlen(s) /*+ 1*/); 
}

#ifdef INLINE_PRAGMA
#pragma inline write_endpID
#endif
HOT_INLINE
static void write_endpID(
    hot_endpt_t *ep
) {
    write_string(ep->name);
}

struct header {
  hot_uint32 id ;
  hot_uint32 type ;
} ;

#ifdef INLINE_PRAGMA
#pragma inline write_hdr
#endif
HOT_INLINE
static void write_hdr(
    hot_context_t s,
    hot_dnType_t type
) {
    struct header h ;
    assert(s) ;
    trace("write_hdr:id=%d, type=%d(%s)",s->id, type, string_of_dntype(type)) ;
    h.id = htonl(s->id) ;
    h.type = htonl(type) ;
    do_write(&h,sizeof(h)) ;
}

/* write_msg is the last call before a message is packaged and
 * sent. Therefore, we record the iovec length, and the header
 * length, so that end_write will write this on the outgoing
 * packet. 
 */
#ifdef INLINE_PRAGMA
#pragma inline write_msg
#endif
HOT_INLINE
static void write_msg(
    hot_msg_t msg
) {
    void *body;
    unsigned size;
    hot_err_t err;

    err = hot_msg_GetPos(msg, &size);
    if (err != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));

    err = hot_msg_Look(msg, size, &body);
    if (err != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));

    /* Put the actual size on the message.
     */
    g.write_hdr_len  = g.write_pos;
    g.write_iov_len  = size;
   
    /* Write the body.
     */
    do_write(body, size);

    trace("write_msg: size = %d", size);
    trace("write_msg: body = %s", (char*) body);
}

#ifdef INLINE_PRAGMA
#pragma inline write_endpList
#endif
HOT_INLINE
static void write_endpList(
    int size, 
    hot_endpt_t *ep
) {
    int i;
    write_int(size);
    for (i = 0; i < size; i++)
	write_endpID(&ep[i]);
}

#ifdef INLINE_PRAGMA
#pragma inline read_int
#endif
HOT_INLINE
static void read_int(
    hot_uint32 *u
) {
    hot_uint32 u32;
    assert(u);
    do_read(&u32, INT_SIZE);
    *u = ntohl(u32);
    trace("read_int: %d", *u);
}

#ifdef INLINE_PRAGMA
#pragma inline read_bool
#endif
HOT_INLINE
static void read_bool(
    hot_bool_t *b
) {
    hot_uint32 u32;
    assert(b);
    read_int(&u32);
    *b = (u32 != 0) ;
}

/* Read into preallocated buffer.
 */
#ifdef INLINE_PRAGMA
#pragma inline read_buffer_prealloc
#endif
HOT_INLINE
static void read_buffer_prealloc(
    unsigned *size,
    void *body
) {
    hot_uint32 tmp;

    assert(size);
    assert(body);
    
    do_read(&tmp, INT_SIZE);
    *size = ntohl(tmp);
    trace("read_buffer_prealloc: size = %08x", *size);

    do_read(body, *size);
}

#ifdef INLINE_PRAGMA
#pragma inline read_string_prealloc
#endif
HOT_INLINE
static void read_string_prealloc(
    char *body
) {
    unsigned size;
    assert(body);
    read_buffer_prealloc(&size, (void*) body);
    body[size] = 0;
}

#ifdef INLINE_PRAGMA
#pragma inline read_endpID
#endif
HOT_INLINE
static void read_endpID(
    hot_endpt_t *ep
) {
    assert(ep);
    read_string_prealloc(ep->name);
}

#ifdef INLINE_PRAGMA
#pragma inline read_contextID
#endif
HOT_INLINE
static void read_contextID(
    hot_contextID_t *gid
) {
    assert(gid);
    read_int(gid);
}

/* See comments on write_msg().
 */
#ifdef INLINE_PRAGMA
#pragma inline read_msg
#endif
HOT_INLINE
static void read_msg(
    hot_msg_t *msg
) {
    void *body;
    hot_uint32 size;

    assert(msg);
    *msg = NULL ;

    /* Get the real size.
     */
    size = g.read_iov_len;
    trace("read_msg: size = %d", size);

    body = hot_mem_Alloc(ch_read_message, size);
    do_read(body,size);

    trace("read_msg: body = %s", (char*) body);

    *msg = hot_msg_CreateDontCopy(body, size, hot_mem_Free, body);    
}

#ifdef INLINE_PRAGMA
#pragma inline read_endpList
#endif
HOT_INLINE
static void read_endpList(
    int *size, 
    hot_endpt_t **ep
) {
    int i;
    hot_uint32 tmp ;
    assert(size);
    assert(ep);
    
    read_int(&tmp);
    assert(tmp > 0) ;
    *size = tmp;
    *ep = alloc_endpt_array(ch_endpt_array, *size); 
    for (i = 0; i < *size; i++)
	read_endpID(&(*ep)[i]);
}

#ifdef INLINE_PRAGMA
#pragma inline read_boolList
#endif
HOT_INLINE
static void read_boolList(
    int *size, 
    hot_bool_t **bp
) {
    int i;
    hot_uint32 tmp ;
    assert(size);
    assert(bp);
    
    read_int(&tmp) ;
    *size = tmp ;
    *bp = alloc_bool_array(ch_bool_array, *size); 
    for (i = 0; i < *size; i++)
	read_bool (*bp + i);
}

#ifdef INLINE_PRAGMA
#pragma inline read_cbType
#endif
HOT_INLINE
static void read_cbType(
    hot_cbType_t *type
) {
    hot_uint32 tmp ;
    assert(type);
    read_int(&tmp);
    *type = tmp ;
}

/************************** Callback processing ****************************/

static hot_context_t alloc_context(void);
static void release_context(hot_context_t s);
static hot_context_t lookup_context(hot_contextID_t id);

#ifdef INLINE_PRAGMA
#pragma inline cb_View
#endif
HOT_INLINE
static void cb_View(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_uint32 tmp ;
    hot_view_state_t vs;
    hot_context_t s;
    void *env;
    hot_ens_AcceptedView_cback accepted_view;
    unsigned ncl;

    trace("VIEW");

    read_string_prealloc(vs.version);
    trace("\t version: %s", vs.version);

    read_string_prealloc(vs.group_name);
    trace("\t group_name: %s", vs.group_name);

    read_endpList(&vs.nmembers, &vs.members);
    trace("\t nmembers: %d", vs.nmembers);
    { 
      unsigned i;
      for (i = 0; i < vs.nmembers; i++)
	trace("\t\t member[%d] = %s", i, vs.members[i].name);
    }

    read_int(&tmp) ;
    vs.rank = tmp ;
    trace("\t rank: %d", vs.rank);

    read_string_prealloc(vs.protocol);
    trace("\t protocol: %s", vs.protocol);

    read_bool(&vs.groupd);
    trace("\t groupd: %d", vs.groupd);

    read_int(&tmp);
    vs.view_id.ltime = tmp ;
    trace("\t view_id.ltime: %d", vs.view_id.ltime);

    read_endpID(&vs.view_id.coord);
    trace("\t view_id.coord: %s", vs.view_id.coord.name);

    read_string_prealloc(vs.params);
    trace("\t params: %s", vs.params);

    read_bool(&vs.xfer_view);
    trace("\t xfer_view: %d", vs.xfer_view);

    read_bool(&vs.primary);
    trace("\t primary: %d", vs.primary);

    read_boolList(&ncl, &vs.clients);
    assert(ncl == vs.nmembers);

    read_string_prealloc(vs.key);
    trace("\t key: %s", vs.key);

    begin_critical(); {
	s = lookup_context(id);
	env = s->env;
	accepted_view = s->conf.accepted_view;
   
	/* The group is unblocked now.
	 */
	s->blocked = 0;
    } end_critical();

    if (s->view) {
	assert(s->nmembers > 0) ;
        release_endpt_array(ch_endpt_array, s->view, s->nmembers) ;
    }
    s->nmembers = vs.nmembers ;
    assert(s->nmembers > 0) ;
    s->view = alloc_endpt_array(ch_endpt_array, s->nmembers) ;
    memcpy(s->view,vs.members,s->nmembers * sizeof(*vs.members)) ;

    if (accepted_view != NULL) {
	(*accepted_view)(s, env, &vs);
    }

    begin_critical(); {
	if (vs.members != NULL) {
	    release_endpt_array(ch_endpt_array,
				vs.members,
				vs.nmembers) ;
	}
	if (vs.clients != NULL) {
	    release_bool_array(ch_bool_array,
				vs.clients,
				vs.nmembers) ;
	}
	s->joining = 0 ;
    } end_critical();
}

#ifdef INLINE_PRAGMA
#pragma inline cb_Cast
#endif
HOT_INLINE
static void cb_Cast(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_err_t err;
    hot_endpt_t endpt;
    hot_uint32 origin ;
    hot_msg_t msg;
    hot_context_t s;
    void *env;
    hot_ens_ReceiveCast_cback receive_cast;

    trace("cb_Cast");
    read_int(&origin);
    read_msg(&msg);
    
    begin_critical(); {
	s = lookup_context(id);
	env = s->env;
	assert(origin < s->nmembers) ;
	endpt = s->view[origin] ;
	receive_cast = s->conf.receive_cast;
    } end_critical();

    if (receive_cast != NULL) {
      (*receive_cast)(s, env, &endpt, msg);
    }

    err = hot_msg_Release(&msg);
    if (err != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
}

#ifdef INLINE_PRAGMA
#pragma inline cb_Send
#endif
HOT_INLINE
static void cb_Send(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_err_t err;
    hot_endpt_t endpt ;
    hot_uint32 origin ;
    hot_msg_t msg;
    hot_context_t s;
    void *env;
    hot_ens_ReceiveSend_cback receive_send;

    trace("cb_Send");
    read_int(&origin);
    read_msg(&msg);
    
    begin_critical(); {
	s = lookup_context(id);
	env = s->env;
	assert(origin < s->nmembers) ;
	endpt = s->view[origin] ;
	receive_send = s->conf.receive_send;
    } end_critical();

    if (receive_send != NULL) {
      trace("receive_send");
	(*receive_send)(s, env, &endpt, msg);
    }

    err = hot_msg_Release(&msg);
    if (err != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
}

#ifdef INLINE_PRAGMA
#pragma inline cb_Heartbeat
#endif
HOT_INLINE
static void cb_Heartbeat(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_context_t s;
    void *env;
    hot_uint32 t;
    hot_ens_Heartbeat_cback heartbeat;

    read_int(&t);
    
    begin_critical(); {
	s = lookup_context(id);
	env = s->env;
	heartbeat = s->conf.heartbeat;
    } end_critical();
 
    if (heartbeat != NULL) {
	(*heartbeat)(s, env, t);
    }
}

#ifdef INLINE_PRAGMA
#pragma inline cb_Block
#endif
HOT_INLINE
static void cb_Block(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_context_t s;
    void *env;
    hot_ens_Block_cback block;

    begin_critical(); {
	s = lookup_context(id);
	env = s->env;
	block = s->conf.block;
	s->blocked = 1;
    } end_critical();

    if (block != NULL) {
	(*block)(s, env);
    }

    /* Write the block_ok downcall.
     */
    begin_write(); {
	write_hdr(s,DN_BLOCK_OK) ;
    } end_write();
}

#ifdef INLINE_PRAGMA
#pragma inline cb_Exit
#endif
HOT_INLINE
static void cb_Exit(
    hot_contextID_t id, 
    unsigned *cs
) {
    hot_context_t s;
    hot_ens_Exit_cback exit_cb;
    void *env;
    
    begin_critical(); {
	s = lookup_context(id);
	if (!s->leaving)
	    hot_sys_Panic("hot_ens_Exit_cbd: mbr state is not leaving");
	env = s->env;
	exit_cb = s->conf.exit;
    }  end_critical();

    if (exit_cb != NULL) {
	(*exit_cb)(s, env);
    }  

    begin_critical(); {
	release_context(s);
    } end_critical();  
}

typedef void (*cback_f)(hot_contextID_t, unsigned*);
static cback_f callback[6] = { 
    cb_View, 
    cb_Cast, 
    cb_Send, 
    cb_Heartbeat, 
    cb_Block, 
    cb_Exit 
};

/* Loop reading + processing Ensemble upcalls.
 */
static void hot_mainloop(void *arg) {
    hot_contextID_t id;
    hot_cbType_t cb;

    while (1) {
        begin_read() ; {
	    read_contextID(&id);
	    trace("CALLBACK: group ID: %d", id);

	    read_cbType(&cb);
	    trace("CALLBACK: callback type: %d", cb);

	    if (cb > CB_EXIT)
		hot_sys_Panic("bad callback type (%d)", cb);
	    (*(callback[cb]))(id, &cb);	
	} end_read () ;
    }
}

/************************** Standard Outboard Configuration *****************/

static const char * hot_ens_argv_null[2] = {"(?)", NULL};
int sock[2];


#ifndef WIN32

static pid_t cid;

static void ens_died(int signum) {
  int status;
  pid_t pid = wait(&status);
  if (pid == cid)
    hot_sys_Panic("(OUTBOARD): Ensemble died, exiting");
}

/*
static void broken_pipe() {
  hot_sys_Panic("(OUTBOARD): Broken pipe");
}
*/

static void outb_atexit(void) {
  char eof = EOF;

  /* Send a kill signal to the child.
   */
  kill(cid, SIGKILL);
  waitpid(cid, NULL, 0);
  
  /* In case signal fails, write EOF to the pipe.
   */
  begin_write() ;
  do_write(&eof, 4);
  end_write() ;
}


/* Fork Ensemble in a separate process.  
 * arg:  the file name of Ensemble outboard process.
 * Returns file descriptor to be used for communication w/ Ensemble.
 */ 
hot_err_t ens_fork_tcp_init(void *env, char * argv[], /*OUT*/ int *fd) {
    
    char *outboard = getenv("ENS_OUTBOARD");

    trace("ens_pipe_init");
    if (outboard == NULL)
	  return hot_err_Create(0,
             "ens_pipe_init: environment variable ENS_OUTBOARD must be set");
    
    argv = argv ? argv : (char **) hot_ens_argv_null;

    /* Start Ensemble in a separate process.
     */ 
    cid = fork();		/* on solaris this should be fork1() */
    if (cid < 0) {
	return hot_err_Create(0, "ens_tcp_fork_init: fork");
    }
    else if (cid == 0) {
	argv[0] = outboard;
	if (execvp(outboard, argv) < 0)
	    hot_sys_Panic("ens_pipe_init: execvp(%s)", (char*) outboard);
	assert(0);
    }

    /* Give outboard time to start.
     */
    printf("Waiting for the outboard process to start\n");
    sleep(3);

    /* Connect to outboard using TCP.
     */
    ens_tcp_init(NULL, NULL, fd);

    signal(SIGCHLD, ens_died);
    //signal(SIGPIPE, broken_pipe);
#if !(defined(sun))
    atexit(outb_atexit);
#endif

    return HOT_OK;
}
#endif

#ifdef WIN32
/* Need a well-known port for connect to child */
#define OUTBOARD_TCP_PORT 5002
static int cid;

hot_err_t ens_spawn_tcp_init(void *env, char *argv[], /*OUT*/ int *fd) {
    WSADATA         wsaData;
    SOCKET          s;

    struct hostent  *hep;
    struct sockaddr_in sin;
    
    int     i, result, sinlen;
    char    my_host[64], portstring[64], *newargv[64];
    char    *outboard;
    
    /* Call Windows socket initialization function */
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) {
		fprintf(stderr,"WSAStartup failed with error %d",WSAGetLastError());
		WSACleanup();
		hot_sys_Panic("WSAStartup");
	}

    /* If the ENS_OUTBOARD variable is set we need to spawn
     * outboard as a child. Otherwise we assume outboard was
     * started by hand and we just need to connect to it.
     */
    outboard = getenv("ENS_OUTBOARD");

    if (outboard != NULL) {
        /* fill in the argv with the arguments for outboard to
         * start in tcp mode
         */
        sprintf(portstring, "%d", OUTBOARD_TCP_PORT);
        newargv[0] = outboard;
        newargv[1] = "-tcp_port";
        newargv[2] = portstring;
        newargv[3] = 0x0;

        /* copy the app supplied args */
        for (i=1; i < 64; i++) {
            if (argv[i] == NULL) {
                newargv[2+i] = NULL;
                break;
            }
            newargv[3+i] = argv[i];
        }

        /* Spawn outboard, then set up a socket to connect to it */
        if ((cid = spawnvp(_P_NOWAIT, outboard, newargv)) == -1) {
	  printf("Could not start the outboard process: check youe ENS_OUTBOARD variable, and check that the outboard executable is OK.\n");	
	  hot_sys_Panic("spawnvp");	
        }
    }

    /* Create our socket */
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
      hot_sys_Panic("socket");
    }
  
    /* Bind our socket */
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = ntohl(INADDR_ANY);
    sin.sin_port        = ntohs(0);
  
    result = bind(s, (struct sockaddr *)&sin, sizeof(sin));
    if (result < 0) {
      closesocket(s);
      hot_sys_Panic("bind");
    }

    /* Get my host name in preparation for connect */
    result = gethostname(my_host, 64);
    if (result < 0) {
      closesocket(s);
      hot_sys_Panic("gethostname");
    }

    /* Use my host name to get hostent in preparation for connect */
    hep = gethostbyname(my_host);
    if (hep == (struct hostent *)NULL) {
      closesocket(s);
      hot_sys_Panic("gethostbyname");
    }

    /* Copy hostent's addr into sin_addr */
    memcpy(&(sin.sin_addr),hep->h_addr,hep->h_length);
    sin.sin_family      = AF_INET;
    sin.sin_port        = ntohs(OUTBOARD_TCP_PORT);
    
    sinlen = sizeof(sin);  

    trace("TCP connect to %s port %d", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    
    /* Now call connect in a loop, waiting for child's accept */
    while ((result = connect(s, (struct sockaddr *)&sin, sinlen)) == SOCKET_ERROR) {
      Sleep(1000);
    }
    
    trace("TCP connected");

    *fd = (int)s;
    
    return HOT_OK;
}
#endif
/*************************** Initialization *********************************/

/* Initialize Ensemble.
 */ 
static hot_err_t hot_ens_Init(char *outb_name, char *argv[]) {
    hot_err_t err;

    if (g.initialized)
	return hot_err_Create(0, "hot_ens_Init:  double initialization");
    
    err = hot_lck_Create(&g.mutex);
    if (err != HOT_OK) 
	return err;

    begin_critical(); {
	if (g.initialized) {
	    end_critical();
	    return hot_err_Create(0, "hot_ens_Init:  double initialization");
	}

	ch_context = hot_mem_AddChannel("context");
	ch_endpt_array = hot_mem_AddChannel("endpt_array");
	ch_bool_array = hot_mem_AddChannel("bool_array");
	ch_read_buffer = hot_mem_AddChannel("read_buffer");
	ch_read_message = hot_mem_AddChannel("read_message");

	g.initialized = 1;
	g.g_alive = NULL ;
	
	err = hot_lck_Create(&g.w_mutex);
	if (err != HOT_OK)  {
	    end_critical();
	    return err;
	}

	err = hot_lck_Create(&g.r_mutex);
	if (err != HOT_OK)  {
	    end_critical();
	    return err;
	}

	g.io = hot_io_get(outb_name);
	if (g.io == NULL) {
	    end_critical();
	    trace("hot_ens_Init: unknown outboard configuration (%s)", 
		  outb_name);
	    return hot_err_Create(0,
		  "hot_ens_Init: unknown outboard configuration");
	}

	/* Start Ensemble in an outboard mode.
	 */ 
	err = (*g.io->init)(0, argv, &g.fd);
	if (err != HOT_OK)  {
	    end_critical();
	    return err;
	}

	/* Start the callback-processing thread.
	 */
	err = hot_thread_Create(hot_mainloop, (void*) argv, NULL);
	if (err != HOT_OK) {
	    end_critical();
	    return err;
	}
    } end_critical();
    
    return HOT_OK;
}

/****************************** Context management ****************************/

/* Allocate a new context.
 * The global data record must be locked at this point.
 */
static 
hot_context_t alloc_context(void) {
    static unsigned id = 0 ;
    hot_context_t context ;
    
    context = hot_mem_Alloc(ch_context,sizeof(*context)) ;
    
    context->id = id++ ;
    context->next = g.g_alive ;
    g.g_alive = context ;
    
    return context ;
}

/* Release a context descriptor.
 * The g data record must be locked at this point.
 */
static 
void release_context(hot_context_t s) {
    hot_context_t *tmpp, tmp ;

    assert(s) ;
    
    /* Strip this member from list of live members.
     */
    for(tmpp=&g.g_alive;*tmpp;tmpp=&(*tmpp)->next)
	if (*tmpp == s) {
	    *tmpp = (*tmpp)->next ;
	    break ;
	}
    
    /* Check to make sure that it is no longer around.
     */
    for (tmp = g.g_alive;tmp; tmp = tmp->next)
	if (tmp == s)
	    hot_sys_Panic("release_context:internal inconsistency") ;
    
    hot_mem_Free(s) ;
}

/* Lookup a context descriptor.
 * The g data record must be locked at this point.
 *
 * PERF BUG:  Should use a hash table instead of list search.
 */
static 
hot_context_t lookup_context(hot_contextID_t id) {
    hot_context_t s;

    for (s = g.g_alive; s; s = s->next)
	if (s->id == id) break;
    if (!s) {
	fprintf(stderr, "HOT_C: id = %ld", id);
	for (s = g.g_alive; s; s = s->next)
	    fprintf(stderr, " alive.id = %ld", s->id);
	hot_sys_Panic("lookup_context: id not found");
    }
    return s;
}

/************************ User Downcalls ****************************/

/* Initialize/reset an options struct.
 */
void hot_ens_InitJoinOps(hot_ens_JoinOps_t *ops) {
    if (ops == NULL)
	hot_sys_Panic("hot_ens_InitJoinOps: bad argument (NULL)");
    
    memset(ops, 0, sizeof(hot_ens_JoinOps_t));
    ops->heartbeat_rate = HOT_ENS_DEFAULT_HEARTBEAT_RATE;
    strcpy(ops->transports, HOT_ENS_DEFAULT_TRANSPORT);
    strcpy(ops->group_name, HOT_ENS_DEFAULT_GROUP_NAME);
    strcpy(ops->protocol, HOT_ENS_DEFAULT_PROTOCOL);
    strcpy(ops->properties, HOT_ENS_DEFAULT_PROPERTIES);
    ops->use_properties = 1;
    strcpy(ops->params, "");
    ops->groupd = 0;
    ops->argv = NULL ;
    ops->env = NULL ;
    ops->endpt.name[0] = 0x0;
    strcpy(ops->princ, "");
    strcpy(ops->key, "");
    ops->secure = 0;
}

/* Join a group.  The group context is returned in *contextp.  
 */
hot_err_t hot_ens_Join(
    hot_ens_JoinOps_t *jops,
    hot_context_t *contextp	/*OUT*/ 
) {
    hot_err_t err = HOT_OK ;
    hot_context_t s ;

    /* Initialize global state if not done so already.
     */
    if (!g.initialized) {
	err = hot_ens_Init(jops->outboard, jops->argv); 
	if (err != HOT_OK)
	    return err;
    }

    begin_write(); {    
	begin_critical(); {    
	    /* Allocate a new group context 
	     * Initialize the group record.
	     */
	    s = alloc_context();
	    s->joining = 1 ;
	    s->leaving = 0 ;
	    s->conf = jops->conf;
	    s->env = jops->env;
	    s->view = NULL ;
	    *contextp = s ;
	} end_critical();
	
	/* Write the downcall.
	 */
  	write_hdr(s,DN_JOIN);
	write_int(jops->heartbeat_rate);
	write_string(jops->transports);
	write_string(jops->protocol);
	write_string(jops->group_name);
	write_string(jops->properties);
	write_bool(jops->use_properties);
	write_bool(jops->groupd);
	write_string(jops->params);
	write_bool(jops->client);
	write_bool(jops->debug);

	if (jops->endpt.name[0] != 0x0) {
	    hot_sys_Warning("HOT_OUTBOARD does not support 'endpt' in join ops") ;
	    jops->endpt.name[0] = 0x0;
	}
	write_string(jops->princ);
	write_string(jops->key);
	write_bool(jops->secure);

    } end_write();

    return HOT_OK;
}

/* Leave a group.  This should be the last call made to a given context.
 * No more events will be delivered to this context after the call
 * returns.  
 */
hot_err_t hot_ens_Leave(
    hot_context_t s
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_Leave:this member is already leaving") ;
	    }
	    s->leaving = 1 ;
	} end_critical();
	    
	/* Write the downcall.
	 */
	write_hdr(s,DN_LEAVE);
    } end_write();
    
    return HOT_OK;
}

/* Send a multicast message to the group.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_ens_Cast
#endif
HOT_INLINE
hot_err_t hot_ens_Cast(
    hot_context_t s,
    hot_msg_t msg, 
    hot_ens_MsgSendView *send_view	/*OUT*/
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_Cast: member is leaving") ;
	    }

	    if (send_view != NULL) {
		*send_view = (s->blocked) ?
		    HOT_ENS_MSG_SEND_NEXT_VIEW : HOT_ENS_MSG_SEND_CURRENT_VIEW ;
	    }  
	} end_critical() ;
	
	/* Write the downcall.
	 */
	write_hdr(s,DN_CAST);
	write_msg(msg);
    } end_write();
    
    return HOT_OK ;
}

/* Send a point-to-point message to the specified group member.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_ens_Send
#endif
HOT_INLINE
hot_err_t hot_ens_Send(
    hot_context_t s, 
    hot_endpt_t *dest,
    hot_msg_t msg,
    hot_ens_MsgSendView *send_view /*OUT*/
) {
    begin_write(); { 
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_Send: member is leaving") ;
	    }

	    if (send_view != NULL) {
		*send_view = (s->blocked) ?
		    HOT_ENS_MSG_SEND_NEXT_VIEW : HOT_ENS_MSG_SEND_CURRENT_VIEW ;
	    }
	} end_critical() ;

	if (s->blocked) {
	    write_hdr(s,DN_SEND_BLOCKED);
	    write_endpID(dest);
	    write_msg(msg);
	} else {
 	    unsigned i ;
	    begin_critical(); {
		for (i=0;i<s->nmembers;i++) {
		    if (!strcmp(s->view[i].name,dest->name))
			break ;
		}
		assert(i<s->nmembers) ;
	    } end_critical() ;
	    write_hdr(s,DN_SEND);
	    write_int(i);
	    write_msg(msg);
	}
    } end_write();
    return HOT_OK;
}

/* Report group members as failure-suspected.
 * 
 * NB:  In the initial implementation, this downcall will not be supported.
 *      (if invoked, an exeption will be raised by OCAML).
 */
hot_err_t hot_ens_Suspect(
    hot_context_t s,
    hot_endpt_t *suspects, 
    int nsuspects
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_Suspect: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_SUSPECT);
	write_endpList(nsuspects, suspects);
    } end_write();
    return HOT_OK;
}

/* Inform Ensemble that the state-transfer is complete. 
 */
hot_err_t hot_ens_XferDone(
    hot_context_t s
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_XferDone: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_XFERDONE);
    } end_write();
    return HOT_OK;
}



/* Request a protocol change.
 */
hot_err_t hot_ens_ChangeProtocol(
    hot_context_t s,
    char *protocol
) {
    assert(protocol);
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_ChangeProtocol: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_PROTOCOL);
	write_string(protocol); 
    } end_write();
    return HOT_OK;
}

/* Request a protocol change specifying properties.
 */
hot_err_t hot_ens_ChangeProperties(
    hot_context_t s,
    char *properties
) {
    assert(properties) ;
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_ChangeProperties: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_PROPERTIES);
	write_string(properties); 
    } end_write();
    return HOT_OK;
}

/* Request a new view to be installed.
 */
hot_err_t hot_ens_RequestNewView(
    hot_context_t s
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_RequestNewView: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_PROMPT);
    } end_write();
    return HOT_OK;
}


/* Request a Rekey operation.
 */
hot_err_t hot_ens_Rekey(
    hot_context_t s
) {
    begin_write(); {
	begin_critical(); {
	    if (s->leaving) {
		hot_sys_Panic("hot_ens_Rekey: member is leaving") ;
	    }
	} end_critical();
	write_hdr(s,DN_REKEY);
    } end_write();
    return HOT_OK;
}

/*****************************************************************************/
/* These are both no-ops for the outboard mode.
 */

hot_err_t hot_ens_MLPrintOverride(
	void (*handler)(char *msg)
) {
    return HOT_OK ;
}

hot_err_t hot_ens_MLUncaughtException(
	void (*handler)(char *info)
) {
    return HOT_OK ;
}

/*****************************************************************************/

#ifdef WIN32
void
Perror(char *s) {
  char error[256];
  int errno;
  
  errno = GetLastError();
  (void) FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM ,
		       NULL,	// message format
		       errno,
		       0,		// language
		       error,
		       256,		// buffer size, ignored for ALLOC_BUF
		       NULL	// args for msg format
		       );
  fprintf(stderr, "%s: (%d) %s\n", s, errno, error);
}

#endif
