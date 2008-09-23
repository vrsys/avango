/**************************************************************/
/* SKT.H */
/* Authors: Mark Hayden, Robbert van Renesse, 8/97 */
/* Cleanup: Ohad Rodeh 10/2002 */
/**************************************************************/
#ifndef __PLATFROM_H__
#define __PLATFORM_H__

#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#ifndef hpux
#include <sys/socket.h>
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#define h_errno errno

union sock_addr_union {
  struct sockaddr s_gen;
  struct sockaddr_un s_unix;
  struct sockaddr_in s_inet;
};

#endif

/**************************************************************/
