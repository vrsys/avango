/**************************************************************/
/* SENDRECV.C */
/* Author: Ohad Rodeh  9/2001 */
/* This is a complete rewrite of previous code by Mark Hayden */
/**************************************************************/
#include "skt.h"
#include "prefix_suffix.h"
/**************************************************************/

/* Read from a TCP socket into a string.
 */
value skt_recv(sock, buff, ofs, len) /* ML */
    value sock, buff, ofs, len;
{
    int ret;
    
    SKTTRACE(("skt_recv(\n"));
    ret = recv(Socket_val(sock), &Byte(buff, Long_val(ofs)), Int_val(len),0) ;
    if (ret == -1) serror("skt_recv");
    SKTTRACE2(("len=%d)\n",ret));
    return Val_int(ret);
}

/* Read from a TCP socket into an iovec.
 */
value skt_recv_iov(sock_v, iov_v, ofs_v, len_v) /* ML */
    value sock_v, iov_v, ofs_v, len_v;
{
    int ret;
    ocaml_skt_t sock = Socket_val(sock_v);
    char* cbuf = mm_Cbuf_val(Field(iov_v,1));
    int ofs = Int_val(ofs_v);
    int len = Int_val(len_v);
    
    SKTTRACE(("skt_recv_iov\n"));
    ret = recv(sock, (char*)cbuf + ofs, len, 0) ;
    if (ret == -1) serror("skt_recv_iov");
    return Val_int(ret);
}


value skt_send_p(
    value sock_v,
    value buf_v,
    value ofs_v,
    value len_v
    )
{
    ocaml_skt_t sock;
    int len ;
    int ofs ;
    int ret ;
    char *buf ;
    
    SKTTRACE(("skt_send_p(\n"));
    sock = Socket_val(sock_v);
    ofs = Int_val(ofs_v) ;
    len = Int_val(len_v) ;
    buf = ((char*)&Byte(buf_v,ofs)) ;
    
    ret = send(sock, buf, len, 0) ;
    if (ret == -1) serror("skt_send_p");
    SKTTRACE((")\n"));
    return Val_int(ret) ;
}


value skt_sendto(
    value info_v,
    value buf_v,
    value ofs_v,
    value len_v
    )
{
    int len, ofs, i, ret =0;
    skt_sendto_info_t *info ;
    int naddr ;
    char *buf ;

    info = skt_Sendto_info_val(info_v);
    ofs = Int_val(ofs_v) ;
    len = Int_val(len_v) ;
    buf = ((char*)&Byte(buf_v,ofs)) ;
    SKTTRACE3(("skt_sendto(len=%d ofs=%d\n", len, ofs));
    
    naddr = info->naddr ;
    
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	ret = sendto(info->sock, buf, len, 0, &info->sa[i], info->addrlen) ;
	    
	SKTTRACE3(("skt_sendto, ret=%d (port=%d,addr=%s)\n",
		   ret, 
		   ntohs(((struct sockaddr_in*)&(info->sa[i]))->sin_port),
		   inet_ntoa(((struct sockaddr_in*)&(info->sa[i]))->sin_addr)
		      ));
	
	if (SOCKET_ERROR == ret) skt_udp_recv_error();
    }
    SKTTRACE3((")\n"));
    return Val_unit;
}


value skt_sendtov(
    value info_v,
    value iova_v
    )
{
    int naddr, i, ret=0, len=0, iovlen;
    skt_sendto_info_t *info ;
    ocaml_skt_t sock;
    
    info = skt_Sendto_info_val(info_v);
    iovlen = gather(iova_v) ;
    
    sock = info->sock ;
    naddr = info->naddr ;
    
    SKTTRACE(("skt_sendtov(\n"));
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	ret = WSASendTo(sock, iov, iovlen, &len, 0,
			&info->sa[i], info->addrlen, 0, NULL);
	if (SOCKET_ERROR == ret) skt_udp_recv_error();
    }
    
    SKTTRACE((")\n"));
    return Val_unit;
}

value skt_sendtosv(
    value info_v,
    value prefix_v,
    value ofs_v,
    value len_v,
    value iova_v
    )
{
    int naddr=0, i, ret=0, len=0, iovlen;
    ocaml_skt_t sock=0 ;
    skt_sendto_info_t *info ;
    
    SKTTRACE(("skt_sendtosv(\n"));
    info = skt_Sendto_info_val(info_v);
    iovlen = prefixed_gather(prefix_v, ofs_v, len_v, iova_v) ;
    
    sock = info->sock ;
    naddr = info->naddr ;
    
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	ret = WSASendTo(sock, iov, iovlen, &len, 0,
			&info->sa[i], info->addrlen, 0, NULL);
/*	{
	    int j;
	    struct sockaddr_in *addr = (struct sockaddr_in*) &info->sa[i];
	    
	    for(j=0; j<iovlen; j++) {
		SKTTRACE((" <iov[%d].len=%d> ", j, Iov_len(iov[j])));
	    }
	    SKTTRACE(("\n sock=%d  iovlen=%d sent_len=%d", sock, iovlen, ret));
	    SKTTRACE(("\n port=%d ip=%s", ntohs(addr->sin_port),
		      inet_ntoa(addr->sin_addr)));
		      }*/
	if (SOCKET_ERROR == ret) skt_udp_recv_error();
    }
    
    SKTTRACE((")\n"));
    return Val_unit;
}

