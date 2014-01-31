/**************************************************************/
/* E_IOVEC.H */
/* Author: Ohad Rodeh 11/2001 */
/**************************************************************/

/* The type of iovec's. On each system, they are defined in
 * some native form.
 *
 * We currently assume that all unixes define iovec in 
 * sys/socket.h
 */
#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
typedef struct iovec ce_iovec_t ;
#define Iov_len(iov) (iov).iov_len
#define Iov_buf(iov) (iov).iov_base

#else
#include <winsock2.h>
typedef WSABUF ce_iovec_t ;
#define Iov_len(iov) (iov).len
#define Iov_buf(iov) (iov).buf

#endif



