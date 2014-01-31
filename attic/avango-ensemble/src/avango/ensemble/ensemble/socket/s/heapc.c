/**************************************************************/
/* HEAPC.C: support for analyzing memory usage. */
/* Author: Mark Hayden, 10/97 */
/**************************************************************/
#include "skt.h"
/**************************************************************/

/* Begin: from minor_gc.h */
extern char *young_start, *young_ptr, *young_end, *young_limit;
extern char *heap_start ;
/* End: from minor_gc.h */


extern value caml_gc_full_major(value v) ;

typedef struct {
  asize_t size;
  char *next;
} heap_chunk_head;

#define Chunk_size(c) (((heap_chunk_head *) (c)) [-1]).size
#define Chunk_next(c) (((heap_chunk_head *) (c)) [-1]).next
#define Next(hp) ((hp) + Bhsize_hp (hp))

#define White (0 << 8)
#define Gray  (1 << 8)
#define Blue  (2 << 8)
#define Black (3 << 8)
#define Color_hd(hd) ((color_t) ((hd) & Black))
#define Color_hp(hp) Color_hd (Hd_hp (hp))

static void for_all_live(void (*f)(value)) {
  char *chunk ;
  char *chunk_end ;
  char *cur_hp ;
  header_t cur_hd ;

  for (chunk = heap_start ;
       chunk ;
       chunk = Chunk_next(chunk)) {
    chunk_end = chunk + Chunk_size(chunk) ;
    for (cur_hp = chunk ;
	 cur_hp < chunk_end ;
	 cur_hp = Next(cur_hp)) {
      cur_hd = Hd_hp(cur_hp) ;
      switch(Color_hd(cur_hd)) {
      case Blue : break ;
      case White :
	if (Wosize_hd (cur_hd) == 0)
	  break ;
      default:
	f(Val_hp(cur_hp)) ;
      }
    }
  }
}  

static unsigned int nlive ;
static unsigned int next ;
static value heap_v ;

static void count_live(value ignored) {
  nlive ++ ;
}

static void ens_add_to_heap(value obj_v) {
  assert(nlive == Wosize_val(heap_v)) ;
  if (heap_v != obj_v) {
    assert(next >= 0) ;
    assert(next < Wosize_val(heap_v)) ;
    /*printf("HEAPC:%d of %d:tag=%d:size=%d\n",next,nlive,Tag_val(obj_v),Wosize_val(obj_v)) ; fflush(stdout) ;*/
    Field(heap_v,next) = obj_v ;
    next++ ;
  }
}  

value skt_heap(void) {
  unsigned int i ;
  caml_gc_full_major(Val_unit) ;
  
  /* Scan major heap.
   */
  nlive = 0 ;
  for_all_live(count_live) ;

  heap_v = alloc_shr(nlive,0) ;
  for(i=0;i<nlive;i++) {
    Field(heap_v,i) = Val_unit ;
  }

  /* Scan major heap.
   */
  next = 0 ;
  for_all_live(ens_add_to_heap) ;
  assert(next == nlive) ;
  
  return heap_v ;
}

#ifdef CODE_OF_CLOSURE_DIFF
value code_of_closure(value clos_v) {
  value ret_v ;
  code_t code ;

  assert(Tag_val(clos_v) == Closure_tag) ;
#ifdef NATIVE_CODE
  {
    char addr_buf[100] ;
    value tmp_v ;
    Begin_root(tmp_v) ;
    tmp_v = Val_unit ;

    code = Code_val(clos_v) ;
    sprintf(addr_buf,"%08lx",(unsigned long)code) ;
    tmp_v = copy_string(addr_buf) ;
    ret_v = alloc(1,1) ;
    Field(ret_v,0) = tmp_v ;
  }    
#else
  {
    extern code_t start_code;
    extern asize_t code_size;
    code = Code_val(clos_v) ;
    assert(code >= start_code && code < start_code + code_size) ;
    ret_v = alloc(1,0) ;
    Field(ret_v,0) = code - start_code ;
  }
#endif
  return ret_v ;
}
#endif

value skt_code_of_closure(value clos_v) {
  code_t code ;
  char addr_buf[100] ;
  
  assert(Tag_val(clos_v) == Closure_tag) ;
  code = Code_val(clos_v) ;
  sprintf(addr_buf,"%08lx",(unsigned long)code) ;
  return copy_string(addr_buf) ;
}

value skt_addr_of_obj(value obj_v) {
  char addr_buf[100] ;
  sprintf(addr_buf,"%08lx",(unsigned long)obj_v) ;
  return copy_string(addr_buf) ;
}

extern long caml_stat_minor_words ; /* gc_ctrl.c */

/* This code is extracted from gc_ctrl.c */
value skt_minor_words(void) {
  long ret ;
  ret = caml_stat_minor_words + Wsize_bsize (young_end - young_ptr) ;
  return Val_long(ret) ;
}  

#if 0 /*defined(NATIVE_CODE)*/
/* From asmrun/roots.c */
typedef struct {
  unsigned long retaddr;
  short frame_size;
  short num_live;
  short live_ofs[1];
} frame_descr;

extern long * caml_frametable[];

value skt_frame_descriptors(value ignore_v) {
  int i,j,k,l ;
  long *tbl ;
  int len ;
  frame_descr *d ;
  value ret_v ;
  int ndescr ;
  CAMLparam0() ;
  CAMLlocal1(ret_v) ;

  /* Count the # of descriptors.
   */
  ndescr = 0 ;
  for (i = 0; caml_frametable[i]; i++)
    ndescr += *(caml_frametable[i]) ;

  /* Allocate and clear the array.
   */
  ret_v = alloc_shr(ndescr, 0) ;
  for (i=0;i<ndescr;i++) {
    Field(ret_v,i) = Val_unit ;
  }
  
  k = 0 ;
  for (i=0; caml_frametable[i];i++) {
    tbl = caml_frametable[i] ;
    len = *tbl ;
    d = (frame_descr*)(tbl+1) ;
    for (j=0;j<len;j++) {
      Field(ret_v,k) = alloc_shr(2 + d->num_live, 0) ;
      Field(Field(ret_v,k),0) = Val_long(d->frame_size) ;
      Field(Field(ret_v,k),1) = Val_long(d->num_live) ;
      for (l=0;l<d->num_live;l++) {
	  Field(Field(ret_v,k),l+2) = Val_long(d->live_ofs[l]) ;
      }
      d = (frame_descr *)
	  (((unsigned long)d +
	    sizeof(char *) + sizeof(short) + sizeof(short) +
	    sizeof(short) * d->num_live + sizeof(frame_descr *) - 1)
	   & -sizeof(frame_descr *));
      k++ ;
    }
  }
  assert(k == ndescr) ;
  
  CAMLreturn(ret_v) ;
}      

#else

value skt_frame_descriptors(value ignore) {
  return Atom(0) ;
}

#endif
