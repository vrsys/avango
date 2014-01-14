/**************************************************************/
/* MM.C */
/* Author: Ohad Rodeh, 9/2001 */
/* The basic memory management functions. Used to manage user allocated */
/* buffer in C layout. */
/**************************************************************/
/* Instead of raising exceptions upon malloc failure, we return
 * empty iovecs.
 *
 * 
 */
/**************************************************************/
#include "skt.h"
/**************************************************************/

value mm_Val_cbuf (char *buf) {
    CAMLparam0();
    CAMLlocal1(cbuf_v);
    
    cbuf_v = alloc_small(1,Abstract_tag);
    Field(cbuf_v,0) = (value) buf;
    
    CAMLreturn(cbuf_v);
}

char *mm_Cbuf_val(value cbuf_v){
    return (char*) Field(cbuf_v,0);
}


value mm_cbuf_free(value cbuf_v){
    char *buf = mm_Cbuf_val(cbuf_v);
    if (buf != NULL) ce_intrn_free_msg_space(buf);
    return Val_unit;
}

/**************************************************************/
value mm_Raw_of_len_buf(int len, char *buf){
    CAMLparam0();
    CAMLlocal2(iov_v,cbuf_v);
    
    SKTTRACE(("mm_Raw_of_len_buf("));
    cbuf_v = mm_Val_cbuf(buf);
    iov_v = alloc_small(2,0);
    Field(iov_v,0) = Val_int(len);
    Field(iov_v,1) = cbuf_v;
    SKTTRACE((")\n"));
    
    CAMLreturn(iov_v);
}

/* An empty iovec.
 * Register it so that the Caml GC will be aware of it. 
 */
value empty_iovec = (value) NULL;

/* Initialize the empty iovec. The initialization function is called
 * by ssocket.
 */
value mm_empty (){
    CAMLparam0();
    if (empty_iovec == (value) NULL) {
	SKTTRACE(("mm_empty: setting mm_empty(\n"));
	empty_iovec =  mm_Raw_of_len_buf(0,NULL);
	SKTTRACE((".\n"));
	register_global_root(&empty_iovec);
	SKTTRACE((")\n"));
    }
    CAMLreturn(empty_iovec);
}

value mm_Val_raw (ce_iovec_t *iov){
    CAMLparam0();
    CAMLlocal2(cbuf_v, iov_v);
    
    SKTTRACE(("mm_Val_raw("));
    cbuf_v = mm_Val_cbuf(Iov_buf(*iov));
    iov_v = alloc_small(2,0);
    Field(iov_v,0) = Val_int(Iov_len(*iov));
    Field(iov_v,1) = cbuf_v;
    SKTTRACE((")\n"));
    
    CAMLreturn(iov_v);
}

value mm_copy_raw_into_string(value iov_v, value buf_v, value ofs_v){
    int len;
    char *cbuf;
    
    SKTTRACE2(("mm_copy_raw_into_string("));
    len = Int_val(Field(iov_v,0));
    cbuf = mm_Cbuf_val(Field(iov_v,1));
    SKTTRACE2(("raw= (ptr=%d,len=%d) (strlen=%d,ofs=%d)", (int)cbuf, len, string_length(buf_v), (int) Int_val(ofs_v)));
    memcpy(String_val(buf_v) + Int_val(ofs_v), cbuf, len);
    SKTTRACE2((")\n"));
    
    return Val_unit;
}

value mm_copy_string_into_raw(value str_v, value ofs_v, value len_v, value iov_v){
    char *dst;
    
    SKTTRACE(("mm_copy_string_into_raw("));
    dst = mm_Cbuf_val(Field(iov_v,1));
    memcpy(dst, (char*)String_val(str_v) + Int_val(ofs_v), Int_val(len_v));
    SKTTRACE((")\n"));
    
    return Val_unit;
}

value mm_copy_raw_into_raw(value iov_src_v, value iov_dst_v){
    int len;
    char *dst, *src;
    
    SKTTRACE(("mm_copy_raw_into_raw("));
    len = Int_val(Field(iov_src_v,0));
    assert(len!=0);
    src = mm_Cbuf_val(Field(iov_src_v,1));
    dst = mm_Cbuf_val(Field(iov_dst_v,1));
    memcpy(dst, src, len);
    SKTTRACE((")\n"));
    
    return Val_unit;
}

/* Here, we assume that [ofs] is indeed inside the iovec.
 */
value mm_cbuf_sub(value iov_v, value ofs_v){
    char *cbuf;
    CAMLparam2(iov_v, ofs_v);
    CAMLlocal1(cbuf_new_v);
    
    SKTTRACE(("mm_sub("));
    cbuf = mm_Cbuf_val(Field(iov_v,1));
    cbuf_new_v = mm_Val_cbuf((char*) cbuf + Int_val(ofs_v));
    SKTTRACE((")\n"));
    
    CAMLreturn(cbuf_new_v);
}

/* Flatten an array of iovectors. 
 * A iovec of the correct size is preallocated, and given by the
 * ML side. 
 */
value mm_flatten(value iova_v, value iov_v){
    int len, i, n, ofs;
    char *dst, *src;
    
    SKTTRACE(("mm_flatten:("));
    n = Wosize_val(iova_v);
    dst= mm_Cbuf_val(Field(iov_v,1));
    
    for (i=0, ofs=0; i<n; i++){
	len = Int_val(Field(Field(iova_v,i),0));
	src = mm_Cbuf_val(Field(Field(iova_v,i),1));
	memcpy((char*)dst + ofs, src, len);
	ofs += len;
    }
    SKTTRACE((")\n"));
    
    return Val_unit;
}

/**************************************************************/
/* Allocate an iovec
 */

value mm_alloc(value len_v){
    int len;
    char *buf;
    CAMLparam1(len_v);
    CAMLlocal2(iov_v, cbuf_v);
    
    SKTTRACE(("mm_alloc("));
    len = Int_val(len_v);

    if (len == 0) {
	buf = NULL;
    } else {
	buf = (char*) ce_intrn_alloc_msg_space(len);
    }
    
    if (len>0 && buf == NULL)
	raise_out_of_memory ();    
    
    cbuf_v = mm_Val_cbuf(buf);
    iov_v = alloc_small(2,0);
    Field(iov_v,0) = len_v;
    Field(iov_v,1) = cbuf_v;
    
    SKTTRACE((")\n"));
    CAMLreturn(iov_v);
}

/**************************************************************/
value mm_output_val(value obj_v, value iov_v, value flags_v){
    CAMLparam3(obj_v, iov_v, flags_v);
    
    int len = Int_val(Field(iov_v,0));
    char *cbuf = mm_Cbuf_val(Field(iov_v,1));
    int ret;
    
    SKTTRACE(("output_value_to_block, cbuf=%x, len=%d(", (int)cbuf, len));
    ret = output_value_to_block(obj_v, flags_v, cbuf, len);
    SKTTRACE((")\n"));
    CAMLreturn(Val_int(ret));
}

value mm_input_val(value iov_v){
    CAMLparam1(iov_v);
    CAMLlocal1(ret_v);
    int len = Int_val(Field(iov_v,0));
    char *cbuf = mm_Cbuf_val(Field(iov_v,1));
    
    SKTTRACE(("input_value_from_block, cbuf=%x, len=%d(",(int)cbuf, len));
    ret_v = input_value_from_block(cbuf, len);
    SKTTRACE((")\n"));
    CAMLreturn(ret_v);
}

/**************************************************************/
