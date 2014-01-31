/**************************************************************/
/* SENDRECV.C */
/* Author: Ohad Rodeh  9/2001 */
/* This is a complete rewrite of previous code by Mark Hayden */
/**************************************************************/
/* The code has been placed in an H file because some C compilers
 * respect the INLINE declaration only for H files.
 */
/**************************************************************/
#include "skt.h"
/**************************************************************/
/* BUG:
 * This MUST be the same as Buf.max_msg_len
 */
#define MAX_MSG_SIZE 8*1024

#define SIZE_INT32 sizeof(uint32)

/* Peeking to read the first two integers.  This is signed (versus
 * unsigned) because we are comparing against the received length to
 * see if enough bytes have been received from the packet.
 */
#define HEADER_PEEK   ((int32)(2 * SIZE_INT32))

#define N_IOVS 100
extern ce_iovec_t iov[N_IOVS] ;
extern ce_iovec_t recv_iov[3] ;
extern char peek_buf[HEADER_PEEK]; 

/* This is used in the flat versions
 */
extern char iobuf[MAX_MSG_SIZE];
extern int iobuf_len;

/**************************************************************/

/* A structure containing pre-processed information for
 * sending on a socket to several destinations. 
 */
typedef struct skt_sendto_info_t {
    int sock ;
    int naddr ;
    int addrlen ;
  /* jsd: chaned from sa[0] to sa[1] to avoid error by mipspro */
    struct sockaddr sa[1]; /* An array of addresses comes here. 
			    * Since we do not know its size, we 
			    * cannot pre-allocate it. 
			    */
} skt_sendto_info_t ;

#define IP_ADDR_LEN (sizeof(struct sockaddr_in))

/**************************************************************/

skt_sendto_info_t *skt_Sendto_info_val(value info_v){
    return (skt_sendto_info_t*) Field(info_v,1);
}

void skt_Sendto_info_free(value info_v){
    free(skt_Sendto_info_val(info_v));
}

value skt_Val_create_sendto_info(
    value sock_v,
    value sina_v
    ) {
    skt_sendto_info_t *info ;
    ocaml_skt_t sock ;
    int naddr ;
    int i ;
    union sock_addr_union sock_addr ;
    int sock_addr_len ; 
    int size;
    CAMLparam2(sock_v, sina_v) ;
    CAMLlocal1(info_v) ;
    
    sock = Socket_val(sock_v) ;
    naddr = Wosize_val(sina_v) ;
    size = sizeof(skt_sendto_info_t) + naddr * sizeof(struct sockaddr);
    
    info = (skt_sendto_info_t*) malloc(size);
    memset((char*)info, 0, size);
    
    info->sock  = sock ;
    info->naddr = naddr ;
    info->addrlen = IP_ADDR_LEN ;	/* always IP_ADDR_LEN */
    
    for(i=0;i<naddr;i++) {
	get_sockaddr(Field(sina_v,i), &sock_addr, &sock_addr_len) ;
	assert(sock_addr_len == IP_ADDR_LEN) ;
	memcpy(&info->sa[i], &sock_addr, IP_ADDR_LEN) ;
    }
    info_v = alloc_final(2, skt_Sendto_info_free, 1, sizeof(skt_sendto_info_t)) ; 
    Field(info_v, 1) =  (value) info ; 
    CAMLreturn(info_v) ;
}

/**************************************************************/
/* Common exported buffers.
 */
ce_iovec_t iov[N_IOVS] ;
ce_iovec_t recv_iov[3] ;
char peek_buf[HEADER_PEEK]; 

/* This is used in the flat versions
 */
char iobuf[MAX_MSG_SIZE];
int iobuf_len;

/**************************************************************/

INLINE
static int iovl_len(value iova_v){
    int i, nvecs, len=0;
    value t_v, iov_v;
    
    nvecs = Wosize_val(iova_v) ;	
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ; // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;  // Extract Basic_iov.iovec
	len += Int_val(Field(iov_v,0));
    }
    
    return len;
}

INLINE
static void prepare_send_header(value len_v, value iova_v){
    *((int*)((char*)peek_buf)) = htonl(Int_val(len_v));
    *((int*)((char*)peek_buf + SIZE_INT32)) = htonl(iovl_len(iova_v));
}

/* Add a header describing the length of the ML/Iovec parts.
 */
int gather(value iova_v){
    value t_v, iov_v;
    int i, nvecs, iovlen ;
    int len;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ;	
    assert(nvecs +1 <= N_IOVS) ;
    iovlen  = nvecs +1;
    
    /* Write the ML/Iovec length.
     */
    prepare_send_header(Val_int(0), iova_v);
    Iov_len(iov[0]) = HEADER_PEEK;
    Iov_buf(iov[0]) = peek_buf;
    
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ; // Extract Basic_iov.t
	iov_v = Field(t_v,1) ; // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_len(iov[i+1])  = len ;
	Iov_buf(iov[i+1]) = buf ;
    }
    
    return iovlen;
}


int prefixed_gather(value prefix_v, value ofs_v, value len_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, iovlen;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ; /* inspired by array.c */
    assert(nvecs + 2 <= N_IOVS) ;
    iovlen = nvecs + 2 ;
    
    /* Write the ML/Iovec length.
     */
    prepare_send_header(len_v, iova_v);
    Iov_len(iov[0]) = HEADER_PEEK;
    Iov_buf(iov[0]) = peek_buf;
    
    /* Fill in for the prefix. 
     */
    Iov_len(iov[1])  = Int_val(len_v);
    Iov_buf(iov[1]) = (char*) &Byte(prefix_v,Int_val(ofs_v)) ;
    
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ; // Extract Basic_iov.t
	iov_v = Field(t_v,1) ; // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_buf(iov[i+2]) = buf ;
	Iov_len(iov[i+2])  = len ;
    }
    
    return iovlen;
}

