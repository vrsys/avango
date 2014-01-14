/**************************************************************/
/* SOCKFD.H */
/* Author: Ohad Rodeh 11/2001 */
/**************************************************************/
/* Interfacing sockets between C and ML.
 */

#ifndef __SOCKFD_H__
#define __SOCKFD_H__

typedef int ocaml_skt_t ;

/* Get the file descriptor inside the wrapper.
 */
#define Socket_val(sock_v) (Int_val(sock_v))
#define Val_socket(sock)   (Val_int(sock))

#endif
