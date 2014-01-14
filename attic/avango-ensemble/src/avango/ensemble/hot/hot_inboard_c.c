/*
 * Contents: C side of the HOT C interface to Ensemble.
 *
 * Author:  Alexey Vaysburd, Dec. 1996.
 */ 

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef _WIN32
void Perror(char *s);
#include <winsock2.h>
#else
#include <sys/time.h>
#define Perror(s) perror(s)
#include <sys/socket.h>
#include <unistd.h>
#endif
#include "caml/mlvalues.h"
#include "caml/memory.h"
#include "caml/callback.h"
#include "caml/alloc.h"
extern mlsize_t string_length(value) ;

#include "hot_sys.h"
#include "hot_error.h"
#include "hot_thread.h"
#include "hot_msg.h"
#include "hot_mem.h"
#include "hot_ens.h"

/* Include the memory-management file from directory socket.
 * 
 * BUG: mm.h should be copied outside. 
 */
#include "mm.h"
#include "sockfd.h"

#ifdef INLINE_PRAGMA
#pragma inline begin_critical
#endif 
MSG_INLINE
static void trace(const char *s, ...) {
  //#if 0
  va_list args;
  static int debugging = -1 ;

  va_start(args, s);

  if (debugging == -1) {
      debugging = (getenv("ENS_HOT_TRACE") != NULL) ;
  }
  
  if (!debugging) return ;

  fprintf(stderr, "HOT_INBOARD(C):");
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  //#endif
}

#if 0
static
void timer(char *msg) {
    static struct timeval last ;
    struct timeval tv ;
    int usec ;
    gettimeofday(&tv, NULL) ;
    usec = (tv.tv_usec - last.tv_usec) + (tv.tv_sec - last.tv_sec) * 1000000 ;
    fprintf(stderr, "HOT_INBOARD:%08d usec:%s\n", usec, msg) ;
    last = tv ;
}
#endif

#if 0
static
void select_calls(void) {
    static int c ;
    c++;
    if ((c % 100)==0)
	fprintf(stderr, "HOT_INBOARD:#select_calls=%d\n", c) ;
}
#endif

/*************************** Memory channels *******************************/

static hot_mem_t ch_gctx, 
  ch_endpt_array, 
  ch_bool_array, 
  ch_dncall,
  ch_joinops,
  ch_protocol;

/****************************************************************************/

value hot_ml_alloc(const char *debug, mlsize_t s, tag_t t) {
#if 0
  fprintf(stderr, "hot_ml_alloc:%s:%d:%d\n", debug, (int)s, (int)t) ;
#endif
  assert(s >= 0) ;
  if (s == 0) {
    return Atom(0) ;
  } else if (s < Max_young_wosize) {
    return alloc(s,t) ;
  } else {
    return alloc_shr(s,t) ;
  }
}  

value hot_ml_alloc_tuple(const char *debug, mlsize_t s) {
  return hot_ml_alloc(debug,s,0) ;
}

value hot_ml_copy_string(const char *s) {
  int len;
  value res;
  
  len = strlen(s);
  res = alloc_string(len);
  //  bcopy(s, String_val(res), len);
  memcpy(String_val(res), s, len);
  return res;
}

/****************************************************************************/

/* Downcall types. The ordering must match ML dncall_t type definition.
 */
typedef enum {
  DNCALL_JOIN,
  DNCALL_CAST,
  DNCALL_SEND,
  DNCALL_SUSPECT,
  DNCALL_XFERDONE,
  DNCALL_PROTOCOL,
  DNCALL_PROPERTIES,
  DNCALL_LEAVE,
  DNCALL_PROMPT,
  DNCALL_REKEY,
  DNCALL_VOID
} dncall_type ;

/****************************************************************************/

/* This describes a downcall request issued by the application.
 * Not all fields are used by all downcall types. 
 */
typedef struct dncall {
  struct dncall *next ;
  hot_gctx_t gctx ;
  dncall_type type ;
  hot_ens_JoinOps_t *jops ;	
  hot_msg_t msg ;		/* NB:  Store here an alias of the dncall msg */
  hot_endpt_t dest ;
  hot_endpt_t *suspects ;
  unsigned nsuspects ;
  char *protocol ;
} dncall_t;

/****************************************************************************/

/* This describes a group context.
 */
struct hot_gctx {
  unsigned int id ;		/* unique integer identifier */
  hot_gctx_t next;		/* linked list of alive members */
  hot_ens_cbacks_t conf;	/* application callbacks */
  void *env;			/* application state */
  int group_blocked;		/* is group blocked? */
  int joining ;			/* are we joining? */
  int leaving ;			/* are we leaving? */
} ;

/****************************************************************************/

/* Global state.
 */
static struct {
  hot_lock_t mutex;		/* global mutex lock  */
  int in_critical;		/* Set when mutex is locked */
  int initialized;		/* Set when hot_c is initialized. */
  int started;			/* Set when Ensemble/OCAML is started */
  
  dncall_t *dn_free ;		/* free list of dncalls */
  dncall_t *dn_tail ;		/* last pending dncall */
  
  hot_gctx_t g_alive ;		/* allocated group contexts */
} g ;

/****************************************************************************/

value hot_ens_thread_usleep(value usecs_v) { /* ML */
  int usecs ;
  usecs = Int_val(usecs_v) ;
  hot_thread_Usleep(usecs) ;
  return Val_unit ;
}

/*****************************************************************************/

static void (*MLPrinter)(char *) = NULL ;

hot_err_t hot_ens_MLPrintOverride(
	void (*handler)(char *msg)
) {
    MLPrinter = handler ;
    return HOT_OK ;
}

value hot_ens_MLDoPrint(value msg_v) {
    if (MLPrinter != NULL) {
	MLPrinter(String_val(msg_v)) ;
	return Val_true ;
    }
    return Val_false ;
}

static void (*MLHandler)(char *) = NULL ;

hot_err_t hot_ens_MLUncaughtException(
	void (*handler)(char *info)
) {
    MLHandler = handler ;
    return HOT_OK ;
}

value hot_ens_MLHandleException(value msg_v) {
    if (MLHandler != NULL) {
	MLHandler(String_val(msg_v)) ;
	exit(1) ;
    }
    return Val_false ;
}

/****************************************************************************/
/* BUG: This stuff should be rolled into the global state.
 */

#ifdef THREADED_SELECT
/* This is set when the main Ensemble thread is blocked
 * waiting for socket input or new actions.
 */
static int sock_blocked = 0 ;

/* This is the number of times the Ensemble thread has
 * been signalled by the occurrence of new actions.
 */
static int sock_signalled = 0 ;

/* Mutex for blocking operations' state.
 */
static hot_lock_t blocking_lock ;

/* Semaphore for controlling the selector thread.
 */
