/* 
 * Contents: Interface to NT threads.
 *
 * Author: Werner Vogels, 1996
 */
#include <assert.h>
#include <windows.h>
 
#include "hot_error.h"
#include "hot_thread.h"
#include "hot_sys.h"

#define HOT_SEMA_MAGIC 0x29836abe
#define HOT_LOCK_MAGIC 0x542bacdf

struct hot_sema {
    unsigned magic;
    HANDLE sema;
};

struct hot_lock {
    unsigned            magic;
    CRITICAL_SECTION	lock;
};

typedef void*(*thread_routine)(void*);

static void thread_fun(void *env, void *param) {
    thread_routine fun = (thread_routine) env;
    (void) (*fun)(param);
}

/* Create a new thread.  
 */
hot_err_t hot_thread_Create(
        void (*routine)(void*),
        void *arg,
	hot_thread_attr_t *attr
) {
    DWORD   dwThreadID, dwStackSize = 0;
//    HANDLE  hThread;

//    LPTHREAD_START_ROUTINE func = (LPTHREAD_START_ROUTINE)routine;

    if (attr)
        dwStackSize = attr->stacksize;
        
	if (CreateThread(NULL, dwStackSize, (LPTHREAD_START_ROUTINE)routine, arg, 0, &dwThreadID) == NULL)
	    return hot_err_Create(0, "hot_thread_Create: t_create");
    else
        return HOT_OK;
}

/* Yield to other runnable threads (if any).
 */
hot_err_t hot_thread_Yield(void) {
  Sleep(0);
  return HOT_OK;
}

/* Yield to other runnable threads (if any).
 */
hot_err_t hot_thread_Usleep(int usecs) {
  assert(usecs >= 0) ;
  Sleep(usecs / 1000) ;
  return HOT_OK;
}

/******************************** Locks ***********************************/

/* Create a new lock.  
 */
 hot_err_t hot_lck_Create(/*OUT*/hot_lock_t *lp) {
    struct hot_lock *lckp;

    if ((lckp = (struct hot_lock*) malloc(sizeof(struct hot_lock))) == NULL)
	hot_sys_Panic("hot_lck_Create: malloc returned NULL");

    InitializeCriticalSection(&lckp->lock);
    lckp->magic = HOT_LOCK_MAGIC;

    *lp = (hot_lock_t) lckp;
    return HOT_OK;
}

/* Destroy a lock. 
 */
 hot_err_t hot_lck_Destroy(hot_lock_t l) {
    struct hot_lock *lp = (struct hot_lock *) l;
    if (lp == NULL || lp->magic != HOT_LOCK_MAGIC)
	return hot_err_Create(0, "hot_lck_Destroy: bad lock handle");

    DeleteCriticalSection(&lp->lock);
    lp->magic = 0;
    free(lp);

    return HOT_OK;
}

/* Locks/unlocks a lock.  
 */
 hot_err_t hot_lck_Lock(hot_lock_t l) {
    struct hot_lock *lp = (struct hot_lock *) l;
    if (lp == NULL || lp->magic != HOT_LOCK_MAGIC)
	return hot_err_Create(0, "hot_lck_Lock: bad lock handle");

    EnterCriticalSection(&lp->lock);
    return HOT_OK;
}

 hot_err_t hot_lck_Unlock(hot_lock_t l) {
    struct hot_lock *lp = (struct hot_lock *) l;
    if (lp == NULL || lp->magic != HOT_LOCK_MAGIC)
	return hot_err_Create(0, "hot_lck_Unlock: bad lock handle");

    LeaveCriticalSection(&lp->lock);
    return HOT_OK;
}

/******************************* Semaphores *********************************/

/* Create a semaphore.  
 */
 hot_err_t hot_sema_Create(int initial_value, /*OUT*/ hot_sema_t *semap) {
    struct hot_sema *sp;

    if ((sp = (struct hot_sema*) malloc(sizeof(struct hot_sema))) == NULL)
	hot_sys_Panic("hot_sema_Create:  malloc returned NULL");

    if ((sp->sema = CreateSemaphore(NULL, initial_value, 0x7fffffff, NULL)) == NULL) {
        printf("gle %d\n",GetLastError());
        hot_sys_Panic("hot_sema_Create: failed to create semaphore");
    }
        
    sp->magic = HOT_SEMA_MAGIC;

    *semap = (hot_sema_t) sp;
    return HOT_OK;
}

/* Destroy a semaphore.  
 */
 hot_err_t hot_sema_Destroy(hot_sema_t sema) {
    struct hot_sema *sp = (struct hot_sema *) sema;
    if (sp == NULL || sp->magic != HOT_SEMA_MAGIC)
	return hot_err_Create(0, "hot_sema_Destroy: bad sema handle");

    CloseHandle(sp->sema);
    sp->magic = 0;

    free(sp);
    return HOT_OK;
}

/* Increase the value of a semaphore.  
 */
 hot_err_t hot_sema_Inc(hot_sema_t sema) {
    struct hot_sema *sp = (struct hot_sema *) sema;
    if (sp == NULL || sp->magic != HOT_SEMA_MAGIC)
	return hot_err_Create(0, "hot_sema_Inc: bad sema handle");

    ReleaseSemaphore(sp->sema, 1, NULL);
    return HOT_OK;
}

/* Decrease the value of a semaphore (may block).  
 */
 hot_err_t hot_sema_Dec(hot_sema_t sema) {
    struct hot_sema *sp = (struct hot_sema *) sema;
    if (sp == NULL || sp->magic != HOT_SEMA_MAGIC)
	return hot_err_Create(0, "hot_sema_Dec: bad sema handle");

    WaitForSingleObject(sp->sema, INFINITE);
    return HOT_OK;
}
