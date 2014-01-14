/**************************************************************/
/* MULTICAST.C: support for IPMC operations. */
/* Author: Ohad Rodeh 7/2001 */
/* Based on code by Mark Hayden */
/**************************************************************/
#include "skt.h"

#ifndef HAS_SOCKETS
#error No socket support, cannot compile the optimized socket library.
#endif

/**************************************************************/
value skt_has_ip_multicast(void) {	/* ML */
  return Val_true ; 
}

/**************************************************************/
/* Is the address a D class address?
 */
value skt_in_multicast(
		       value inet_addr_v
		       )
{
  int ret;
  uint32 inet_addr = GET_INET_ADDR(inet_addr_v);

  ret = IN_MULTICAST(ntohl(inet_addr));
 
  return Val_bool(ret);
}	   

/**************************************************************/

value skt_setsockopt_ttl(	
		     value sock_v,
		     value ttl_v
) {
  int sock = Int_val(sock_v);
  int ttl = Int_val(ttl_v);
  int ret ;
  unsigned char char_value ;

  char_value = (unsigned char) ttl ;
  ret = setsockopt(sock,
		   IPPROTO_IP, IP_MULTICAST_TTL,
		   &char_value, sizeof(char_value)) ;
  if (ret < 0) 
    serror("setsockopt(time-to-live)");

  return Val_unit;
}


/**************************************************************/
/* Join an IP multicast group.
 */
value skt_setsockopt_join(		
		      value sock_v,
		      value group_inet_v,
		      value port_v  // Unused here.
) {
  int sock = Int_val(sock_v);
  uint32 group_inet = GET_INET_ADDR(group_inet_v);
  struct ip_mreq mreq ;
  int ret ;

  if (!(IN_MULTICAST(ntohl(group_inet))))
    serror("setsockopt:join address is not a class D address");
  
  mreq.imr_multiaddr.s_addr = group_inet;
  mreq.imr_interface.s_addr = htonl(INADDR_ANY) ;

  ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		   (void*)&mreq, sizeof(mreq)) ;

  if (ret < 0) serror("setsockopt:join");
  return Val_unit;
}

/**************************************************************/
/* Set the loopback switch.
 */
value skt_setsockopt_loop(
		 value sock_v,
		 value onoff_v)
{
  int sock = Int_val(sock_v);
  int onoff = Int_val(onoff_v);
  int ret ;
  u_char flag;

  flag = onoff;
  ret = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
		   (const char*)&flag, sizeof(flag));

  if (ret < 0) serror ("setsockopt:loopback");
  return Val_unit;
}

/**************************************************************/

/* Leave an IP multicast group.
 */
value skt_setsockopt_leave(	/* ML */
	value sock_v,
	value group_inet_v
) {
  struct ip_mreq mreq ;
  uint32 group_inet = GET_INET_ADDR(group_inet_v);
  int sock = Int_val(sock_v) ;
  int ret ;
  
  mreq.imr_multiaddr.s_addr = group_inet ;
  mreq.imr_interface.s_addr = INADDR_ANY ;

  ret = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		   (void*)&mreq, sizeof(mreq)) ;

  if (ret < 0)  serror("setsockopt:leave");
  return Val_unit ;
}

/**************************************************************/
