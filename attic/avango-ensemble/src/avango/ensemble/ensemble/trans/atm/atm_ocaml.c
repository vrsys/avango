#include <assert.h>
#include "mlvalues.h"
#include "atm.h"

value ens_atm_init(void) {
  atm_init() ;
  return Val_unit ;
}

value ens_atm_activate(
        value vci_v
) {
  atm_activate(Int_val(vci_v)) ;
  return Val_unit ;
}

value ens_atm_send(
        value vcis_v,
	value buf_v,
	value ofs_v,
	value len_v
) {
  int vci ;
  char *buf ;
  int ofs ;
  int len ;
  int n ;
  int i ;

  ofs = Int_val(ofs_v) ;
  len = Int_val(len_v) ;
  buf = &Byte(buf_v, ofs) ;

  n = Wosize_val(vcis_v) ; /* inspired by array.c */
  for(i=0;i<n;i++) {
    vci = Int_val(Field(vcis_v, i)) ;
    atm_send(vci, buf, len) ;
  }

  return Val_unit ;
}
  
value ens_atm_recv(
        value buf_v,
	value ofs_v,
	value len_v
) {
  int vci ;
  char *buf ;
  int ofs ;
  int len ; 
  int ret ;
  
  ofs = Int_val(ofs_v) ;
  len = Int_val(len_v) ;
  buf = &Byte(buf_v, ofs) ;
  ret = atm_recv(&vci, buf, len) ;
  return Val_int(ret) ;
}
