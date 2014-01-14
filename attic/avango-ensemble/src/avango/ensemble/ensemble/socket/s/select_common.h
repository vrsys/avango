/**************************************************************/
/* SELECT_COMMON.H: optimized select and polling helper functions */
/* Author: Mark Hayden, 8/97 */
/* Refactoring: Ohad Rodeh 10/2002 */
/**************************************************************/
/* Notes:
 * 1. Based on otherlibs/unix/select.c in Objective Caml
 * 2. The code has been placed in an H file because some C compilers
 *    respect the INLINE declaration only for H files. 
 */
/**************************************************************/
#include "skt.h"

typedef fd_set file_descr_set;

#ifndef HAS_SOCKETS
#error No socket support, cannot compile the optimized socket library.
#endif

INLINE 
file_descr_set *fdarray_to_fdset(
    value fdinfo,
    file_descr_set *fdset
    )
{
    value sock_arr_v ;
    int i, n ;
    ocaml_skt_t fd ;
    
    /* Go to the socket array.
     */
    sock_arr_v = Field(fdinfo,0) ;
    
    /* If no sockets then just return NULL. 
     */
    n = Wosize_val(sock_arr_v) ;
    if (n == 0) 
	return NULL ;
    
    /* Set the fdset.
     */
    FD_ZERO(fdset) ;
    for (i=0;i<n;i++) {
	fd = Socket_val(Field(sock_arr_v,i)) ;
//	SKTTRACE(("select: fd=%d\n", fd));
	FD_SET(fd, fdset) ;
    }
    
    return fdset ;
}

INLINE 
void fdset_to_fdarray(
    value fdinfo,
    file_descr_set *fdset
    )
{
    value sock_arr_v, ret_arr_v ;
    int i, n ;
    ocaml_skt_t fd ;
    
    /* If fdset is NULL then just return.
     * The array should be of zero length.
     */
    if (!fdset) return ;
    
    /* Get the arrays (and length).
     */
    sock_arr_v = Field(fdinfo,0) ;
    ret_arr_v = Field(fdinfo,1) ;
    n = Wosize_val(ret_arr_v) ;
    
    /* Scan through the fdset.
     */
    for (i=0;i<n;i++) {
	fd = Socket_val(Field(sock_arr_v,i)) ;
	Field(ret_arr_v,i) = Val_bool(FD_ISSET(fd, fdset)) ;
    }
}

