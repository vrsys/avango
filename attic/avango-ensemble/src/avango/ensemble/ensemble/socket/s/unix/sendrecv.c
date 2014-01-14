/**************************************************************/
/* SENDRECV.C */
/* Author: Ohad Rodeh  9/2001 */
/* This is a complete rewrite of previous code by Mark Hayden */
/**************************************************************/
#include "skt.h"
#include "prefix_suffix.h"
/**************************************************************/
/* Receive a packet, allocated a user buffer and read the data into
 * it. ML has a preallocated buffer. Return Some(iov) on success,
 * None on failure. 
 *
 * No allocation on the ML heap is performed here. 
 * No exceptions are raised. 
 *
 * The ML string is preallocated, it looks like the following:
 *
 * The string is formatted as follows:
 * [len_buf] [len_iov] [actual ml data]
 */

static struct msghdr recv_msghdr = {
    NULL,/*(caddr_t) &sin,*/
    0,/*sizeof(sin),*/
    recv_iov,
    3,
    NULL,
    0
} ;

/* Try to read a packet from the network.
 * sock_v : a socket
 * cbuf_v : a memory pointer in which to write user date.
 */
value skt_udp_recv_packet(
    value sock_v,
    value cbuf_v
    )
{
    int len =0, ml_len, usr_len;
    ocaml_skt_t sock;
    char *usr_buf  = NULL, *ml_buf = NULL;
    CAMLparam2(sock_v, cbuf_v);
    CAMLlocal3(buf_v, iov_v, ret_v);
    
    SKTTRACE2(("Udp_recv_packet(\n"));
    sock = Socket_val(sock_v);
    
    /* Peek to read the headers. (8 Bytes, = 2 UINT32)
     */
    
    *(uint32*) &(peek_buf[0]) = 0 ;
    *(uint32*) &(peek_buf[SIZE_INT32]) = 0 ;
    
    len = recv(sock, peek_buf, HEADER_PEEK, MSG_PEEK);
    
    if(len < 0) {
	perror(" recv(sock, peek_buf, HEADER_PEEK, MSG_PEEK); ");
	SKTTRACE2((" errno=%d\n", errno));
	goto dump_packet ;
    }
    
    SKTTRACE2(("SENDRECV:recv (%d, peek), len=%d\n", HEADER_PEEK, len));
    
    /* Header is too short, read the rest of the packet and dump
     * it. 
     */
    
    if (len < HEADER_PEEK) {
	fprintf (stderr, "Packet too short.\n"); 
	len = recv(sock,  peek_buf, HEADER_PEEK, 0);
	SKTTRACE2(("SENDRECV:recv (%d, 0), len=%d\n", HEADER_PEEK, len));
	goto dump_packet;
    }
    
    ml_len = ntohl (*(uint32*) &(peek_buf[0]));
    usr_len = ntohl (*(uint32*) &(peek_buf[SIZE_INT32]));
    
    SKTTRACE(("SENDRECV:udp_recv: ml_len=%d usr_len=%d\n", ml_len, usr_len));
    
    /* The headers read will be too long. This is a bogus
     * packet, dump it. 
     */
    if ((ml_len + usr_len) >= MAX_MSG_SIZE) {
	printf ("Hdr length too long.\n"); 
	len = recv(sock,  peek_buf, HEADER_PEEK, 0);
	goto dump_packet;
    }
    
    if (ml_len ==0 && usr_len ==0)
    {
	printf ("No msg body.\n"); 
	len = recv(sock,  peek_buf, HEADER_PEEK, 0);
	goto dump_packet;
    }
    
    //  {
    //    int i;
    //    printf("recv_scribble --- ");
    //    for(i=0; i<HEADER_PEEK; i++)
    //      printf("%d:", ctx->recv_hd[i]);
    //    printf("\n");
    //
    //    printf("ml_len=%d, msg_len=%d\n", ml_len, usr_len);
    //  }
    
    
    /* Preparing the receiving IO-vector.
     * PERF: The 0'th buffer could be precomputed. 
     */
    
    Iov_len(recv_iov[0]) =  HEADER_PEEK ;
    Iov_buf(recv_iov[0]) = peek_buf ;
    
    /* Allocate an ML buffer.
     */
    recv_msghdr.msg_iovlen  = 3;
    if(ml_len > 0) {
	buf_v = alloc_string(ml_len);
	ml_buf = String_val(buf_v);
    }
    
    /* The user-buffer has already been allocated, no
     * need to allocate it. 
     */
    usr_buf = mm_Cbuf_val(cbuf_v);
    
    
    Iov_len(recv_iov[1]) =  ml_len ;
    Iov_buf(recv_iov[1]) = ml_buf ;
    Iov_len(recv_iov[2]) =  usr_len ;
    Iov_buf(recv_iov[2]) = usr_buf ;
    
    SKTTRACE2(("SENDRECV: ml buffer=%ld\n", string_length(buf_v)));
    
    /*  
	printf("iovlen=%d\n", ctx->recv_msghdr.msg_iovlen);
	printf("total_len=%d\n", 
	ctx->recv_msghdr.msg_iov[0].iov_len + 
	ctx->recv_msghdr.msg_iov[1].iov_len + 
	ctx->recv_msghdr.msg_iov[2].iov_len);
    */
    
    len = recvmsg(sock, &recv_msghdr, 0) ;   
    if (len != HEADER_PEEK + ml_len + usr_len) {
	printf("len2=%d, (%d) bad packet length, dumping the packet\n",
	       len,
	       HEADER_PEEK + ml_len + usr_len);
	goto dump_packet;
    }
    
    if (usr_len > 0)
	iov_v =  mm_Raw_of_len_buf(usr_len, usr_buf);
    else
	iov_v = mm_empty();
    
    ret_v = alloc_small(2, 0);
    Field(ret_v, 0) = buf_v;
    Field(ret_v, 1) = iov_v;
    
 ret:
    SKTTRACE2((")\n"));
    CAMLreturn(ret_v);
    
    /* PREF: Could precompute some of this stuff.
     */
 dump_packet:
    fprintf(stderr, "dump_packet)\n");
    ret_v = alloc_small(2, 0);
    Field(ret_v, 0) = alloc_string(0);
    Field(ret_v, 1) = mm_empty ();
    if (len == -1) skt_udp_recv_error () ;
    // printf("dump_packet)\n"); fflush (stdout);
    goto ret;
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
    
    naddr = info->naddr ;
    
    SKTTRACE2(("skt_sendto\n"));
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	ret = sendto(info->sock, buf, len, 0, &info->sa[i], info->addrlen) ;
    }
    return Val_unit;
}


