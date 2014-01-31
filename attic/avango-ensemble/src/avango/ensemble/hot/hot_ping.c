/* 
 * Test of Ensemble with the HOT C interface.
 *
 * Author:  Alexey Vaysburd, Dec. 96.
 *
 * NB: Access to the global struct is not protected with a mutex
 * (that could cause a deadlock). 
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include "hot_sys.h"
#include "hot_error.h"
#include "hot_msg.h"
#include "hot_ens.h"
#include "hot_thread.h"

#define HOT_PING_MAGIC 234324

struct {
  int ncasts ;
  int nsends ;
  int nviews ;
  int nexits ;
  int nblocks ;
  int nheartbeats ;
  int njoins ;
} stats ;

typedef struct {
  int magic ;
  hot_gctx_t gctx;
  hot_lock_t mutex;
  unsigned seqno;
  int running;
  int thresh;
  int leaving;
  unsigned int next_sweep;
  unsigned int sweep;
  int exited ;
  hot_view_state_t vs;
  hot_ens_JoinOps_t jops;
} state ;

void scheck(state *s,hot_gctx_t g) {
  assert(s) ;
  assert(s->magic == HOT_PING_MAGIC) ;
  assert(!s->exited) ;
  assert(s->gctx==g) ;
}

void trace(char *s) {
  /*
  printf("HOT_PING:%s\n",s) ;
  */
}

void join(
        int thresh,
	char **argv
) ;

double gettime(void) {
  struct timeval tv ;
  int ret ;
  ret = gettimeofday(&tv,NULL) ;
  if (ret == -1) {
    perror("gettimeofday") ;
    exit(1) ;
  }

  return (double) tv.tv_sec + (double) tv.tv_usec / 1e6 ;
}


void check_stats(void) {
  static int nevents = 0 ;
  static double start = 0.0 ;

  if (start == 0.0)
    start = gettime() ;
  
  nevents++ ;
  if (nevents % 100 == 0) {
    printf ("HOT_PING:stats:c=%d s=%d v=%d e=%d b=%d h=%d j=%d (total=%d) %f\n", 
	    stats.ncasts,
	    stats.nsends,
	    stats.nviews,
	    stats.nexits,
	    stats.nblocks,
	    stats.nheartbeats,
	    stats.njoins,
	    (stats.ncasts+stats.nsends+stats.nviews+stats.nexits+stats.nblocks+stats.nheartbeats+stats.njoins),
	    (gettime() - start)
	   ) ;
  }
}

/********************** Callbacks *****************************************/

void receive_cast_thread(void *env) {
  state *s = (state*) env ;
  char contents[128];
  hot_err_t err;
  unsigned pos;
  hot_msg_t msg ;
  hot_uint32 u;

  char buf [1000] ;
  msg = hot_msg_Create();
  
  sprintf(buf, "mcast<%d>", s->seqno);
  err = hot_msg_Write(msg, buf, strlen(buf));
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  u = s->seqno;
  err = hot_msg_WriteUint32(msg, &u);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  err = hot_ens_Cast(s->gctx, msg, NULL);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  err = hot_msg_Release(&msg);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
}

void receive_cast(
        hot_gctx_t gctx,
	void *env,
	hot_endpt_t *origin, 
	hot_msg_t msg
) {
  state *s = (state*) env ;
  char contents[128];
  hot_err_t err;
  unsigned pos;
  hot_uint32 u;

  scheck(s,gctx) ;

  err = hot_msg_ReadUint32(msg, &u);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  err = hot_msg_GetPos(msg, &pos);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  err = hot_msg_Read(msg, contents, pos);
  if (err != HOT_OK)
    hot_sys_Panic(hot_err_ErrString(err));
  
  contents[pos] = 0;

  if (s->vs.nmembers == 2 &&
      memcmp(origin,&s->vs.members[s->vs.rank],sizeof(*origin))) {
#ifdef NOT
    hot_thread_Create(receive_cast_thread, (void*)s, NULL) ;
#endif

    char buf [1000] ;
    msg = hot_msg_Create();
    
    sprintf(buf, "mcast<%d>", s->seqno);
    err = hot_msg_Write(msg, buf, strlen(buf));
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
    
    u = s->seqno;
    err = hot_msg_WriteUint32(msg, &u);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
    
    err = hot_ens_Cast(s->gctx, msg, NULL);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
    
    err = hot_msg_Release(&msg);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
  }
  
  stats.ncasts++ ;
  if (stats.ncasts > 1000)
    exit(0) ;
  check_stats() ;
  /*printf("mcast [%d]: '%s' from %s\n", (int)u, contents, origin->name);*/
}


