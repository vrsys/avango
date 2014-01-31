/**************************************************************/
/* COMMON.H */
/* Author: Ohad Rodeh  10/2002 */
/* Based on code by Mark Hayden */
/**************************************************************/
#ifndef __SKT_H__
#define __SKT_H__

#include "caml/config.h"
#include "caml/mlvalues.h"
#include "caml/misc.h"
#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/fail.h"
#include "caml/callback.h"

/* This is for zero-copy marshaling.
 */
#include "caml/intext.h"

/**************************************************************/
/* Include the memory functions.
 */
#include "e_iovec.h"
#include "mm.h"

/* Include the socket descriptor handles.
 */
#include "sockfd.h"

/**************************************************************/
/* Include things specific to the machine (nt/unix)
 */
#include "platform.h"
/**************************************************************/
/* Debuggin macros. 
 */

//#define SKTTRACE(x) printf x; fflush(stdout)
#define SKTTRACE(x) {}
//#define SKTTRACE2(x) printf x; fflush(stdout)
#define SKTTRACE2(x) {}
//#define SKTTRACE3(x) printf x; fflush(stdout)
#define SKTTRACE3(x) {}
/**************************************************************/
/* A set of errors to ignore on receiving a UDP or a TCP packet.
 */
void skt_udp_recv_error(void);
int skt_tcp_recv_error(char *debug);

/**************************************************************/

extern void serror(char * cmdname);

#define GET_INET_ADDR(v) (*((uint32 *) (v)))

/* Defined inside CAML win32unix library
 *
 * BEGIN: Copied from ocaml/otherlibs/unix/{socketaddr.h,unixsupport.h}
 */
#define Nothing ((value) 0)

//extern union sock_addr_union sock_addr;

#ifdef HAS_SOCKLEN_T
typedef socklen_t socklen_param_type;
#else
typedef int socklen_param_type;
#endif

extern void get_sockaddr(value mladr,
			 union sock_addr_union * adr /*out*/,
			 socklen_param_type * adr_len /*out*/);

extern value alloc_sockaddr(union sock_addr_union * adr /*in*/,
			    socklen_param_type adr_len);

/* END
 */
#endif

/**************************************************************/