/**************************************************************/

static struct msghdr send_msghdr = {
    NULL,/*(caddr_t) &sin,*/
    0,/*sizeof(sin),*/
    iov,
    0,
    NULL,
    0
} ;

/**************************************************************/

value skt_sendtov(
    value info_v,
    value iova_v
    )
{
    int naddr, i, ret=0;
    skt_sendto_info_t *info ;
    ocaml_skt_t sock;
    
    info = skt_Sendto_info_val(info_v);
    send_msghdr.msg_iovlen = gather(iova_v);
    
    send_msghdr.msg_namelen = info->addrlen ;
    sock = info->sock ;
    naddr = info->naddr ;
    
    SKTTRACE2(("skt_sendtov\n"));
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	send_msghdr.msg_name = (char*) &info->sa[i] ;
	ret = sendmsg(sock, &send_msghdr, 0) ;
    }
    
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
    int naddr=0, i, ret=0;
    ocaml_skt_t sock=0 ;
    skt_sendto_info_t *info ;
    
    info = skt_Sendto_info_val(info_v);
    send_msghdr.msg_iovlen = prefixed_gather(prefix_v, ofs_v, len_v, iova_v); 
    
    send_msghdr.msg_namelen = info->addrlen ;
    sock = info->sock ;
    naddr = info->naddr ;
    
    for (i=0;i<naddr;i++) {
	/* Send the message.  Assume we don't block or get interrupted.  
	 */
	send_msghdr.msg_name = (char*) &info->sa[i] ;
	ret = sendmsg(sock, &send_msghdr, 0) ;
    }
    
    /*  {
	int ilen = iovl_len(iova_v);
	///struct sockaddr_in *sa = (struct sockaddr_in*)&info->sa[0];
	
	if (ilen > 0) {
	printf("SENDRECV:sendtosv: (len=%d) (iovec=%d)\n", 
	Int_val(len_v), ilen);
	//printf("trg: IP=%s port=%d\n",
	//inet_ntoa(sa->sin_addr),
	//ntohs(sa->sin_port));
	}
	}*/
    
    return Val_unit;
}

