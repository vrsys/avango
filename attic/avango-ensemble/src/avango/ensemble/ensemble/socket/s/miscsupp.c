/**************************************************************/

#include "skt.h"

/**************************************************************/
#ifndef _WIN32
value win_version(void){ invalid_argument("Calling win_version on a Unix system");}
#else

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

#endif

/**************************************************************/

value skt_substring_eq(
        value s1_v,
	value ofs1_v,
	value s2_v,
	value ofs2_v,
	value len_v
) {
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

/* Unix and WIN32 (in errno.h)
 */
#ifdef EINTR
  case EINTR:
#endif
#ifdef EAGAIN
  case EAGAIN:
#endif

#ifdef ECONNREFUSED
  case ECONNREFUSED:
#endif
    /* WIN32 */
#ifdef WSAECONNREFUSED
  case WSAECONNREFUSED:
#endif

#ifdef ECONNRESET
  case ECONNRESET:
#endif
    /* WIN32 */
#ifdef WSAECONNRESET
  case WSAECONNRESET:
#endif

#ifdef ENETUNREACH
  case ENETUNREACH:
#endif
    /* WIN32 */
#ifdef WSAEHOSTUNREACH
  case WSAEHOSTUNREACH:
#endif

#ifdef EHOSTDOWN
  case EHOSTDOWN:
#endif
    /* WIN32 */
#ifdef WSAEHOSTDOWN
  case WSAEHOSTDOWN:
#endif


#ifdef EISCONN
  case EISCONN:
#endif
    /* WIN32 */
#ifdef WSAEISCONN
  case WSAEISCONN:
#endif

    /* WIN32 */
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

#ifndef _WIN32
void serror(char *cmdname)
{
  uerror(cmdname, Nothing);
}

#else
void win32_maperr(unsigned long errcode);

void serror(char *cmdname)
{
  SKTTRACE(("h_errno=%d\n", h_errno));
  win32_maperr(h_errno);
  uerror(cmdname, Nothing);
}
#endif