static hot_sema_t selector_sema ;

/* Semaphore for controlling the Ensemble thread.
 */
static hot_sema_t ml_sema ;

/* This is the information the selector uses
 * for calling the blocking select.
 */
#define NBLOCKING_READ_FDS 100	/* HACK */
static int blocking_read_fds[NBLOCKING_READ_FDS] ;
static int nblocking_read_fds ;
static struct timeval *blocking_timeout, bt_space ;

static void selector_thread(void *ignore) {
  int signalled ;
  struct timeval timeval, *tp ;
  int read_fds[NBLOCKING_READ_FDS] ;
  int nread_fds ;
  fd_set read_fdset ;
  int i, max_fd, ret ;

  /* Loop forever.
   */
  while (1) {
    /* Block until the ML thread requests me to wait for data
     * on some descriptors.
     */
    hot_sema_Dec(selector_sema) ; /* Matched in ml_block */

    while (1) {
      hot_lck_Lock(blocking_lock) ; {
	signalled = (sock_signalled > 0) ;
	if (signalled)
	  sock_signalled -- ;
	
	/* Get the fd information.
	 */
	nread_fds = nblocking_read_fds ; 
	for(i=0; i<nread_fds; i++)
	  read_fds[i] = blocking_read_fds[i] ;

	/* Get the timeout information.
	 */
	if (blocking_timeout) {
	  timeval = *blocking_timeout ;
	  tp = &timeval ;
	} else {
	  tp = NULL ;
	}
	
      } hot_lck_Unlock(blocking_lock) ;
	
      /* If we were signalled, then block and loop around again.  
       */
      if (signalled) {
	hot_sema_Dec(selector_sema) ; /* Matched in notify_new_actions */
	continue ;
      }
      
      /* Initialize the fd_set.
       */
      FD_ZERO(&read_fdset) ;
      max_fd = 0 ;
      for(i=0;i<nread_fds;i++) {
	FD_SET(read_fds[i],&read_fdset) ;
	max_fd = (max_fd > read_fds[i]) ? max_fd : read_fds[i] ;
      }

#if 0
      fprintf(stderr,"Calling select:%d fds ([%d,%d,...])\n",nread_fds,read_fds[0],read_fds[1]) ;
      if (tp) 
	fprintf(stderr,"  timeout=%f\n", (tp->tv_sec + (((float)tp->tv_usec) / 1E6))) ;
#endif

      /*timer("pre-select") ;*/
      ret = select(max_fd+1, &read_fdset, NULL, NULL, tp) ;      
      /*timer("post-select") ;*/

      /*select_calls();*/

#if 0
      fprintf(stderr,"Select returned:%d\n", ret) ;
#endif

      /* Data is ready or the timeout went off.  Wake up the ML
       * thread.  Break out of the loop and wait for next request.
       */
      if (ret >= 0) {
	break ;
      }

      /* Ignore errors...
       */
      if (ret == -1) {
	perror("HOT_C:select") ;
      }
    }
      
    hot_sema_Inc(ml_sema) ;	/* Matched in ml_block */
  }
}

/* Called by ML thread to block the thread until there
 * is data available on our sockets.
 */
static void ml_block(void) {
  static value *hot_timeout_ref_v ;
  static value *hot_socket_arr_ref_v ;

  /* Initialize these values if not done so already.
   */
  if (!hot_timeout_ref_v) {
    hot_timeout_ref_v    = caml_named_value((char*)"hot_timeout") ;
    hot_socket_arr_ref_v = caml_named_value((char*)"hot_sockets") ;
  }
  assert(hot_timeout_ref_v) ;
  assert(hot_socket_arr_ref_v) ;

  /* Setup shared data.
   */
  hot_lck_Lock(blocking_lock) ; {
    /* Mark me as being blocked.
     */
    sock_blocked = 1 ;
    
    { /* Get the timeout information.
       */
      value timeout_v = Field(*hot_timeout_ref_v,0) ;
      bt_space.tv_sec  = Long_val(Field(timeout_v,0)) ;
      bt_space.tv_usec = Long_val(Field(timeout_v,1)) ;
      if (bt_space.tv_sec < 0 || bt_space.tv_usec < 0)
	blocking_timeout = NULL ;
      else
	blocking_timeout = &bt_space ;
    }
    
    { /* Get the socket information.
       */
      value socket ;
      value socks_arr ;
      int i ;

      /* Dereference the ref cell.
       */
      socks_arr = Field(*hot_socket_arr_ref_v,0) ;

      /* Get the # of fds.
       */
      nblocking_read_fds = Wosize_val(socks_arr) ;
      assert(nblocking_read_fds <= NBLOCKING_READ_FDS) ;

      /* Copy the ML data to our C array, mapping the
       * sockets into fds/handles.
       */
      for (i=0;i<nblocking_read_fds;i++) {
	/* Prototype socket_val().  This probably won't work
	 * as-is on NT.  This is taken from socket/skt.h.
         */
	socket = Field(socks_arr,i) ;
	blocking_read_fds[i] = Socket_val(socket) ;
      }
    }    

  } hot_lck_Unlock(blocking_lock) ;
  
  /* Wake up selector thread.
   */
  hot_sema_Inc(selector_sema) ;	/* Matched in selector_thread */

  /* Put ML thread to sleep.
   */
  /*timer("sleeping...") ;*/
  hot_sema_Dec(ml_sema) ;	/* Matched in selector_thread */

  /* When I wake up, clear the blocked marker.
   */
  hot_lck_Lock(blocking_lock) ;
  sock_blocked = 0 ;
  hot_lck_Unlock(blocking_lock) ;

}

/* Called when actions are generated.  It will wake up the ML thread
 * if it was blocked waiting for things to do.
 */
static void notify_newactions(void) {
  hot_lck_Lock(blocking_lock) ;
  if (sock_blocked) {
    hot_sema_Inc(ml_sema) ;
    sock_signalled ++ ;
  }
  hot_lck_Unlock(blocking_lock) ;
}
#endif /* THREADED_SELECT */

/****************************************************************************/
#ifdef INLINE_PRAGMA
#pragma inline begin_critical
#endif 
MSG_INLINE
static void begin_critical(void) {
  hot_err_Check(hot_lck_Lock(g.mutex)) ;
  assert(!g.in_critical) ;
  g.in_critical = 1 ;
}

#ifdef INLINE_PRAGMA
#pragma inline end_critical
#endif 
MSG_INLINE
static void end_critical(void) {
  assert(g.in_critical) ;
  g.in_critical = 0 ;
  hot_err_Check(hot_lck_Unlock(g.mutex)) ;
}

/****************************************************************************/

static const char *hot_ens_argv_null[2] = {"(?)", NULL};

/* Start OCAML.  The function does not return.
 */