void receive_send(
        hot_gctx_t gctx,
	void *env,
	hot_endpt_t *origin,
	hot_msg_t msg
) {}

/* We have accepted a new view.  The new view state is specified.
 */
void accepted_view(
        hot_gctx_t gctx,
	void *env,
	hot_view_state_t *view_state
) {
  state *s = (state*) env ;
  int size ;
  scheck(s,gctx) ;

  /* Release the old view.
   */
  if (s->vs.members != NULL) {
    free(s->vs.members) ;
  }
  s->vs = *view_state; 
  size = sizeof(s->vs.members[0])*s->vs.nmembers ;
  s->vs.members = (hot_endpt_t*) malloc(size) ;
  memcpy(s->vs.members,view_state->members,size) ;

  /*if (s->vs.rank == 0)*/
  printf("HOT_PING:view (nmembers=%d, rank=%d)\n",s->vs.nmembers,s->vs.rank);

  s->running = 1;

  stats.nviews++ ;
  check_stats() ;

  {
    hot_msg_t msg ;
    hot_err_t err;
    unsigned pos;
    hot_uint32 u;
    char buf [1000] ;

    msg = hot_msg_Create();

    sprintf(buf, "mcast<%d>", s->seqno);
    err = hot_msg_Write(msg, buf, strlen(buf));
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));

    u = s->seqno;
    err = hot_msg_WriteUint32(msg, &u);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));

    err = hot_ens_Cast(s->gctx, msg, NULL);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));

    err = hot_msg_Release(&msg);
    if (err != HOT_OK)
      hot_sys_Panic(hot_err_ErrString(err));
  }
}

/* A periodic heartbeat event has occurred.  The 'time' argument 
 * is an approximation of the current time.
 */
void heartbeat(
        hot_gctx_t gctx,
	void *env,
	unsigned time
) {}

void exit_cb(
        hot_gctx_t gctx,
        void *env
) {}

void block(
        hot_gctx_t gctx,
	void *env
) {}

void join(
        int thresh,
	char **argv
) {
  hot_err_t err ;
  state *s ;
  s = (state *) malloc(sizeof(*s)) ;
  memset(s,0,sizeof(*s)) ;
  
  s->thresh = thresh ;
  s->magic = HOT_PING_MAGIC ;
  s->jops.heartbeat_rate = 3000 ;
  s->sweep = 3000 ;
  s->jops.argv = argv;
  s->next_sweep = 0 ;
  strcpy(s->jops.transports, "UDP");
  strcpy(s->jops.group_name, "HOT_ping");
  
  strcpy(s->jops.protocol, "") ; 
  strcpy(s->jops.properties, "Gmp:Sync:Heal:Frag:Suspect");
  s->jops.use_properties = 1;
  strcpy(s->jops.params, "suspect_max_idle=3:int;suspect_sweep=1.000:time");
  s->jops.groupd = 0;
  
  s->jops.conf.receive_cast = receive_cast;
  s->jops.conf.receive_send = receive_send;
  s->jops.conf.accepted_view = accepted_view;
  s->jops.conf.heartbeat = heartbeat;
  s->jops.conf.exit = exit_cb;
  s->jops.conf.block = block;
  s->jops.debug = 0 ;
  strcpy(s->jops.outboard, "ENS_OUTBOARD");
  
  s->jops.env = s;
  
  /* Join the group.
   */
  err = hot_ens_Join(&s->jops, &s->gctx);
  if (err != HOT_OK) {
    hot_sys_Panic(hot_err_ErrString(err));
  }

  stats.njoins++ ;
  check_stats() ;
}

int main(int argc, char *argv[]) {
  hot_sema_t sema ;
  hot_err_t err ;
  int thresh = 5 ;
  int nmembers = 7 ;
  int i ;
  
  trace("HOT_PING: starting");
  
  /* Initialize state data.
   */
  srand(time(NULL));

  join(thresh,argv) ;

  hot_sema_Create(0,&sema) ;
  hot_sema_Dec(sema) ;
  return 0 ;
}
