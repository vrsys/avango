/* 
 * Contents: Interface to pthreads.
 *
 * Author:  Alexey Vaysburd, December 1996
 * Bug fixes: Mark Hayden, 1997
 * Added support for native locks, semaphores: Mark Hayden, 1998
 * Clean up: Ohad Rodeh 10/2002
 */

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "hot_sys.h"
#include "hot_error.h"
#include "hot_thread.h"

/**************************************************************/
/* These three functions are common to (nearly) all platforms
 */
static void *
pt_malloc(int size)
{
    void *ptr;
    
    ptr = malloc(size);
    if (ptr == NULL) 
	hot_sys_Panic("Out of memory, exiting");
    return ptr;
}

/* Sleep for usecs.
 */
hot_err_t hot_thread_Usleep(int usecs) {
    struct timespec ts ;
    int ret ;
    int secs ;
    if (usecs <= 0)
	return HOT_OK ;
    
    secs = usecs / 1000000/*1E6*/ ;
    usecs %= 1000000/*1E6*/ ;
    
    ts.tv_sec = secs ;
    ts.tv_nsec = usecs * 1000 ;
    
    assert(ts.tv_nsec >= 0) ;
    assert(ts.tv_nsec < 1000000000/*1E9*/) ;
    assert(ts.tv_sec  >= 0) ;
    
    ret = nanosleep(&ts, NULL) ;
    if (ret < 0)
	return hot_err_Create(0,"hot_thread_Usleep: nanosleep") ;
    return HOT_OK ;
}

/* Yield the processor to another thread.
 * POSIX_PRIORITY_SCHEDULING tells us that sched_yield is available.  
 * Otherwise, we just call s
 */
hot_err_t hot_thread_Yield(void) {
    hot_err_t err ;
#ifdef _POSIX_PRIORITY_SCHEDULING
    sched_yield() ;
    err = HOT_OK ;
#else
    err = hot_thread_Usleep(1) ;
#endif
    return err ;
}

struct hot_sema {
    sem_t sema ;
};

struct hot_lock {
    pthread_mutex_t lck;
};

static pthread_t td;

typedef void*(*hot_thread_routine)(void*);

/* Create a new thread
 */
hot_err_t hot_thread_Create(void (*routine)(void*), 
			    void *arg,
			    hot_thread_attr_t *attr_arg)
{
    pthread_attr_t attr;
    
    if (pthread_attr_init(&attr))
	hot_sys_Panic("hot_thread_Create: pthread_attr_init");
    
/* Linux threads currently do not support pthread_attr_setstacksize.
 */
#if !(defined(linux))
    if (attr_arg != NULL) {
	if (pthread_attr_setstacksize(&attr, attr_arg->stacksize))
	    hot_sys_Panic("hot_thread_Create: pthread_attr_setstacksize");
    }
#endif
    
/* Linux pthreads use an "enum" for this rather than #define, so we
 * just check for Linux.  Which platforms don't support this?  
 */
#if defined(PTHREAD_CREATE_DETACHED) || defined(linux)
    /* This is done in order to release the thread resources when the
     * thread dies.  
     */
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
	hot_sys_Panic("hot_thread_Create: pthread_attr_setdetachstate");
#endif
    
    if (pthread_create(&td, &attr, (hot_thread_routine) routine, arg))
	hot_sys_Panic("hot_thread_Create: pthread_create");
    pthread_attr_destroy(&attr);
    return HOT_OK;
}


/* Create a semaphore.
 */
hot_err_t hot_sema_Create(
    int initial_value,
    hot_sema_t *semap /*OUT*/ 
    ) {
    hot_sema_t s ;
    hot_err_t err ;
    assert(semap) ;
    *semap = NULL ;
    
    s = (hot_sema_t) pt_malloc(sizeof(struct hot_sema)) ;
    if (sem_init(&s->sema,0,initial_value))
	hot_sys_Panic("hot_sema_Create: sema_init") ;
    
    *semap = s;
    return HOT_OK ;
}

hot_err_t hot_sema_Destroy(hot_sema_t sp) {
    assert(sp) ;
    
    if (sem_destroy(&sp->sema))
	return hot_err_Create(0,"hot_sema_Destroy");
    
    /* Zero the memory.
     */
    memset(sp,0,sizeof(struct hot_sema)) ;
    free(sp);
    return HOT_OK;
}


hot_err_t hot_sema_Inc(hot_sema_t sp) {
    assert(sp) ;
    if (sem_post(&sp->sema)) {
	return hot_err_Create(0,"hot_sema_Inc: sem_post") ;
    }
    return HOT_OK;	
}

hot_err_t hot_sema_Dec(hot_sema_t sp) {
  assert(sp) ;

  int semret = 0;

  errno = 0;
  
  while ((semret = sem_wait(&sp->sema)) && errno == EINTR)
    ;
  
  if (semret) {
    return hot_err_Create(0,"hot_sema_Dec: sem_wait") ;
  }
  return HOT_OK;	
}

/* Create a lock.
 */
hot_err_t hot_lck_Create(hot_lock_t *lp) {
    hot_err_t err ;
    hot_lock_t l;
    assert(lp) ;
    *lp = NULL ;
    
    l = (hot_lock_t) pt_malloc(sizeof(struct hot_lock)) ;
    
    if (pthread_mutex_init(&l->lck,NULL))
	hot_sys_Panic("hot_sema_Create: sema_init") ;
    
    *lp = l ;
    return HOT_OK ;
}


hot_err_t
hot_lck_Destroy(hot_lock_t l) {
    assert(l) ;
    
    if (pthread_mutex_destroy(&l->lck))
	hot_sys_Panic("hot_lck_Destroy: pthread_mutex_destroy");
    free(l);
    return HOT_OK;
}

hot_err_t
hot_lck_Lock(hot_lock_t l) {
    assert(l) ;
    if (pthread_mutex_lock(&l->lck))
	hot_sys_Panic("hot_lck_Lock: pthread_mutex_lock") ;
    return HOT_OK;
}

hot_err_t
hot_lck_Unlock(hot_lock_t l) {
    assert(l) ;
    if (pthread_mutex_unlock(&l->lck))
	hot_sys_Panic("hot_lck_Unlock: pthread_mutex_unlock") ;
    return HOT_OK;
}