/**************************************************************/
/* TCP stuff goes here.
 * 
 * Exceptions are thrown, and the length acually sent matters.
 * This is very similar to the above functions for UDP. 
 */

/* Read from a TCP socket into a string.
 */
value skt_recv(value sock, value buff, value ofs, value len) /* ML */
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
value skt_recv_iov(value sock_v, value iov_v, value ofs_v, value len_v) /* ML */
{
    int ret;
    ocaml_skt_t sock = Socket_val(sock_v);
    char* cbuf = mm_Cbuf_val(Field(iov_v,1));
    int ofs = Int_val(ofs_v);
    int len = Int_val(len_v);
    
    SKTTRACE(("skt_recv_iov\n"));
    ret = recv(sock, (char*)cbuf + ofs, len, 0) ;
    if (ret == -1) serror("recv_iov");
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
    
    sock = Socket_val(sock_v);
    ofs = Int_val(ofs_v) ;
    len = Int_val(len_v) ;
    buf = ((char*)&Byte(buf_v,ofs)) ;
    
    ret = send(sock, buf, len, 0) ;
    if (ret == -1) serror("send_p");
    return Val_int(ret) ;
}

value skt_sendv_p(
    value sock_v,
    value iova_v
    )
{
    int ret=0;
    ocaml_skt_t sock;
    
    
    send_msghdr.msg_iovlen = gather_p(iova_v) ;
    send_msghdr.msg_namelen = 0; 
    sock = Socket_val(sock_v);
    send_msghdr.msg_name = NULL;
    
    ret = sendmsg(sock, &send_msghdr, 0) ;
    if (ret == -1) serror("sendv_p");
    return(Val_int(ret));
}

value skt_sendsv_p(
    value sock_v,
    value prefix_v,
    value ofs_v,
    value len_v,
    value iova_v
    )
{
    int ret=0;
    ocaml_skt_t sock=0 ;
    
    send_msghdr.msg_iovlen = prefixed_gather_p(prefix_v, ofs_v, len_v, iova_v) ;
    send_msghdr.msg_namelen = 0;
    sock = Socket_val(sock_v);
    send_msghdr.msg_name = NULL;
    
    ret = sendmsg(sock, &send_msghdr, 0) ;
    if (ret == -1) serror("sendsv_p");
    return (Val_int(ret));
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
    int ret=0;
    ocaml_skt_t sock=0 ;
    
    
    send_msghdr.msg_iovlen = prefixed2_gather_p(prefix1_v, prefix2_v, ofs2_v, len2_v, iova_v) ;
    sock = Socket_val(sock_v);
    send_msghdr.msg_namelen = 0; 
    send_msghdr.msg_name = NULL;
    
    ret = sendmsg(sock, &send_msghdr, 0) ;
    if (ret == -1) serror("sends2v_p");
    return (Val_int(ret));
}

value skt_sends2v_p_bytecode(value *argv, int argn)
{
    return skt_sends2v_p_native(argv[0], argv[1], argv[2], argv[3],
				argv[4], argv[5]);
}

/**************************************************************/
value skt_tcp_recv_packet(
    value sock_v, 
    value buf_v, 
    value ofs_v,
    value len_v,
    value iov_v
    )
{
    int len;
    
    recv_msghdr.msg_namelen = 0; 
    recv_msghdr.msg_name = NULL;
    recv_msghdr.msg_iovlen  = 2;
    Iov_len(recv_iov[0]) =  Int_val(len_v);
    Iov_buf(recv_iov[0]) = (char*)&Byte(buf_v, Int_val(ofs_v)) ;
    Iov_len(recv_iov[1]) =  Int_val(Field(iov_v,0));
    Iov_buf(recv_iov[1]) = mm_Cbuf_val(Field(iov_v,1));
    
    SKTTRACE(("skt_tcp_recv_packet, len=%d, iov_len=%d (\n",  
	      Int_val(len_v), Int_val(Field(iov_v,0))));
    len = recvmsg(Socket_val(sock_v), &recv_msghdr, 0);   
    SKTTRACE(("recv_len=%d )\n", len));
    
    if (len == -1) serror("skt_tcp_recv_packet");
    return Val_int(len);
}

/**************************************************************/
