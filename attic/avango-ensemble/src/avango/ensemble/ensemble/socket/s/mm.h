/**************************************************************/
/* MM.H */
/* Author: Ohad Rodeh 9/2001 */
/**************************************************************/

#ifndef __MM_H__
#define __MM_H__

#include "mm_basic.h"

value mm_Raw_of_len_buf(int, char*);
char *mm_Cbuf_val(value);

/* An empty iovec.
 */
value mm_empty(void);

/* Marshaling/unmarshaling from/to C buffers
 */
value mm_output_val(value obj_v, value iov_v, value flags_v);
value mm_input_val(value iov_v);

#endif

