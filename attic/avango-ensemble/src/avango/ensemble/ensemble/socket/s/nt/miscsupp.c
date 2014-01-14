/**************************************************************/
/* MISCSUPP.C: miscelaneous support functions */
/* Author: Mark Hayden, 10/97 */
/* Changes: Ohad Rodeh 10/2002 */
/**************************************************************/
#include "skt.h"
/**************************************************************/

value
win_version(){
    DWORD dwVersion, MajorVersion, MinorVersion;
    char *rep;
    int len;
    value rep_v;
    
    dwVersion = GetVersion();
    
    // Get the Windows version.
    MajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    MinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
    
    switch (MinorVersion){
    case 0:
	switch (MajorVersion) {
	case 3:
	    rep = "NT3.5";
	    break;
	case 4:
	    rep = "NT4.0";
	    break;
	case 5:
	    rep = "2000";
	    break;
	default:
	    printf("major=%d\n", MajorVersion); fflush(stdout);
	    invalid_argument ("Bad major version in WIN32 system");
	    break;
	}
	break;
	
    case 1:
	switch (MajorVersion){
	case 4:
	    rep = "95/98";
	    break;
	case 3:
	    rep = "3.11";
	    break;
	default:
	    printf("major=%d\n", MajorVersion); fflush(stdout);
	    invalid_argument ("Bad major version in WIN32 system");
	    break;
	}
	break;
	
    default:
	printf("minor=%d\n", MajorVersion); fflush(stdout);
	invalid_argument ("Bad minor version in WIN32 system");
	break;
    }
    
    len = strlen(rep);
    rep_v = alloc_string(len);
    memcpy(String_val(rep_v), rep, len);
    return rep_v;
}

/**************************************************************/
/**************************************************************/
/* Author: Robbert Van Renesse
 *
 * This code solves the following problem.  We would like applications
 * to be able to run as DOS applications using a command window.  This
 * rules out using Windows window handles and event driven programming
 * (the new SDK contains a separate events package which is supported
 * by Winsock2, but for portability/availability reasons, I haven't
 * considered that option yet).  The winsock library has a select()
 * function, but it only works on sockets.  You cannot do a select on
 * stdin!  So to make everything work, I'm using a thread that reads
 * stdin, and then sends it to a socket.  The socket handle is used by
 * the Socket library and called Socket.stdin.  This way, it appears
 * as if it's actually reading input.  Beware though: the thread eats
 * your type ahead!
 */

#define SKT_BUFSIZE 128

static ocaml_skt_t skt_stdin;
static struct sockaddr_in skt_addr;

/* This thread waits for input, and sends it to socket skt_stdin.
 */
static DWORD skt_input_thread(LPDWORD p){
  char buf[SKT_BUFSIZE];
  int n;
  
  for (;;) {
    n = _read(0, buf, sizeof(buf));
    if (n <= 0) 
      break;
    n = sendto(skt_stdin, buf, n, 0, (struct sockaddr *) &skt_addr,
	       sizeof(skt_addr));
    if (n <= 0) {
      printf("sendto --> %d %d\n", n, WSAGetLastError ());
      break;
    }
  }
  closesocket(skt_stdin);
  ExitThread(0);
  return 0;
}

/* This function returns Socket.stdin.  It creates a socket for this, and a
 * thread that sends messages to this socket.
 */
value skt_start_input(){
  CAMLparam0();
  CAMLlocal1(skt_stdin_v);
  int fromlen;
  char buf[SKT_BUFSIZE];
  DWORD tid;
  struct sockaddr_in from;
  struct hostent *h;
  
  skt_stdin = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (skt_stdin == -1)
    serror("start_input:socket");
  memset(&skt_addr, 0, sizeof(skt_addr));
  skt_addr.sin_family = AF_INET;
  skt_addr.sin_port = 0;
  gethostname(buf, sizeof(buf));
  h = gethostbyname(buf);
  if (h == 0)
    serror("start_input:gethostbyname");
  memcpy(&skt_addr.sin_addr, *h->h_addr_list, h->h_length);
  if (bind(skt_stdin, (struct sockaddr *) &skt_addr,
	   sizeof(skt_addr)) < 0)
    serror("start_input:bind");
  fromlen = sizeof(from);
  getsockname(skt_stdin, (struct sockaddr *) &from, &fromlen);
  skt_addr.sin_port = from.sin_port;
  CreateThread(0, 0, (LPTHREAD_START_ROUTINE) skt_input_thread,
	       0, 0, &tid);

  skt_stdin_v = Val_socket(skt_stdin);
  CAMLreturn(skt_stdin_v);
}

/**************************************************************/
/**************************************************************/

value skt_substring_eq(
    value s1_v,
    value ofs1_v,
    value s2_v,
    value ofs2_v,
    value len_v
    )
{
    long len ;
    long ret ;
    char *s1 ;
    char *s2 ;
    
    len = Long_val(len_v) ;
    s1 = ((char*)String_val(s1_v)) + Long_val(ofs1_v) ;
    s2 = ((char*)String_val(s2_v)) + Long_val(ofs2_v) ;
    ret = !memcmp(s1,s2,len) ;
    return Val_bool(ret) ;
}

/**************************************************************/
/* A set of errors to ignore on receiving a UDP packet.
 */
void skt_udp_recv_error(void) {
    switch (h_errno) {
#ifdef EPIPE
    case EPIPE:
#endif
	
#ifdef EINTR
    case EINTR:
#endif
#ifdef EAGAIN
    case EAGAIN:
#endif
	
#ifdef ECONNREFUSED
    case ECONNREFUSED:
#endif
	
#ifdef WSAECONNREFUSED
    case WSAECONNREFUSED:
#endif
	
#ifdef WSAECONNRESET
    case WSAECONNRESET:
#endif
	
#ifdef WSAEHOSTUNREACH
    case WSAEHOSTUNREACH:
#endif
	
#ifdef WSAEHOSTDOWN
    case WSAEHOSTDOWN:
#endif
	
#ifdef WSAEISCONN
    case WSAEISCONN:
#endif
	
#ifdef WSAEMSGSIZE
    case WSAEMSGSIZE:
#endif
	
	/* Do nothing */
	break ;
    default:
	printf ("Udp recv, uncaught error (%d)\n", h_errno);
	serror("udp_recv");
	break ;
    }
}

/* unix_error is from unixsupport.c */
extern void uerror(char *cmdname, value cmdarg);

void win32_maperr(unsigned long errcode);

void serror(char *cmdname)
{
    SKTTRACE(("h_errno=%d\n", h_errno));
    win32_maperr(h_errno);
    uerror(cmdname, Nothing);
}


