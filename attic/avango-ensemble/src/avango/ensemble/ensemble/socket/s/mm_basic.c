/**************************************************************/
/* MM_BASIC.C */
/* Author: Ohad Rodeh, 9/2001 */
/* The basic memory management functions. */
/**************************************************************/
#define CE_MAKE_A_DLL
#include "mm_basic.h"
#include <malloc.h>
#include <stdio.h>

static mm_alloc_t alloc_fun = (mm_alloc_t) malloc ;
static mm_free_t free_fun = free;

LINKDLL void *ce_alloc_msg_space(int size)
{
//    printf("<alloc>");
    return alloc_fun(size);
}

LINKDLL void ce_free_msg_space(void *buf)
{
//    printf("<free>");
    free_fun(buf);
}

void* ce_intrn_alloc_msg_space (int size)
{
//    printf("<intern alloc>");
    return alloc_fun(size);
}

void ce_intrn_free_msg_space (void *buf)
{
//    printf("<intern free>");
    free_fun(buf);
}


LINKDLL void ce_set_alloc_fun(mm_alloc_t f)
{
    alloc_fun = f;
}

LINKDLL void ce_set_free_fun(mm_free_t f)
{
    free_fun = f;
}


