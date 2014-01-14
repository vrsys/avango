/*
 * Contents:  Error interface for HOT (implementation)
 *
 * Author:  Alexey Vayeburd, November 1996
 *
 */

#define HOT_ERROR_NO_DEBUG_WRAPPERS
#include <assert.h>
#include "hot_sys.h"
#include "hot_error.h"

/* An error object:  specified are the error code and the reason.
 */ 
struct hot_err {
  char *reason;
  unsigned code;
};

/* Create an error object with specified code and reason
 */ 
hot_err_t 
hot_err_Create(unsigned err_code, const char *err_string) {
  struct hot_err *err = (struct hot_err*) malloc(sizeof(struct hot_err));
  if (err == NULL)
    hot_sys_Panic("out of memory");
  
  if (err_string == NULL)
    err_string = "unspecified error"; 

  err->reason = (char*) malloc(strlen(err_string) + 1);
  if (err->reason == NULL)
    hot_sys_Panic("out of memory");

  strcpy(err->reason, err_string);
  err->code = err_code;

  return (hot_err_t) err;
}

/* Release an error object
 */
void hot_err_Release(hot_err_t err) {
  struct hot_err *errp = (struct hot_err*) err;
  free(errp->reason);
  free(errp);
}

/* Return the reason of an error
 */
char* hot_err_ErrString(hot_err_t err) {
  struct hot_err *errp = (struct hot_err*) err;
  return errp->reason;
}

/* Return the code of an error
 */
unsigned hot_err_ErrCode(hot_err_t err) {
  struct hot_err *errp = (struct hot_err*) err;
  return errp->code;
}

void hot_err_Panic(hot_err_t err, char *msg2) {
  char *msg1 = hot_err_ErrString(err) ;
  char *comb ;
  
  comb = malloc(sizeof(char) * (strlen(msg1) + strlen(msg2) + 2)) ;
  if (comb == NULL)
    hot_sys_Panic("hot_err_Panic:malloc returned NULL") ;
  
  sprintf(comb,"%s:%s",msg1,msg2) ;
  assert(strlen(comb) == strlen(msg1) + strlen(msg2) + 1) ;
  hot_sys_Panic(comb) ;
}