value skt_tcp_recv_packet(
    value sock_v, 
    value buf_v, 
    value ofs_v,
    value len_v,
    value iov_v
    )
{
    int ret=0, len=0;
    DWORD flags=0;
    
    SKTTRACE(("skt_tcp_recv_packet(\n"));
    Iov_len(recv_iov[0]) =  Int_val(len_v);
    Iov_buf(recv_iov[0]) = (char*)&Byte(buf_v, Int_val(ofs_v)) ;
    Iov_len(recv_iov[1]) =  Int_val(Field(iov_v,0));
    Iov_buf(recv_iov[1]) = mm_Cbuf_val(Field(iov_v,1));
    
    SKTTRACE(("skt_tcp_recv_packet, len=%d, iov_len=%d (\n",  
	      Int_val(len_v), Int_val(Field(iov_v,0))));
    
    ret = WSARecv(Socket_val(sock_v), recv_iov, 2, &len, &flags , NULL, NULL);
    
    SKTTRACE(("recv_len=%d )\n", len));
    
    if (ret == -1) serror("skt_tcp_recv_packt");
    SKTTRACE((")\n"));
    return Val_int(len);
}

/**************************************************************/
/* Receive a packet, allocated a user buffer and read the data into
 * it. 
 *
 * The string is formatted as follows:
 * [len_buf] [len_iov] [actual ml data]
 
 * sock_v : a socket
 * cbuf_v : a memory pointer in which to write user date.
 *
 * Since NT4 and WIN2000 do not support MSG_PEEK, we need to
 * to receive everthing into the pre-allocated user-buffer.
 */
value skt_udp_recv_packet(
    value sock_v,
    value cbuf_v
    )
{
    int len =0, ml_len, usr_len;
    ocaml_skt_t sock;
    char *usr_buf  = NULL, *ml_buf = NULL;
    int ret = 0 ;
    int header_peek = (int) HEADER_PEEK;
    int flags = 0;
    char *cbuf = mm_Cbuf_val(cbuf_v);
    
    CAMLparam2(sock_v, cbuf_v);
    CAMLlocal3(buf_v, iov_v, ret_v);
    
    sock = Socket_val(sock_v);
    SKTTRACE(("udp_recv_packet(sock=%d\n", sock));
    
    /* On NT, PEEK does not work, we must read the whole packet.
     */
    len = recvfrom(sock, cbuf, MAX_MSG_SIZE, 0, NULL, 0);

    if (-1 == len) {
	SKTTRACE3(("<error= %d>", WSAGetLastError()));
	skt_udp_recv_error();
	goto dump_packet;
    }

    if (0 == len){
	SKTTRACE3(("<len=0>"));
	goto dump_packet;
    }

    /* Header is too short, read the rest of the packet and dump
     * it. 
     */
    SKTTRACE3(("<len=%d>", len));
    if (len < header_peek) {
	printf ("(NT) Packet too short.\n"); 
	goto dump_packet;
    }
    ml_len = ntohl (*(uint32*) &(cbuf[0]));
    usr_len = ntohl (*(uint32*) &(cbuf[SIZE_INT32]));
    
    SKTTRACE3(("SENDRECV:udp_recv_packet: ml_len=%d usr_len=%d\n", ml_len, usr_len));
    
    /* The headers read will be too long. This is a bogus
     * packet, dump it. 
     */
    if ((ml_len + usr_len) >= MAX_MSG_SIZE) {
	printf ("Hdr length too long.\n"); 
	goto dump_packet;
    }
    
    if (ml_len ==0 && usr_len ==0) {
	printf ("No msg body.\n"); 
	goto dump_packet;
    }
    
    /* Allocate an ML buffer.
     */
    if (ml_len > 0) {
	buf_v = alloc_string(ml_len);
	ml_buf = String_val(buf_v);
    }
    
    /* Place usr_buf at the start of user-data.
     */
    usr_buf = (char*)cbuf + HEADER_PEEK + ml_len;
    
    /* Copy the ML header.
     */
    memcpy(ml_buf, (char*)cbuf + HEADER_PEEK, ml_len);
    
    if (usr_len > 0)
	iov_v =  mm_Raw_of_len_buf(usr_len, usr_buf);
    else
	iov_v = mm_empty();
    
    ret_v = alloc_small(2, 0);
    Field(ret_v, 0) = buf_v;
    Field(ret_v, 1) = iov_v;
    
 ret:
    SKTTRACE3((")\n"));
    CAMLreturn(ret_v);
    
    /* PREF: Could precompute some of this stuff.
     */
 dump_packet:
    ret_v = alloc_small(2, 0);
    Field(ret_v, 0) = alloc_string(0);
    Field(ret_v, 1) = mm_empty ();
    if (len == -1) skt_udp_recv_error () ;
    SKTTRACE3(("dump_packet)\n"));
    goto ret;
}