int gather_p(value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, iovlen ;
    int len;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ;	
    assert(nvecs <= N_IOVS) ;
    iovlen  = nvecs ;
    
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;   // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_len(iov[i]) = len ;
	Iov_buf(iov[i]) = buf ;
    }
    return iovlen;
}

int prefixed_gather_p(value prefix_v, value ofs_v, value len_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, iovlen;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ; /* inspired by array.c */
    assert(nvecs + 1 <= N_IOVS) ;
    iovlen = nvecs + 1 ;
    
    /* Fill in for the prefix. 
     */
    Iov_len(iov[0])  = Int_val(len_v);
    Iov_buf(iov[0]) = (char*) &Byte(prefix_v,Int_val(ofs_v)) ;
    
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;   // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_len(iov[i+1])  = len ;
	Iov_buf(iov[i+1]) = buf ;
    }
    return iovlen;
}



int prefixed2_gather_p(value prefix1_v, value prefix2_v, value ofs2_v,
		       value len2_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, iovlen;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ; /* inspired by array.c */
    assert(nvecs + 2 <= N_IOVS) ;
    iovlen = nvecs + 2 ;
    
    /* Fill in for the prefix.
     */
    Iov_len(iov[0]) = string_length(prefix1_v) ;
    Iov_buf(iov[0]) = String_val(prefix1_v) ;
    Iov_len(iov[1]) = Int_val(len2_v) ;
    Iov_buf(iov[1]) = (char*) &Byte(prefix2_v, Int_val(ofs2_v)) ;
    
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;     // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_len(iov[i+2])  = len ;
	Iov_buf(iov[i+2]) = buf ;
    }
    return iovlen;
}

/**************************************************************/
/* The flat versions come here.
 */
void gather_p_flat(value iova_v)
{
    value t_v, iov_v;
    int i, nvecs;
    int len, ofs;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ;	
    assert(nvecs <= N_IOVS) ;
    
    ofs =0;
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;   // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	
	Iov_len(iov[i]) = len ;
	Iov_buf(iov[i]) = buf ;
	memcpy((char*)iobuf + ofs, buf, len);
	ofs +=len;
    }
    /* Set the length of the "returned" buffer.
     */
    iobuf_len = ofs;
}

void prefixed_gather_p_flat(value prefix_v, value ofs_v, value len_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, iovlen, ofs, len1;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ; /* inspired by array.c */
    assert(nvecs + 1 <= N_IOVS) ;
    iovlen = nvecs + 1 ;
    
    /* Fill in for the prefix. 
     */
    len1 = Int_val(len_v);
    memcpy(iobuf, (char*) &Byte(prefix_v,Int_val(ofs_v)), len1);
    
    ofs = len1;
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;   // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	memcpy((char*)iobuf + ofs, buf, len);
	ofs +=len;
    }
    /* Set the length of the "returned" buffer.
     */
    iobuf_len = ofs;
}

void prefixed_gather_flat(value prefix_v, value ofs_v, value len_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, iovlen, ofs, len1;
    char *buf;

    /* Prepare the ML header
     */
    *((int*)&iobuf[0]) = htonl(Int_val(len_v));
    *((int*)&iobuf[SIZE_INT32]) = htonl(iovl_len(iova_v));
    ofs = SIZE_INT32 * 2;

    nvecs = Wosize_val(iova_v) ; /* inspired by array.c */
    assert(nvecs + 1 <= N_IOVS) ;
    iovlen = nvecs + 1 ;

    /* Fill in for the prefix. 
     */
    len1 = Int_val(len_v);
    memcpy(iobuf + ofs, (char*) &Byte(prefix_v,Int_val(ofs_v)), len1);
    
    ofs += len1;
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;   // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.iovec
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	memcpy((char*)iobuf + ofs, buf, len);
	ofs +=len;
    }
    
    /* Set the length of the "returned" buffer.
     */
    iobuf_len = ofs;
}

void prefixed2_gather_p_flat(value prefix1_v, value prefix2_v, value ofs2_v, value len2_v, value iova_v)
{
    value t_v, iov_v;
    int i, nvecs, len, len1, len2, ofs=0;
    char *buf;
    
    nvecs = Wosize_val(iova_v) ; 
    assert(nvecs + 2 <= N_IOVS) ;
    
    len1 = string_length(prefix1_v);
    memcpy(iobuf, String_val(prefix1_v), len1);
    len2 = Int_val(len2_v);
    memcpy((char*)iobuf + len1,(char*) &Byte(prefix2_v, Int_val(ofs2_v)), len2);
    
    ofs = len1+len2;
    for(i=0;i<nvecs;i++) {
	t_v = Field(iova_v,i) ;     // Extract Basic_iov.t
	iov_v = Field(t_v,1) ;      // Extract Basic_iov.raw
	len   = Int_val(Field(iov_v,0)) ;
	buf   = mm_Cbuf_val(Field(iov_v,1));
	memcpy((char*)iobuf + ofs, buf, len);
	ofs +=len;
    }
    /* Set the length of the "returned" buffer.
     */
    iobuf_len = ofs;
}
/**************************************************************/