static void ens_do_init(void *param) {
  static int called_already = 0 ;
  value *hot_poll_v ;
  value *hot_block_v ;
  value ret_v ;

  if (called_already)
    hot_sys_Panic("caml_startup:called twice") ;
  called_already = 1 ;

  caml_startup((char**) param ? param : hot_ens_argv_null) ;
  /*fprintf(stderr, "HOT_C:caml_startup returned\n") ;*/

  hot_poll_v = caml_named_value((char*)"hot_poll") ;
  hot_block_v = caml_named_value((char*)"hot_block") ;
  
  while(1) {
    /* Call the Ensemble polling function.
     */
    ret_v = callback(*hot_poll_v,Val_unit) ;

    /* If there is nothing left to be done then
     * block.
     */
    if (!Bool_val(ret_v)) {
#ifdef THREADED_SELECT
      /* If the threaded select() is available then use the
       * call the block() function in ML to get the next
       * alarm timeout set.  Then call the ml_block() function
       * to block on our sockets.
       */
      callback(*hot_block_v,Val_unit) ;
      ml_block() ;
#else
      /* Otherwise, we just sleep for some period of time.
       */
      /*callback(*hot_block_v,Val_unit) ;*/	/* Unnecessary... */
      hot_thread_Usleep(100000 /* 100ms */ ) ;
#endif
    } else {
      /* If there is more stuff to do, it is often a good idea
       * to yield at this point, anyway.
       */
      hot_thread_Yield() ;
    }

    /*fprintf(stderr, "HOT_C:looping (%i)\n", Bool_val(ret_v)) ;*/
  }
}

/* Initialize the interface. Start Ensemble/OCAML if necessary.
 */ 
static hot_err_t hot_ens_Init(char **argv) {
    hot_err_t err;
    hot_thread_attr_t attr;

#ifdef _WIN32_PRECOMPILED
    if (init_ops != HOT_ENS_INIT_SEPARATE_THREAD) {
    	fprintf(stderr, "HOT_INBOARD:This pre-compiled WIN32 version of HOT only supports the SEPARATE_THREAD options\n");
    	return hot_err_Create(0, "hot_ens_Init:  unsupported init option");;
    }
#endif
    
    /* Initialize C side of the interface (preallocate/initialize gctx entries,
     * dncall structures, mutex.
     */
    err = hot_lck_Create(&g.mutex) ;
    if (err != HOT_OK) hot_sys_Panic(hot_err_ErrString(err));
    
#ifdef THREADED_SELECT
    /* These could also be #ifdef'd away like the thread create.
     */
    err = hot_sema_Create(0,&selector_sema) ;
    if (err != HOT_OK) hot_sys_Panic(hot_err_ErrString(err)) ;
    err = hot_sema_Create(0,&ml_sema) ;
    if (err != HOT_OK) hot_sys_Panic(hot_err_ErrString(err)) ;
    err = hot_lck_Create(&blocking_lock) ;
    if (err != HOT_OK) hot_sys_Panic(hot_err_ErrString(err)) ;
    err = hot_thread_Create(selector_thread,NULL,NULL) ;
    if (err != HOT_OK) hot_sys_Panic(hot_err_ErrString(err)) ;
#endif
    
    g.dn_free = NULL ;
    g.dn_tail = NULL ;
    g.g_alive = NULL ;
    g.initialized = 1;
    
    /* Start Ensemble/OCAML.  Don't allow starting Ensemble more than once.
     */    
    begin_critical();
    if (g.started) {
	end_critical();
	return hot_err_Create(0, "hot_ens_Init:  Ensemble already started");
    }

    ch_gctx = hot_mem_AddChannel("gctx");
    ch_endpt_array = hot_mem_AddChannel("endpt_array");
    ch_bool_array = hot_mem_AddChannel("bool_array");
    ch_dncall = hot_mem_AddChannel("dncall");
    ch_joinops = hot_mem_AddChannel("join_ops");
    ch_protocol = hot_mem_AddChannel("protocol");

    g.started = 1;
    end_critical();
    
    /* MH: BUG: move into critical stuff above? */
    attr.stacksize = 1024 * 1024; /* stack size for Ensemble thread = 1M */
    
    if ((err = hot_thread_Create(ens_do_init, (void*) argv, &attr)) != HOT_OK)
	return err;
    
    return HOT_OK;
}

/****************************************************************************/

static void release_endpt_array(
        hot_mem_t *ch,
        hot_endpt_t *endpts,
	unsigned nendpts
) {
  assert(endpts) ;
  assert(nendpts > 0) ;

  hot_mem_Free(endpts) ;
}

static void release_bool_array(
        hot_mem_t *ch,
        hot_bool_t *bools,
	unsigned nbools
) {
  assert(bools) ;
  assert(nbools > 0) ;

  hot_mem_Free(bools) ;
}

/****************************************************************************/

/* Allocate a new gctx.
 * The global data record must be locked at this point.
 */
static 
hot_gctx_t alloc_gctx(void) {
  static unsigned int id = 0 ;
  hot_gctx_t gctx ;

  
  gctx = hot_mem_Alloc(ch_gctx,sizeof(*gctx)) ;

  gctx->id = id++ ;
  gctx->next = g.g_alive ;
  g.g_alive = gctx ;
  
  return gctx ;
}

/* Release a gctx descriptor.
 * The g data record must be locked at this point.
 */
static 
void release_gctx(hot_gctx_t gc) {
  hot_gctx_t *tmpp, tmp ;

  assert(gc) ;

  /* Strip this member from list of live members.
   */
  for(tmpp=&g.g_alive;*tmpp;tmpp=&(*tmpp)->next)
    if (*tmpp == gc) {
      *tmpp = (*tmpp)->next ;
      break ;
    }

  /* Check to make sure that it is no longer around.
   */
  for (tmp = g.g_alive;tmp; tmp = tmp->next)
    if (tmp == gc)
      hot_sys_Panic("HOT:release_gctx:internal inconsistency") ;

  hot_mem_Free(gc) ;
}

/* Return the index of a newly allocated downcall struct.
 * The global data record must be locked at this point.
 */
static 
dncall_t *alloc_dn(
        hot_gctx_t gctx,
	dncall_type type
) {
  dncall_t *d ;

#ifdef THREADED_SELECT
  /* If the ML thread is blocked, wake it up.
   */
  (void)notify_newactions() ;
#endif
  
  /* Allocate more gctx structs if we are out of them.
   */
  if (g.dn_free) {
    d = g.dn_free ;
    g.dn_free = d->next ;
    assert(d->type == DNCALL_VOID) ;
  } else {
    d = (dncall_t*) hot_mem_Alloc(ch_dncall,sizeof(*d)) ;
  }
  memset(d,0,sizeof(*d)) ;	/* BUG */

  /* Insert in pending queue.
   */
  d->next = g.dn_tail ;
  g.dn_tail = d ;

  /* Initialize some fields.
   */
  d->gctx = gctx ;
  d->type = type ;

  return d ;
}

