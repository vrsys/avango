/**************************************************************/
/* MM_BASIC.C */
/* Author: Ohad Rodeh, 9/2001 */
/* The basic memory management functions. */
/**************************************************************/

#ifndef __MM_BASIC_H__
#define __MM_BASIC_H__

#include "mm_so.h"

/* The type of application-defined allocation and de-allocation functions. 
 */
typedef void* (*mm_alloc_t)(int);
typedef void  (*mm_free_t)(void*);

/* Wrappers for basic LIBC malloc and free. This is a required to work with
 * windows DLLs. On unix or with a windows static library, there is not need to
 * use these functions.
 *
 * The problem is that on windows a DLL normally contains its own copy of LIBC,
 * This means that all allocation on the DLL's LIBC needs wrappers. 
 */
LINKDLL void *ce_alloc_msg_space(int size);
LINKDLL void ce_free_msg_space(void *buf);

/* The same two functions, for internal use
 */
void *ce_intrn_alloc_msg_space(int);
void  ce_intrn_free_msg_space(void *);

/* allocating message space
 */
LINKDLL void ce_set_alloc_fun(mm_alloc_t f);
LINKDLL void ce_set_free_fun(mm_free_t f);


#endif
