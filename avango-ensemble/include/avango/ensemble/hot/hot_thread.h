/* Interface to system/machine-dependent threads implementation.
 */

#ifndef __HOT_THREAD_H__
#define __HOT_THREAD_H__

#if defined(__cplusplus)
extern "C" {
#endif

/* Define if we are going to need threads.
 */
#define HOT_USE_THREADS

/******************************** Threads ***********************************/

typedef struct {
  int stacksize;
} hot_thread_attr_t;

/* Create a new thread.  Returns 0 if OK.
 */
hot_err_t hot_thread_Create(void (*routine)(void*), 
			    void *arg,
			    hot_thread_attr_t *attr);

/* Yield to other runnable threads (if any).
 */
hot_err_t hot_thread_Yield(void);

/* Sleep for usecs.
 */
hot_err_t hot_thread_Usleep(int usecs);

/******************************** Locks ***********************************/

/* Obscured type
 */
typedef struct hot_lock* hot_lock_t;

/* Create a new lock.  Returns 0 if OK.
 */
hot_err_t hot_lck_Create(/*OUT*/hot_lock_t*);

/* Destroy a lock.  Returns 0 if OK.
 */
hot_err_t hot_lck_Destroy(hot_lock_t);

/* Locks/unlocks a lock.  Returns 0 if OK.
 */
hot_err_t hot_lck_Lock(hot_lock_t);
hot_err_t hot_lck_Unlock(hot_lock_t);

/******************************* Semaphores *********************************/

/* Obscured type
 */
typedef struct hot_sema* hot_sema_t;

/* Create a semaphore.  Returns 0 if OK.
 */
hot_err_t hot_sema_Create(int initial_value, /*OUT*/ hot_sema_t *semap);

/* Destroy a semaphore.  Returns 0 if OK.
 */
hot_err_t hot_sema_Destroy(hot_sema_t sema);

/* Increase the value of a semaphore.  Returns 0 if OK.
 */
hot_err_t hot_sema_Inc(hot_sema_t sema);

/* Decrease the value of a semapghore (may block).  Returns 0 if OK.
 */
hot_err_t hot_sema_Dec(hot_sema_t sema);

#if defined(__cplusplus)
}
#endif

#endif /* __HOT_THREAD_H__ */