value skt_recvfrom(value sock_v, value buf_v, value ofs_v, value len_v)
{
    CAMLparam4(sock_v, buf_v, ofs_v, len_v);
    CAMLlocal2(pair_v, addr_v);
    int ret;
    union sock_addr_union addr;
    socklen_param_type addr_len;
    int len = 0 /*, flags =0*/;
    
    SKTTRACE3(("skt_recvfrom(skt=%d  ofs=%d  len=%d  ",
	       Socket_val(sock_v), Long_val(ofs_v), Int_val(len_v)));

    ret = recvfrom(Socket_val(sock_v), &Byte(buf_v, Long_val(ofs_v)),
		   Int_val(len_v), 0, (struct sockaddr*) &addr.s_inet, &addr_len);
    
    if (SOCKET_ERROR == ret) {
	SKTTRACE3(("<Errno=%d>)\n", h_errno));
	serror("skt_recvfrom");
    }
    SKTTRACE3(("recv_len=%d)\n",ret));
    
    addr_v = alloc_sockaddr(&addr, addr_len);
    pair_v = alloc_small(2, 0);
    Field(pair_v, 0) = Val_int(ret);
    Field(pair_v, 1) = addr_v;
    CAMLreturn (pair_v);
}

value skt_recvfrom2(value sock_v, value buf_v, value ofs_v, value len_v)
{
    int ret;
    
    SKTTRACE(("skt_recvfrom2(\n"));
    ret = recvfrom(Socket_val(sock_v), &Byte(buf_v, Long_val(ofs_v)), Int_val(len_v),
		   0, NULL, NULL);
    if (SOCKET_ERROR == ret) serror("skt_recvfrom2");
    SKTTRACE(("len=%d)\n",ret));
    
    return (Val_int(ret));
}


value skt_sendv_p(
    value sock_v,
    value iova_v
    )
{
    int ret=0, len=0, iovlen=0;
    ocaml_skt_t sock;
    
    SKTTRACE(("skt_sendv_p(\n"));
    iovlen = gather_p(iova_v) ;
    sock = Socket_val(sock_v);
    ret = WSASend(sock, iov, iovlen, &len, 0, NULL, NULL);
    
    if (SOCKET_ERROR == ret) serror("skt_sendv_p");
    SKTTRACE((")\n"));
    return(Val_int(len));
}

value skt_sendsv_p(
    value sock_v,
    value prefix_v,
    value ofs_v,
    value len_v,
    value iova_v
    )
{
    int ret=0, len=0, iovlen=0;
    ocaml_skt_t sock=0 ;
    
    SKTTRACE(("skt_sendsv_p(\n"));
    iovlen = prefixed_gather_p(prefix_v, ofs_v, len_v, iova_v) ;
    sock = Socket_val(sock_v);
    ret = WSASend(sock, iov, iovlen, &len, 0, NULL, NULL);
    
    if (SOCKET_ERROR == ret) serror("skt_sendsv_p");
    SKTTRACE((")\n"));
    return (Val_int(len));
}

value skt_sends2v_p_native(
    value sock_v,
    value prefix1_v,
    value prefix2_v,
    value ofs2_v,
    value len2_v,
    value iova_v
    )
{
    int ret=0, len=0, iovlen=0;
    ocaml_skt_t sock=0 ;
    
    SKTTRACE(("skt_sends2v_p(\n"));
    iovlen = prefixed2_gather_p(prefix1_v, prefix2_v, ofs2_v, len2_v, iova_v) ;
    sock = Socket_val(sock_v);
    ret = WSASend(sock, iov, iovlen, &len, 0, NULL, NULL);
    
    if (SOCKET_ERROR == ret) serror("skt_sends2v_p");
    SKTTRACE((")\n"));
    return (Val_int(len));
}

value skt_sends2v_p_bytecode(value *argv, int argn){
    return skt_sends2v_p_native(argv[0], argv[1], argv[2], argv[3],
				argv[4], argv[5]);
}

/**************************************************************/





