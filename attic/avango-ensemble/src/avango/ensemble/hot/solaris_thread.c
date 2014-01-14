/* 
 * Contents: Interface to solaris native threads.
 *
 * Author:  Alexey Vaysburd, January 1997
 * Bug fixes: Mark Hayden, 1997
 */

#include <assert.h>
#include <thread.h>
#include <synch.h>
#include "hot_sys.h"
#include "hot_error.h"
#include "hot_thread.h"

#ifdef HOT_USE_THREADS

typedef void*(*hot_thread_routine)(void*);

/******************************** Threads ***********************************/

/* Create a new thread
 */
hot_err_t hot_thread_Create(
        void (*routine)(void*), 
	void *arg, 
	hot_thread_attr_t *attr
) {
  if (thr_create(NULL, (attr) ? attr->stacksize : 0, 
		 (hot_thread_routine) routine, arg, 0, NULL))     
    return hot_err_Create(0,"hot_thread_Create: thr_create");
  else
    return HOT_OK ;
}

/* Yield the processor to another thread.
 */
hot_err_t hot_thread_Yield(void) {
  /* Solaris threads do not support yield.
   */
  return HOT_OK ;
}

/* Sleep for usecs.
 */
hot_err_t hot_thread_Usleep(int usecs) {
  struct timespec ts ;
  int ret ;
  int secs;
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

/******************************** Locks ***********************************/

struct hot_lock {
  mutex_t mutex ;
} ;


/* Create a lock
 */
hot_err_t hot_lck_Create(hot_lock_t *lockp) {
  hot_lock_t lock ;
  assert(lockp) ;
  *lockp = NULL ;

  lock = (hot_lock_t) malloc(sizeof(*lock)) ;
  if (lock == NULL)
    return hot_err_Create(0,"hot_lck_Create: malloc returned NULL") ;

  if (mutex_init(&lock->mutex, 0, NULL))
    return hot_err_Create(0,"hot_lck_Create: mutex_init") ;

  *lockp = lock ;
  return HOT_OK ;
}

/* Destroy a lock
 */
hot_err_t hot_lck_Destroy(hot_lock_t lock) {
  assert(lock) ;

  if (mutex_destroy(&lock->mutex))
    return hot_err_Create(0,"hot_lck_Destroy: mutex_destroy");

  free(lock);
  return HOT_OK ;
}

/* Lock a lock
 */
hot_err_t hot_lck_Lock(hot_lock_t lock) {
  assert(lock) ;

  if (mutex_lock(&lock->mutex))
    return hot_err_Create(0,"hot_lck_Lock: mutex_lock") ;

  return HOT_OK ;
}

/* Unlock a lock
 */
hot_err_t hot_lck_Unlock(hot_lock_t lock) {
  assert(lock) ;

  if (mutex_unlock(&lock->mutex))
    return hot_err_Create(0,"hot_lck_Unlock: mutex_unlock") ;
  return HOT_OK ;
}

/******************************** Semaphores *******************************/

struct hot_sema {
  sema_t sema ;
} ;

/* Create a semaphore.
 */
hot_err_t hot_sema_Create(
        int initial_value, 
	hot_sema_t *semap /*OUT*/ 
) {
  hot_sema_t sema ;
  assert(semap) ;
  *semap = NULL ;

  sema = (hot_sema_t) malloc(sizeof(*sema)) ;
  if (sema == NULL)
    return hot_err_Create(0,"hot_sema_Create: malloc returned NULL");

  if (sema_init(&sema->sema, initial_value, 0, NULL))
    return hot_err_Create(0,"hot_sema_Create: sema_init");

  *semap = sema ;
  return HOT_OK;
}

hot_err_t hot_sema_Destroy(hot_sema_t sema) {
  assert(sema) ;
  
  if (sema_destroy(&sema->sema))
    return hot_err_Create(0,"hot_sema_Destroy: sema_destroy");

  free(sema);
  return HOT_OK;
}

hot_err_t hot_sema_Inc(hot_sema_t sema) {
  assert(sema) ;
  
  if (sema_post(&sema->sema))
    return hot_err_Create(0, "hot_sema_Inc: sema_post");
  
  return HOT_OK;	
}

hot_err_t hot_sema_Dec(hot_sema_t sema) {
  assert(sema) ;
  
  if (sema_wait(&sema->sema))
    return hot_err_Create(0, "hot_sema_Inc: sema_wait");
  
  return HOT_OK;
}

#else /* HOT_USE_THREADS */

hot_err_t hot_thread_Create(void (*routine)(void*), 
			    void *arg, 
			    hot_thread_attr_t *attr) { 
  (*routine)(arg);
  return HOT_OK;
}
hot_err_t hot_thread_Yield(void) { return HOT_OK; }
hot_err_t hot_thread_Usleep(int usecs) { return HOT_OK; }
hot_err_t hot_lck_Create(hot_lock_t *l) { return HOT_OK; }
hot_err_t hot_lck_Destroy(hot_lock_t l) { return HOT_OK; }
hot_err_t hot_lck_Lock(hot_lock_t l) { return HOT_OK; }
hot_err_t hot_lck_Unlock(hot_lock_t l) { return HOT_OK; }
hot_err_t hot_sema_Create(int initial_value, /*OUT*/ hot_sema_t *semap) {
    return HOT_OK;
}
hot_err_t hot_sema_Destroy(hot_sema_t sema) { return HOT_OK; }
hot_err_t hot_sema_Inc(hot_sema_t sema) { return HOT_OK; }
hot_err_t hot_sema_Dec(hot_sema_t sema) { return HOT_OK; }

#endif /* HOT_USE_THREADS */