/* Release a downcall struct.
 * The global data record must be locked at this point.
 */
static void release_dn(dncall_t *d) {
  assert(d) ;
  assert(!d->next) ;
  assert(!d->jops) ;
  assert(!d->suspects) ;
  assert(!d->protocol) ;

  memset(d,0,sizeof(*d)) ;
  d->next = g.dn_free ;
  d->type = DNCALL_VOID ;
  g.dn_free = d ;
}

/*###################### C <-> ML Conversion Routines ###############*/

/* Extract an endpoint structure from a value.
 */
static void Endpt_val(
        value endp_v,
	hot_endpt_t *endp	/*OUT*/ 
) {
  strcpy(endp->name, String_val(endp_v));
}
    
/* Return a value contaiing an endpoint.
 */
static value Val_endpt(
        hot_endpt_t *endp
) {
  return hot_ml_copy_string(endp->name);
}

/* Enum types specifying positions of fields in ML type definitions:
 */

/* ML type definition:
 *
 * type ens_join_options_t = {
 *   heartbeat_rate:	float;
 *   transports:       	string;
 *   protocol: 		string;
 *   group_name:       	string;
 *   properties:        string;
 *   use_properties:    bool;
 *   groupd:            bool;
 *   params:            string;
 *   princ:             string;
 *   key:               string;
 *   debug:             bool;
 * }
 */
enum join_ops_enum {
    JOPS_HEARTBEAT_RATE,
    JOPS_TRANSPORTS,
    JOPS_PROTOCOL,
    JOPS_GROUP_NAME,
    JOPS_PROPERTIES,
    JOPS_USE_PROPERTIES,
    JOPS_GROUPD,
    JOPS_PARAMS,
    JOPS_CLIENT,
    JOPS_DEBUG,
    JOPS_ENDPT,

    JOPS_PRINC,
    JOPS_KEY,
    JOPS_SECURE,
    
    JOPS_NFIELDS
};

#define JOPS_NROOTS (JOPS_NFIELDS + 1)
#define JOPS_V JOPS_NFIELDS

/* Convert a join-options struct into a value.
 */
static value Val_of_joinOps(hot_ens_JoinOps_t *ops) {
    CAMLparam0();
    CAMLlocalN(rt, JOPS_NROOTS);
    memset(rt, 0, sizeof(rt)) ;

    rt[JOPS_HEARTBEAT_RATE] = Val_long(ops->heartbeat_rate);
    rt[JOPS_TRANSPORTS] = hot_ml_copy_string(ops->transports);
    rt[JOPS_PROTOCOL] = hot_ml_copy_string(ops->use_properties ? "" : ops->protocol);
    rt[JOPS_GROUP_NAME] = hot_ml_copy_string(ops->group_name);
    rt[JOPS_PROPERTIES] = hot_ml_copy_string(ops->use_properties ? ops->properties : "");
    rt[JOPS_USE_PROPERTIES] = Val_bool(ops->use_properties);
    rt[JOPS_GROUPD] = Val_bool(ops->groupd);
    rt[JOPS_PARAMS] = hot_ml_copy_string(ops->params);
    rt[JOPS_CLIENT] = Val_bool(ops->client);
    rt[JOPS_DEBUG] = Val_bool(ops->debug);
    rt[JOPS_ENDPT] = hot_ml_copy_string(ops->endpt.name) ;
    rt[JOPS_PRINC] = hot_ml_copy_string(ops->princ);
    rt[JOPS_KEY] = hot_ml_copy_string(ops->key);
    rt[JOPS_SECURE] = Val_bool(ops->secure);

    rt[JOPS_V] = alloc_tuple(JOPS_NFIELDS);

    Field(rt[JOPS_V], JOPS_HEARTBEAT_RATE) = rt[JOPS_HEARTBEAT_RATE];
    Field(rt[JOPS_V], JOPS_TRANSPORTS) = rt[JOPS_TRANSPORTS];
    Field(rt[JOPS_V], JOPS_PROTOCOL) = rt[JOPS_PROTOCOL];
    Field(rt[JOPS_V], JOPS_GROUP_NAME) = rt[JOPS_GROUP_NAME];
    Field(rt[JOPS_V], JOPS_PROPERTIES) = rt[JOPS_PROPERTIES];
    Field(rt[JOPS_V], JOPS_USE_PROPERTIES) = rt[JOPS_USE_PROPERTIES];
    Field(rt[JOPS_V], JOPS_GROUPD) = rt[JOPS_GROUPD];
    Field(rt[JOPS_V], JOPS_PARAMS) = rt[JOPS_PARAMS];
    Field(rt[JOPS_V], JOPS_CLIENT) = rt[JOPS_CLIENT];
    Field(rt[JOPS_V], JOPS_DEBUG) = rt[JOPS_DEBUG] ;
    Field(rt[JOPS_V], JOPS_ENDPT) = rt[JOPS_ENDPT] ;
    Field(rt[JOPS_V], JOPS_PRINC) = rt[JOPS_PRINC];
    Field(rt[JOPS_V], JOPS_KEY)   = rt[JOPS_KEY];
    Field(rt[JOPS_V], JOPS_SECURE) = rt[JOPS_SECURE];

    CAMLreturn(rt[JOPS_V]) ;
}

/* Return a HOT message created from the given substring.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_ens_Msg_of_substring
#endif 
MSG_INLINE
static hot_msg_t hot_ens_Msg_of_substring(
					  value iovec_v
) {
  hot_msg_t msg ;
  int len = Int_val(Field(iovec_v,0));
  char *buf = mm_Cbuf_val(Field(iovec_v,1));
  hot_err_t err;

  msg = hot_msg_Create() ;
  err = hot_msg_Write(msg, buf, len) ;
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  return msg;
}

/* Return a msg value corresponding to the given HOT message.
 *
 * We simply copy the message here, though it should be possible
 * to use zero-coping in this case. 
 */
#ifdef INLINE_PRAGMA
#pragma inline Val_msg
#endif 
MSG_INLINE
static value Val_msg(hot_msg_t msg) {
  value msg_v;
  void *data;
  char *send_data;
  unsigned size;
  hot_err_t err;
    
  err = hot_msg_GetPos(msg, &size);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  err = hot_msg_Look(msg, size, &data);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));

  send_data = malloc(size);
  if (send_data == NULL) {
    printf ("HOT_INBOARD: out of memory\n");
    exit(0);
  }
  memcpy(send_data, (char*)data, size);
  msg_v = mm_Raw_of_len_buf(size,send_data);

  return msg_v;
}

