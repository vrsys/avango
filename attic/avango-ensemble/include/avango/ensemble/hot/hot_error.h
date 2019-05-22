/*
 * Contents:  Error interface for HOT
 *
 * Author:  Alexey Vaysburd, November 1996
 *
 */

#ifndef __HOT_ERROR_H__
#define __HOT_ERROR_H__

#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    /* Obscured error type.
     */
    typedef struct hot_err* hot_err_t;

/* Normal return value
 */
#define HOT_OK NULL

    /* Create an error object with specified code and reason
     */
    hot_err_t hot_err_Create(unsigned err_code, const char* err_string);

    /* Release an error object
     */
    void hot_err_Release(hot_err_t err);

    /* Return the reason of an error
     */
    char* hot_err_ErrString(hot_err_t err);

    /* Return the code of an error
     */
    unsigned hot_err_ErrCode(hot_err_t err);

    /* Panic using the error message.
     */
    void hot_err_Panic(hot_err_t err, char* info);

#define hot_err_Check(err)                                                                                                                                                                             \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        hot_err_t _hot_err_hide = (err);                                                                                                                                                               \
        if(_hot_err_hide != HOT_OK)                                                                                                                                                                    \
            hot_sys_Panic(hot_err_ErrString(_hot_err_hide));                                                                                                                                           \
    } while(0)

#if defined(__cplusplus)
}
#endif

#endif /*__HOT_ERROR_H__*/
