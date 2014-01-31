/**************************************************************/
/* SOCKFD.H */
/* Author: Ohad Rodeh 11/2001 */
/**************************************************************/
/* Interfacing sockets between C and ML.
 */

#ifndef __SOCKFD_H__
#define __SOCKFD_H__

typedef SOCKET ocaml_skt_t ;

/* from unixsupport.h */
struct filedescr {
  union {
    HANDLE handle;
    SOCKET socket;
  } fd;
  enum { KIND_HANDLE, KIND_SOCKET } kind;
};

extern value win_alloc_socket(SOCKET sock);

#define Val_socket(sock) (win_alloc_socket(sock))
#define Socket_val(v) (((struct filedescr *) Data_custom_val(v))->fd.socket)

#endif