/* Convert a group context into a value.
 */
#ifdef INLINE_PRAGMA
#pragma inline Val_of_gctx
#endif 
MSG_INLINE
static value Val_of_gctx(hot_gctx_t gctx) {
  return Val_int(gctx->id);
}

#ifdef __GNUC__
#define hot_ens_Gctx_of_val(a) hot_ens_Gctx_of_val_debug(a,__FUNCTION__)
#else
#define hot_ens_Gctx_of_val(a) hot_ens_Gctx_of_val_debug(a,"<unknown>")
#endif

/* Convert a value into group context.
 */
#ifdef INLINE_PRAGMA
#pragma inline hot_ens_Gctx_of_val
#endif 
MSG_INLINE
static hot_gctx_t 
hot_ens_Gctx_of_val_debug(value gctx_v, const char *where) {
  unsigned id = Int_val(gctx_v) ;
  hot_gctx_t gc ;
  
  for (gc=g.g_alive; gc; gc=gc->next)
    if (gc->id == id) break ;
  if (!gc) {
    fprintf(stderr, "HOT_INBOARD:id=%d (%s)\n", id, where) ;
    for (gc=g.g_alive; gc; gc=gc->next)
	fprintf(stderr, " alive.id=%d\n",gc->id) ;
    
    hot_sys_Panic("Gctx_of_val:id not found") ;
  }
  return gc ;
}

/* Convert a value into an array of endpoints.  Allocate the array and
 * return the pointer to it in *endp_p.
 */
static void Endpt_array_val(
        value endp_array_v,
	unsigned *n_endp,	/*OUT*/ 
	hot_endpt_t **endp_p	/*OUT*/ 
) {
  unsigned i;
  hot_endpt_t *endp_arr;
  
  assert(n_endp) ;
  assert(endp_p) ;
  *n_endp = Wosize_val(endp_array_v);
  endp_arr = (hot_endpt_t*) hot_mem_Alloc(ch_endpt_array,sizeof(hot_endpt_t) * (*n_endp));
  if (endp_arr == NULL)
    hot_sys_Panic("Endpt_array_val: hot_mem_Alloc failed");
  for (i = 0; i < *n_endp; i++)
    Endpt_val(Field(endp_array_v, i), &endp_arr[i]);
  *endp_p = endp_arr;
}

/* Convert a value into an array of hot_bool_t's.  Allocate the array and
 * return the pointer to it in *bool_p.
 */
static void Bool_array_val(
        value bool_array_v,
	unsigned *n_bool,	/*OUT*/ 
	hot_bool_t **bool_p	/*OUT*/ 
) {
  unsigned i;
  hot_bool_t *bool_arr;
  
  assert(n_bool) ;
  assert(bool_p) ;
  *n_bool = Wosize_val(bool_array_v);
  bool_arr = (hot_bool_t*) hot_mem_Alloc(ch_bool_array,
					 sizeof(hot_bool_t) * (*n_bool));
  if (bool_arr == NULL)
    hot_sys_Panic("Bool_array_val: hot_mem_Alloc failed");
  for (i = 0; i < *n_bool; i++)
    bool_arr[i] = Bool_val(Field(bool_array_v, i));
  *bool_p = bool_arr;
}

/* Convert an array of endpoints into a value.
 */
static value Val_endpt_array(
        unsigned nendp,
	hot_endpt_t *endp
) {
  unsigned i;
  CAMLparam0() ;
  CAMLlocal1(endp_v) ;
  endp_v = alloc_tuple(nendp) ;
  
  /* Initialize the array because we are going to do
   * allocation.
   */
  for (i=0;i<nendp;i++)
    Field(endp_v,i) = Val_unit ;
  
  for (i=0;i<nendp;i++)
    Field(endp_v,i) = Val_endpt(&endp[i]) ;
  
  CAMLreturn(endp_v) ;
}

enum ens_view_state_enum {
    HOT_ENS_VIEW_STATE_VERSION,
    HOT_ENS_VIEW_STATE_GROUP,
    HOT_ENS_VIEW_STATE_VIEW,
    HOT_ENS_VIEW_STATE_RANK,
    HOT_ENS_VIEW_STATE_PROTOCOL,
    HOT_ENS_VIEW_STATE_GROUPD,
    HOT_ENS_VIEW_STATE_VIEW_ID,
    HOT_ENS_VIEW_STATE_PARAMS,
    HOT_ENS_VIEW_STATE_XFER_VIEW,
    HOT_ENS_VIEW_STATE_PRIMARY,
    HOT_ENS_VIEW_STATE_CLIENTS,
    HOT_ENS_VIEW_STATE_KEY
};

/*
 * ML type definitions: 
 *

type ens_view_state = {
  c_version	: string ;
  c_group	: string ;
  c_view	: endpt array ; 
  c_rank	: int ;
  c_protocol    : string ;
  c_groupd	: bool ;
  c_view_id	: Trans.ltime * endpt ;
  c_params	: string ;
  c_xfer_view	: bool ; 
  c_primary	: bool ;
  c_clients     : bool array ;
  c_key         : string 

}

 *
 * C type definitions: 
 *
 * struct hot_view_state {
 *     char version[HOT_ENS_MAX_VERSION_LENGTH];
 *     char group_name[HOT_ENS_MAX_GROUP_NAME_LENGTH];
 *     hot_endpt_t *members;
 *     unsigned nmembers;
 *     unsigned my_rank;
 *     char protocol[HOT_ENS_MAX_PROTO_NAME_LENGTH];
 *     int groupd;
 *     hot_view_id view_id;
 *     char params[HOT_ENS_MAX_PARAMS_LENGTH];
 *     int xfer_view;
 *     int primary;
 *     hot_bool_t *clients;
 *     char key[HOT_ENS_MAX_KEY_LEGNTH];
 * };
 */


static void string_of_val_copy(
	const char *debug,
        value s_v,
	char *s,
	unsigned max_len
) {
  unsigned len ;
  assert(s) ;
  
  len = string_length(s_v) ;
  if (len+1 >= max_len) {
    char msg[1000] ;
    sprintf(msg,"ViewState_of_val: %s string too long (%d,max=%d)", debug, len, max_len) ;
    hot_sys_Panic(msg) ;
  }
  
  assert(strlen(String_val(s_v)) == len) ;

  strcpy(s, String_val(s_v)) ;
}

/* Convert a value into a view state.
 */
