#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <assert.h>
#include "libunet.h"
#include "atm.h"

typedef short channel ;
struct unet_activate_chan activate ;
struct unet_deactivate_chan deactivate ;
struct unet_endpt_user *endpt ;
struct unet_info info ;
struct unet_create_endpt create ;

char *addr ;
char *devname ;
int unet_dev ;

#define N_VCI 256
#define N_CHAN 1000
#define RX_BUFFER_AREA_SIZE (12 * 1024)
static int vcis[N_VCI] ;
static channel channels[N_CHAN] ;

void atm_init(void) {		/* ML */
  static inited = 0 ;
  int i ;
  char devname[] = "/dev/sba0" ;

  assert(!inited) ;
  inited = 1 ;
  
  unet_dev = unet_opendev(devname,&info);
  if (unet_dev < 0) {
    fprintf(stderr,"Unable to open %s.\n",devname);
    exit(-1);
  }

  create.num_tx_fifo_entries = info.max_tx_fifo ;
  create.num_free_fifo_entries = info.max_free_fifo ;
  create.num_rx_fifo_entries = 8 ;
  create.rx_buffer_size = RX_BUFFER_AREA_SIZE ;
  create.buffer_area_size = RX_BUFFER_AREA_SIZE + (50*RX_BUFFER_AREA_SIZE) ;

  endpt = unet_create_endpt(unet_dev, &create, 50) ;
  if (endpt == NULL) {
    fprintf(stderr,"Unable to create endpt!\n");
    exit(-1);
  }
  
  for(i = 0; i < N_VCI; i++)
    vcis[i] = -1 ;

  for(i = 0; i < N_CHAN; i++)
    channels[i] = -1 ;
}
  
void atm_activate(
        int vci
) {
  channel chan ;
  char addr[100] ;

  assert(vci >= 0 || vci < N_VCI) ;
  chan = vcis[vci] ;
  if (chan == -1) {
    sprintf(addr,"%d",vci) ;
    str_to_addr(info.devid, addr, &(activate.tx_addr));
    str_to_addr(info.devid, addr, &(activate.rx_addr));
    
    chan = unet_activate_chan(unet_dev, &activate);
    if (chan < 0) 
      abort() ;

    assert(chan >= 0 && chan < N_CHAN) ;
    assert(channels[chan] == -1) ;
    vcis[vci] = chan ;
    channels[chan] = vci ;
  }
}
  
void atm_send(
        int vci,
	char *buf,
	int len
) {
  volatile Unet_txbd *tx_next ;
  channel chan ;
  int ret ;

  /* Find the chan to use.
   */
  assert(vci >= 0 || vci < N_VCI) ;
  chan = vcis[vci] ;
  if (chan == -1) {
    atm_activate(vci) ;
    chan = vcis[vci] ;
    assert(chan != -1) ;
  }

  /* Copy the message into transmit buffer.
   */
  memcpy(endpt->txb_area, buf, len) ;

  /* Keep sending message until successful.
   */
  do {
    ret = unet_send(endpt,chan,endpt->txb_area,len) ;
  } while (ret == -1) ;

  /* Block until the message has been sent.
   */
  tx_next = (endpt->tx_next == endpt->tx_first) ?
    (endpt->tx_last) :
    (endpt->tx_next - 1) ;

  while(!TX_ISFREE(tx_next->chan)) ;
}

#define NIOVECS 10
	      
int atm_recv(
        int *vci,
	char *buf,
	int len
) {
  int iov_len ;
  channel chan ;
  int i, off, ret ;
  int iovcnt ;
  struct iovec iov[NIOVECS] ;

  iovcnt = NIOVECS ;
  ret = unet_recv(endpt, &chan, iov, &iovcnt) ;
  if (ret == -1)
    return 0 ;
  
  assert(chan >= 0 && chan < N_CHAN) ;
  assert(channels[chan] != -1) ;
  assert(vci) ;
  *vci = channels[chan] ;

  if (iovcnt == 1) {
    iov_len = iov[0].iov_len ;
    if (iov_len > len) abort() ;
    memcpy(buf, iov[0].iov_base, iov_len) ;
    off = iov_len ;
  } else {
    off = 0 ;
    for (i = 0; i < iovcnt; i++) {
      iov_len = iov[i].iov_len ;
      if (off + iov_len > len) abort() ;
      memcpy(buf + off, iov[i].iov_base, iov_len) ;
      off += iov_len ;
    }
  }
  
#ifdef notdef
  unet_free_rxv(endpt, iov, iovcnt) ;
#else
  if (ret > info.rxd_payload_size) {
    int total;
    for (total = 0; total < ret; total += create.rx_buffer_size) {
      /* Push buffer back on the free list */
      endpt->free_next->buf = ((unsigned int)iov[0].iov_base -
                                    (unsigned int)endpt->buffer_area);
      if (endpt->free_next == endpt->free_last)
        endpt->free_next = endpt->free_first;
      else
        endpt->free_next++;
    }
  }
#endif

  return off ;
}

#ifdef MAIN

#define BUFLEN 100

char *msg = "lsdkjfaldfjsdlkjfdslkfJJ*****lsfkjdlk____f" ;

double gettime(void) {
  extern int gettimeofday(struct timeval *,struct timezone *) ;
  struct timeval tp ;
  gettimeofday(&tp, NULL) ;
  return tp.tv_sec + ((double)tp.tv_usec) / 1E6 ;
}
 
void main(int argc,char *argv[]) {
  extern long atoi(char *) ;
  char buf[BUFLEN] ;
  int ret ;
  int vci ;
  double next ;
  int recv_vci ;

  if(argc != 2) {
    printf("atm <vci>\n");
    exit(1);
  }
  vci = atoi(argv[1]) ;

  /*
  vci = atoi(argv[1]) ;
  assert(vci >= 0 || vci < N_VCI) ;
  chan = vcis[vci] ;
  if (chan == -1) {
    chan = unet_activate(vci,vci) ;
    assert(chan >= 0 && chan < N_CHAN) ;
    assert(channels[chan] == -1) ;
    vcis[vci] = chan ;
    channels[chan] = vci ;
  }
  */

  atm_init() ;
  atm_activate(vci) ;
  
  next = 0.0 ;
  while(1) {
    ret = atm_recv(&recv_vci, buf, BUFLEN-1) ;
    if (ret != -1) {
      buf[ret] = 0 ;
      printf("ATM:recv:%d bytes:from %d:'%s'\n",
	     ret, recv_vci, buf) ;
    }

    if (gettime() >= next) {
/*
      printf("ATM:sending\n") ;
*/
      atm_send(vci,msg,strlen(msg)+1) ;
      next = gettime() + .1 ;
    }
  }
}

#endif
