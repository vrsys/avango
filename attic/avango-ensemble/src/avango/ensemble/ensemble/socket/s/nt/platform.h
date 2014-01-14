/**************************************************************/
/* SKT.H */
/* Authors: Mark Hayden, Robbert van Renesse, 8/97 */
/* Cleanup: Ohad Rodeh 10/2002 */
/**************************************************************/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <winsock2.h>
#include <io.h>

union sock_addr_union {
  struct sockaddr s_gen;
  struct sockaddr_in s_inet;
};

#endif

/**************************************************************/
