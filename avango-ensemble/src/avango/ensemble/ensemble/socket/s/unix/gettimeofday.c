/**************************************************************/
/* GETTIMEOFDAY.C: optimized gettimeofday stubs. */
/* Authors: Robbert van Renesse, Mark Hayden, 10/97 */
/**************************************************************/
#include "skt.h"
/**************************************************************/

#ifndef HAS_GETTIMEOFDAY
#error "This platform does not support gettimeofday, cannot compile the native socket library. Probably will not be able to compile and use Ensemble"
#endif

#include <sys/time.h>

#define ONEMILLION 1000000
#define TENMILLION 10000000

/* NOTE: Ensemble now use timeval = {sec10; usec} */
INLINE static
void set(value tv_v,long sec,long usec) {
  usec += ((sec % 10) * ONEMILLION);
  sec = sec / 10;
  if (usec < 0) {
    fprintf(stderr,"SOCKET:gettimeofday:usec<0, fixing\n") ;
    sec  -= 1 ;
    usec += TENMILLION ;
    assert(usec >= 0 && usec < TENMILLION) ;
  } else if (usec >= TENMILLION) {
    fprintf(stderr,"SOCKET:gettimeofday:usec>1E6, fixing\n") ;
    sec  += 1 ;
    usec -= TENMILLION ;
    assert(usec >= 0 && usec < TENMILLION) ;
  }
  Field(tv_v,0) = Val_long(sec) ;
  Field(tv_v,1) = Val_long(usec) ;
}
    

value skt_gettimeofday(value a_v) {               /* ML */
  struct timeval tp ;
  
  /* We've declared this function to be "noalloc", so we have to call
   * invalid_argument if there is an error (which there never should
   * be...).  
   */
  if (gettimeofday(&tp, NULL) == -1) {
    invalid_argument("gettimeofday failed");
  }
  set(a_v,tp.tv_sec,tp.tv_usec) ;
  return Val_unit ;
}