static void ViewState_of_val(
        value vs_v, 
	hot_view_state_t *vs	/*OUT*/ 
) {
  unsigned nclients;

  memset(vs,0,sizeof(*vs)) ;
  vs->rank      = Int_val(Field(vs_v, HOT_ENS_VIEW_STATE_RANK));
  vs->groupd    = Int_val(Field(vs_v, HOT_ENS_VIEW_STATE_GROUPD));
  vs->xfer_view = Int_val(Field(vs_v, HOT_ENS_VIEW_STATE_XFER_VIEW));
  vs->primary   = Int_val(Field(vs_v, HOT_ENS_VIEW_STATE_PRIMARY));
  vs->view_id.ltime = Int_val(Field(Field(vs_v, HOT_ENS_VIEW_STATE_VIEW_ID), 0));
  
  string_of_val_copy("view_id.coord",
		     Field(Field(vs_v, HOT_ENS_VIEW_STATE_VIEW_ID), 1),
		     vs->view_id.coord.name,
		     HOT_ENDP_MAX_NAME_SIZE) ;

  string_of_val_copy("version",
		     Field(vs_v, HOT_ENS_VIEW_STATE_VERSION),
		     vs->version,
		     HOT_ENS_MAX_VERSION_LENGTH) ;

  string_of_val_copy("group_name",
		     Field(vs_v, HOT_ENS_VIEW_STATE_GROUP),
		     vs->group_name,
		     HOT_ENS_MAX_GROUP_NAME_LENGTH) ;
  
  string_of_val_copy("protocol",
		     Field(vs_v, HOT_ENS_VIEW_STATE_PROTOCOL),
		     vs->protocol,
		     HOT_ENS_MAX_PROTO_NAME_LENGTH) ;

  string_of_val_copy("params",
		     Field(vs_v, HOT_ENS_VIEW_STATE_PARAMS),
		     vs->params,
		     HOT_ENS_MAX_PARAMS_LENGTH) ;

  Endpt_array_val(Field(vs_v, HOT_ENS_VIEW_STATE_VIEW),
			    &vs->nmembers, &vs->members);

  Bool_array_val(Field(vs_v, HOT_ENS_VIEW_STATE_CLIENTS),
			    &nclients, &vs->clients);
  assert(nclients == vs->nmembers);

  string_of_val_copy("key",
		     Field(vs_v, HOT_ENS_VIEW_STATE_KEY),
		     vs->key,
		     HOT_ENS_MAX_KEY_LEGNTH) ;
}

/* Return a value containing the specified downcall request.
 * The value must match the corresp. ML type definition.
 */
