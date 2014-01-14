
void atm_init(void) ;
void atm_send(int vci, char *buf, int len) ;
int  atm_recv(int *vci, char *buf, int len) ;
void atm_activate(int vci) ;
