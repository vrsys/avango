/**************************************************************/
/* E_IOVEC.H */
/* Author: Ohad Rodeh 11/2001 */
/**************************************************************/

#ifndef __E_IOVEC_H__
#define __E_IOVEC_H__

/* The type of iovec's. On each system, they are defined in
 * some native form.
 */
#include <winsock2.h>
typedef WSABUF ce_iovec_t ;
#define Iov_len(iov) (iov).len
#define Iov_buf(iov) (iov).buf

#endif