static value Val_of_dn(dncall_t *dn) {
  value pair_v ;
  hot_err_t err;
  
  CAMLparam0() ;
  CAMLlocal3(field0, field1, dn_v) ;

  switch (dn->type) {
  case DNCALL_JOIN:
    field0 = Val_of_joinOps(dn->jops);
    hot_mem_Free(dn->jops) ;
    dn->jops = NULL ;
    
    dn_v = hot_ml_alloc("Val_of_dn:join",1, DNCALL_JOIN);
    Field(dn_v, 0) = field0;
    break;
  case DNCALL_LEAVE:
    dn_v = hot_ml_alloc("Val_of_dn:leave",1, DNCALL_LEAVE);
    Field(dn_v, 0) = Val_unit ;
    break;
  case DNCALL_CAST:
    field0 = Val_msg(dn->msg);
    if ((err = hot_msg_Release(&dn->msg)) != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
    dn_v = hot_ml_alloc("Val_of_dn:cast", 1, DNCALL_CAST);
    Field(dn_v, 0) = field0;
    break;
  case DNCALL_SEND:
    field0 = Val_endpt(&dn->dest);
    field1 = Val_msg(dn->msg);
    if ((err = hot_msg_Release(&dn->msg)) != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
    dn_v = hot_ml_alloc("Val_of_dn:send", 2, DNCALL_SEND);
    Field(dn_v, 0) = field0;
    Field(dn_v, 1) = field1;
    break;
  case DNCALL_SUSPECT:
    field0 = Val_endpt_array(dn->nsuspects, dn->suspects);
    
    release_endpt_array(&ch_endpt_array,dn->suspects,dn->nsuspects) ;
    dn->suspects = NULL ;
    dn->nsuspects = -1 ;

    dn_v = hot_ml_alloc("Val_of_dn:suspect",1, DNCALL_SUSPECT);
    Field(dn_v, 0) = field0;
    break;
  case DNCALL_XFERDONE:
    dn_v = hot_ml_alloc("Val_of_dn:xferdone", 1, DNCALL_XFERDONE);
    Field(dn_v, 0) = Val_unit ;
    break;
  case DNCALL_PROMPT:
    dn_v = hot_ml_alloc("Val_of_dn:prompt", 1, DNCALL_PROMPT);
    Field(dn_v, 0) = Val_unit ;
    break;
  case DNCALL_REKEY:
    dn_v = hot_ml_alloc("Val_of_dn:rekey", 1, DNCALL_REKEY);
    Field(dn_v, 0) = Val_unit ;
    break;
  case DNCALL_PROTOCOL:
    field0 = hot_ml_copy_string(dn->protocol);

    hot_mem_Free(dn->protocol) ;
    dn->protocol = NULL ;

    dn_v = hot_ml_alloc("Val_of_dn:protocol", 1, DNCALL_PROTOCOL);
    Field(dn_v, 0) = field0;
    break;
  case DNCALL_PROPERTIES:
    field0 = hot_ml_copy_string(dn->protocol);

    hot_mem_Free(dn->protocol) ;
    dn->protocol = NULL ;

    dn_v = hot_ml_alloc("Val_of_dn:properties", 1, DNCALL_PROPERTIES);
    Field(dn_v, 0) = field0;
    break;
  default:
    hot_sys_Panic("Val_of_dn: bad downcall type");
    break;
  }
  
  pair_v = hot_ml_alloc_tuple("Val_of_dn:pair", 2);
  Field(pair_v, 0) = Val_of_gctx(dn->gctx) ; /* no alloc */
  Field(pair_v, 1) = dn_v;
  CAMLreturn(pair_v) ;
}

/*######################## User Downcalls ###################################*/

/* Exported interface:  
 */

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
  strcpy(ops->princ, "");
  strcpy(ops->key, "");
  ops->groupd = 0;
  ops->argv = NULL ;
  ops->env = NULL ;
  strcpy(ops->endpt.name, "") ;
}

/* Join a group.  In case of success, the call returns after the
 * first view is delivered.  The group context is returned in *gctxp.  
 */
hot_err_t hot_ens_Join(
        hot_ens_JoinOps_t *jops,
	hot_gctx_t *gctxp	/*OUT*/ 
) {
  hot_err_t err = HOT_OK ;
  hot_gctx_t gc ;
  dncall_t *dn ;

  /* Initialize global state if not done so already.
   */
  if (!g.initialized) {
    err = hot_ens_Init(jops->argv); 
    if (err != HOT_OK)
      return err;
  }
  
  begin_critical();    
  
  /* Allocate a new group context 
   * Initialize the group record.
   */
  gc = alloc_gctx();
  gc->joining = 1 ;
  gc->leaving = 0 ;
  gc->conf = jops->conf;
  gc->env = jops->env;
  
  /* Enqueue the join request.
   */
  dn = alloc_dn(gc,DNCALL_JOIN);

  dn->jops = (hot_ens_JoinOps_t*) hot_mem_Alloc(ch_joinops, sizeof(*dn->jops)) ;
  *dn->jops = *jops ;

  /* We actually set the "return" value here so that
   * it is available when the accepted view call arrives.
   * This is somewhat of a hack.
   */
  *gctxp = gc ;
  
  end_critical();
  return HOT_OK;
}

/* Leave a group.  This should be the last call made to a given gctx.
 * No more events will be delivered to this gctx after the call
 * returns.  
 */
hot_err_t hot_ens_Leave(hot_gctx_t gc) {
  begin_critical();

  if (gc->leaving) {
    hot_sys_Panic("hot_ens_Leave:this member is already leaving") ;
  }

  gc->leaving = 1 ;
  
  /* Enqueue the leave request.
   */
  (void) alloc_dn(gc, DNCALL_LEAVE);
  
  end_critical();

  return HOT_OK;
}

/* Send a multicast message to the group.
 */
hot_err_t hot_ens_Cast(
         hot_gctx_t gc,
	 hot_msg_t msg, 
	 hot_ens_MsgSendView *send_view	/*OUT*/
) {
  dncall_t *dn;
  hot_err_t err;

  begin_critical() ;
  /*timer("hot_ens_Cast") ;*/
  if (send_view != NULL) {
    *send_view = (gc->group_blocked) ?
      HOT_ENS_MSG_SEND_NEXT_VIEW : HOT_ENS_MSG_SEND_CURRENT_VIEW ;
  }
  
  /* Enqueue the cast request.
   */
  dn = alloc_dn(gc,DNCALL_CAST);
  
  err = hot_msg_Alias(msg, &dn->msg);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  end_critical() ;
  return HOT_OK ;
}

/* Send a point-to-point message to the specified group member.
 */
hot_err_t hot_ens_Send(
        hot_gctx_t gc, 
	hot_endpt_t *dest,
	hot_msg_t msg,
	hot_ens_MsgSendView *send_view /*OUT*/
) {
  dncall_t *dn;
  hot_err_t err;

  begin_critical() ;

  if (send_view != NULL) {
    *send_view = (gc->group_blocked) ?
      HOT_ENS_MSG_SEND_NEXT_VIEW : HOT_ENS_MSG_SEND_CURRENT_VIEW ;
  }
  
  /* Enqueue the send request.
   */
  dn = alloc_dn(gc,DNCALL_SEND);
  dn->dest = *dest;
  err = hot_msg_Alias(msg, &dn->msg);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  end_critical() ;
  return HOT_OK ;
}

/* Report group members as failure-suspected.
 * 
 * NB:  In the initial implementation, this downcall will not be supported.
 *      (if invoked, an exeption will be raised by OCAML).
 */
hot_err_t hot_ens_Suspect(
        hot_gctx_t gctx,
	hot_endpt_t *suspects, 
	int nsuspects
) {
  dncall_t *dn;
  int size ;

  begin_critical();    
  
  /* Enqueue the suspect request.
   */
  dn = alloc_dn(gctx,DNCALL_SUSPECT);
  
  size = sizeof(hot_endpt_t) * nsuspects ;
  dn->suspects = (hot_endpt_t*) hot_mem_Alloc(ch_endpt_array,size);
  memcpy(dn->suspects, suspects, size);
  dn->nsuspects = nsuspects;
  
  end_critical();
  return HOT_OK;
}

/* Inform Ensemble that the state-transfer is complete. 
 */
hot_err_t hot_ens_XferDone(
        hot_gctx_t gctx
) {
  dncall_t *dn;
  begin_critical();
    
  /* Enqueue the XferDone request.
   */
  dn = alloc_dn(gctx,DNCALL_XFERDONE);
    
  end_critical();
  return HOT_OK;
}


/* Request a protocol change.
 */
hot_err_t hot_ens_ChangeProtocol(
        hot_gctx_t gctx,
	char *protocol
) {
  dncall_t *dn;

  begin_critical();

  /* Enqueue the ChangeProtocol request.
   */ 
  dn = alloc_dn(gctx, DNCALL_PROTOCOL);

  assert(!dn->protocol) ;
  dn->protocol = hot_mem_Alloc(ch_protocol,HOT_ENS_MAX_PROTO_NAME_LENGTH) ;
  if (strlen(protocol) >= sizeof(dn->protocol))
    hot_sys_Panic("hot_ens_AddChangeProperties: properties too large") ;
  strcpy(dn->protocol, protocol);

  end_critical();
  return HOT_OK;
}

/* Request a protocol change specifying properties.
 */
hot_err_t hot_ens_ChangeProperties(
        hot_gctx_t gctx,
	char *properties
) {
  dncall_t *dn;
  assert(properties) ;

  begin_critical();

  /* Enqueue the ChangeProperties request.
   */ 
  dn = alloc_dn(gctx,DNCALL_PROPERTIES);

  assert(!dn->protocol) ;
  dn->protocol = hot_mem_Alloc(ch_protocol,HOT_ENS_MAX_PROTO_NAME_LENGTH) ;
  if (strlen(properties) >= sizeof(dn->protocol))
    hot_sys_Panic("hot_ens_AddChangeProperties: properties too large") ;
  strcpy(dn->protocol, properties);

  end_critical();
  return HOT_OK;
}

/* Request a new view to be installed.
 */
hot_err_t hot_ens_RequestNewView(
        hot_gctx_t gctx
) {
  dncall_t *dn;
  begin_critical();
    
  /* Enqueue the ChangeProtocol request.
   */
  dn = alloc_dn(gctx,DNCALL_PROMPT);
    
  end_critical();
  return HOT_OK;
}

/* Request a Rekey operation.
 */
hot_err_t hot_ens_Rekey(
        hot_gctx_t gctx
) {
  dncall_t *dn;
  begin_critical();
    
  /* Enqueue the Rekey request.
   */
  dn = alloc_dn(gctx,DNCALL_REKEY);
    
  end_critical();
  return HOT_OK;
}

/*######################### Callback Dispatchers ############################*/


/* Callback dispatchers:
 *
 * Callback dispatchers are invoked from OCAML. The arguments are extracted and 
 * converted into C types, and the user callback function is invoked.
 *
 * NB:  Conventions for messages:  If a message is to be used outside of
 *      a callback, a copy of it (not alias!) must be created.  The
 *      reason is, a message in an upcall is created using the passed
 *      ocaml string, without copying.  After the upcall returns, the
 *      message string can be garbage collected by ocaml.
 * 
 *      For uniformity, the view structure should also not be used
 *      outside of the callback.  The struct. is allocated and released
 *      by the corresp. dispatcher function (hot_ens_AcceptedView_cbd).  
 *      However, the user callback function can steal the array of members by 
 *      setting the value of that field (in the view-state struct) to NULL.
 */ 

/* Got a multicast message. 
 */
value hot_ens_ReceiveCast_cbd(
    value gctx_v,
    value origin_v, 
    value iovec_v
) {
  hot_endpt_t origin;
  hot_msg_t msg;
  hot_err_t err;
  hot_gctx_t gctx = hot_ens_Gctx_of_val(gctx_v);
  void *env;
  hot_ens_ReceiveCast_cback receive_cast;
  
  trace("cast") ;
  begin_critical();
  /*timer("receive cast cbd") ;*/
  env = gctx->env;
  receive_cast = gctx->conf.receive_cast;
  end_critical();
  
  Endpt_val(origin_v, &origin);
  msg = hot_ens_Msg_of_substring(iovec_v) ;
  
  if (receive_cast != NULL) {
    (*receive_cast)(gctx, env, &origin, msg);
  }
  
  err = hot_msg_Release(&msg);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  return Val_unit;
}

/* Got a point-to-point message.
 */
value hot_ens_ReceiveSend_cbd(
        value gctx_v,
	value origin_v, 
	value iovec_v
) {
  hot_endpt_t origin;
  hot_msg_t msg;
  hot_err_t err;
  hot_gctx_t gctx = hot_ens_Gctx_of_val(gctx_v);
  void *env;
  hot_ens_ReceiveSend_cback receive_send;
  
  trace("send") ;
  begin_critical();
  env = gctx->env;
  receive_send = gctx->conf.receive_send;
  end_critical();
  
  Endpt_val(origin_v, &origin);
  msg = hot_ens_Msg_of_substring(iovec_v);
  
  if (receive_send != NULL) {
    (*receive_send)(gctx, env, &origin, msg);
  }
  
  err = hot_msg_Release(&msg);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  return Val_unit;
}

/* Accepted a new view.
 */
value hot_ens_AcceptedView_cbd(	/* ML */
        value gctx_v,
	value view_state_v
) {
  hot_view_state_t view_state;
  hot_gctx_t gc = hot_ens_Gctx_of_val(gctx_v);
  void *env;
  hot_ens_AcceptedView_cback accepted_view;
  
  trace("view") ;

  begin_critical();
  env = gc->env;
  accepted_view = gc->conf.accepted_view;
  
  /* The group is unblocked now.
   */
  gc->group_blocked = 0;
  
  /* Setup the new view.
   */
  ViewState_of_val(view_state_v, &view_state); 

  end_critical();

  if (accepted_view != NULL) {
    (*accepted_view)(gc, env, &view_state);
  }
  
  begin_critical();

  if (view_state.members != NULL) {
    release_endpt_array(&ch_endpt_array, view_state.members, view_state.nmembers);
  }

  if (view_state.clients != NULL) {
    release_bool_array(&ch_bool_array, view_state.clients, view_state.nmembers);
  }

  gc->joining = 0 ;
  
  end_critical();
  
  return Val_unit;
}

/* Got a heartbeat event.
 */
value hot_ens_Heartbeat_cbd(
        value gctx_v,
        value time_v
) {
  hot_gctx_t gctx = hot_ens_Gctx_of_val(gctx_v);
  void *env;
  hot_ens_Heartbeat_cback heartbeat;
  
  trace("heartbeat") ;
  begin_critical();
  env = gctx->env;
  heartbeat = gctx->conf.heartbeat;
  end_critical();
  
  if (heartbeat != NULL) {
    (*heartbeat)(gctx, env, Long_val(time_v));
  }
  
  return Val_unit;
}

/* The group is about to block.
 */
value hot_ens_Block_cbd(value gctx_v) {
  hot_gctx_t gctx = hot_ens_Gctx_of_val(gctx_v);
  void *env;
  hot_ens_Block_cback block;
  
  /* Set the "group_blocked" flag in the corresp. gctx.
   * The flag is cleared when the next view is delivered.
   */
  trace("block") ;
  begin_critical();
  env = gctx->env;
  block = gctx->conf.block;
  gctx->group_blocked = 1;
  end_critical();
  
  if (block != NULL) {
    (*block)(gctx, env);
  }

  return Val_unit;
}

/* The member has left the group.
 */
value hot_ens_Exit_cbd(value gctx_v) {
  hot_gctx_t gc = hot_ens_Gctx_of_val(gctx_v);
  hot_ens_Exit_cback exit_cb;
  void *env;

  /* Phase 1: get info for callback.
   */
  trace("exit") ;
  begin_critical();  
  
  if (!gc->leaving)
    hot_sys_Panic("hot_ens_Exit_cbd: mbr state is not leaving");

  env = gc->env;
  exit_cb = gc->conf.exit;
  
  end_critical();

  if (exit_cb != NULL) {
    (*exit_cb)(gc, env);
  }  
  
  /* Phase 2: cleanup.
   */
  begin_critical();  
  release_gctx(gc);

  end_critical();  
  
  return Val_unit;
}

/************************** C/Ensemble communication ****************************/

/* Return the array of pending downcalls.
 */
value hot_ens_GetDncalls(void) {
  value tmp ;
  int i, n ;
  dncall_t *d, *next ;
  CAMLparam0() ;
  CAMLlocal1(dn_v) ;
  
  begin_critical();

  /* Count the number of downcalls.
   */
  for (n=0, d=g.dn_tail ; d ; n++, d=d->next) ;
  
  dn_v = hot_ml_alloc_tuple("hot_ens_GetDncalls", n) ;

  /* Initialize array because we're going to be allocating
   * soon.
   */
  for (i=0;i<n;i++)
    Field(dn_v,i) = Val_unit ;
  
  for (i = 0, d = g.dn_tail ; 
       d ;
       i++, d = next) {
    assert(i<n) ;
    tmp = Val_of_dn(d) ;
    Field(dn_v,(n-i-1)) = tmp ;
    next = d->next ;
    d->next = NULL ;
    release_dn(d) ;
  }
  assert(i==n) ;
  g.dn_tail = NULL ;
  
  end_critical();
  CAMLreturn(dn_v) ;
}

#ifdef WIN32

void
Perror(char *s) {
  char error[256];
  int errno;
  
  errno = GetLastError();
  (void) FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM ,
		       NULL,	// message format
		       errno,
		       0,	// language
		       error,
		       256,	// buffer size, ignored for ALLOC_BUF
		       NULL	// args for msg format
		       );
  printf("%s: (%d) %s\n", s, errno, error);
  //LocalFree(error);
}

#endif
