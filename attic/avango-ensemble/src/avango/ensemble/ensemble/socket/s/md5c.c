/**************************************************************/
/* MD5C.C */
/* Authors: Ohad Rodeh, 11/97 */
/**************************************************************/
#include "skt.h"
/**************************************************************/
/* This is from byterun/md5.c.  We assume it will not change.
 */

struct MD5Context {
    uint32 buf[4];
    uint32 bits[2];
    unsigned char in[64];
};

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char *buf, unsigned int len);
void MD5Final(unsigned char *digest, struct MD5Context *ctx);



/**************************************************************/
/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to 
 * user initial key.
 * This is used in the HMAC standard.
 */
void MD5InitFull(struct MD5Context *ctx, char * init_key)
{
  memcpy((char*)ctx->buf, (char*)init_key, 16);
  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

/**************************************************************/

value 
skt_md5_context_length(
        value ignore
) {
    return Val_int(sizeof(struct MD5Context));
}

value 
skt_md5_init(
        value ctx_v
) {
    struct MD5Context *ctx;  
    ctx = (struct MD5Context*)String_val(ctx_v) ;
    MD5Init(ctx);
    return Val_unit ;
}

value 
skt_md5_init_full(
		  value ctx_v,
		  value init_key_v
) {
    struct MD5Context *ctx;  
    char * init_key ;
    ctx = (struct MD5Context*)String_val(ctx_v) ;
    init_key = String_val(init_key_v);
    MD5InitFull(ctx,init_key);
    return Val_unit ;
}

value 
skt_md5_update( 
        value ctx_v,
	value buf_v,
	value ofs_v,
	value len_v
) {
    struct MD5Context *ctx ;
    unsigned char *buf;
    unsigned int len ;

    ctx = (struct MD5Context*)String_val(ctx_v) ;
    buf = &Byte(buf_v,Long_val(ofs_v)) ;
    len = Long_val(len_v) ;

    MD5Update(ctx,buf,len);

    return Val_unit;
}
	
value 
skt_md5_update_iov( 
        value ctx_v,
	value iov_v
) {
    struct MD5Context *ctx ;
    unsigned char *buf;
    unsigned int len ;

    ctx = (struct MD5Context*)String_val(ctx_v) ;
    len = Int_val(Field(iov_v,0));
    buf = mm_Cbuf_val(Field(iov_v,1));
    MD5Update(ctx,buf,len);

    return Val_unit;
}
	


value 
skt_md5_final(
    value ctx_v,
    value digest_v
){
    unsigned char *digest;
    struct MD5Context *ctx ;

    digest = String_val(digest_v);
    ctx = (struct MD5Context*)String_val(ctx_v);
    MD5Final(digest,ctx);
    return Val_unit ;
}

