/**************************************************************/
/* SOCKFD.C: socket/fd conversion function. */
/* Author: Mark Hayden, 10/97 */
/**************************************************************/
#include "skt.h"
/**************************************************************/

/* Return an integer representation of the handle.
 */
value skt_int_of_file_descr(value fd_v) { /* ML */
  return Val_long((long)Socket_val(fd_v)) ;
}



