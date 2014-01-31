/**************************************************************/
/* SOCKOPT.C: socket operations not supported natively by Caml */
/* Author: Ohad Rodeh  10/2002 */
/* Based on code by Mark Hayden */
/**************************************************************/
#include "skt.h"
/**************************************************************/

#ifndef HAS_SOCKETS
#error This platform does not support sockets, cannot compile optimized socket
library.
#endif

/* Support for disabling ICMP error reports on Linux.  See
 * ensemble/BUGS.  Also see in the Linux kernel excerpted below:
  
 *   Various people wanted BSD UDP semantics. Well they've come back
 *   out because they slow down response to stuff like dead or
 *   unreachable name servers and they screw term users something
 *   chronic. Oh and it violates RFC1122. So basically fix your client
 *   code people.
 *   (linux/net/ipv4/udp.c, line ~545)
	 
 */
value skt_setsockopt_bsdcompat(	/* ML */
        value sock_v,
	value bool_v
) {
#ifdef SO_BSDCOMPAT
  int sock ;
  int ret ;
  int flag ;
  sock = Socket_val(sock_v) ;
  flag = Bool_val(bool_v) ;
  ret = setsockopt(sock, SOL_SOCKET, SO_BSDCOMPAT, &flag, sizeof(flag)) ;
  if (ret < 0) serror("setsockopt:bsdcompat");
#endif
  return Val_unit ;
}

