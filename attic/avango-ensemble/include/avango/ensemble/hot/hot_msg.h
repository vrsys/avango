/*
 * Contents: Interface for HOT messages.
 *
 * Author:  Alexey Vaysburd, November 1996
 *
 * Terminology:  Messages are written at the head and are read from head to
 * tail (i.e. push/pop).
 */

#ifndef __HOT_MSG_H__
#define __HOT_MSG_H__

#include <limits.h>
#include <sys/types.h>
#include <assert.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

    /* jsd: changed to allow INLINED defined empty */
#ifdef INLINE
#define MSG_INLINE INLINE
#else
#define MSG_INLINE
#endif

#ifndef HOT_UINT32
#define HOT_UINT32
#if(ULONG_MAX == 0xFFFFFFFF)
    typedef unsigned long hot_uint32;
    typedef long hot_int32;
#elif(UINT_MAX == 0xFFFFFFFF) || (WORD_BIT == 32)
    typedef unsigned int hot_uint32;
    typedef long hot_int32;
#elif(USHRT_MAX == 0xFFFFFFFF)
    typedef unsigned short hot_uint32;
    typedef long hot_int32;
#else
    HOT COMPILE TIME ERROR : NO 32 - BIT INTEGER TYPE DETECTED ? ? ?
#endif
#if(USHRT_MAX == 0xFFFF)
    typedef unsigned short hot_uint16;
    typedef short hot_int16;
#else
    HOT COMPILE TIME ERROR : sizeof(short) != 2
#endif
#endif

    /* Obscured type
     */
    typedef void* hot_msg_t;

    /* Msg seek options (whence).
     */
    typedef enum
    {
        HOT_MSG_SEEK_FROM_HEAD,
        HOT_MSG_SEEK_FROM_TAIL,
        HOT_MSG_SEEK_FROM_CURRENT
    } hot_msg_seek_t;

    /* Read direction.
     */
    typedef enum
    {
        HOT_MSG_READ_LEFT, /* The default */
        HOT_MSG_READ_RIGHT
    } hot_msg_readDirection_t;

    /* Create a message.  Returns the new message.
     */
    hot_msg_t hot_msg_Create(void);

    /* Release a message.
     */
    hot_err_t hot_msg_Release(hot_msg_t* msgp);

    /* Return total size of the message.
     */
    hot_err_t hot_msg_Size(hot_msg_t msg, /*OUT*/ unsigned* sizep);

    /* Set the read direction.  By default, messages are read from left
     * to right (first written/last read).  This can changed to read
     * from left to right (first written/first read).
     */
    hot_err_t hot_msg_ReadDirection(hot_msg_t msg, hot_msg_readDirection_t direction);

    /* Move message pointer to the right to the next alignment boundary.
     * The beginning of message data buffers is always 4-byte aligned.
     */
    hot_err_t hot_msg_Align(hot_msg_t msg, unsigned nbytes);

    /* Seek to the specified position (for reads only).
     * The message becomes read-only.
     *
     * Message is written from left to right and read from right to left.
     * Seeks with positive offsets are to the right, and seeks with negative
     * offsets are to the left.
     */
    hot_err_t hot_msg_Seek(hot_msg_t msg, hot_msg_seek_t whence, unsigned offset);

    /* Get the current read offset in the message.
     */
    hot_err_t hot_msg_GetPos(hot_msg_t msg, /*OUT*/ unsigned* posp);

    /* Set read-offset pointer to the specified position.
     * The message becomes read-only.
     */
    hot_err_t hot_msg_SetPos(hot_msg_t msg, unsigned offset);

    /* Make an alias of the specified message.  Both the original message
     * and the alias become read-only.  The alias has its own read-offset pointer.
     *
     * In case of success, the pointer to the alias is returned in aliasp.
     */
    hot_err_t hot_msg_Alias(hot_msg_t msg, /*OUT*/ hot_msg_t* aliasp);

    /* Make a writeable copy of the specified message.
     *
     * In case of success, the pointer to the copy is returned in copyp.
     */
    hot_err_t hot_msg_Copy(hot_msg_t msg, /*OUT*/ hot_msg_t* copyp);

    /* Create a read-only message with the specified buffer as its contents.
     * When the message is to be released, the specified function is invoked.
     */
    hot_msg_t hot_msg_CreateDontCopy(void* data, unsigned nbytes, void (*on_release)(void*), void* env);

    /* Write a buffer to the message.  The total size of the message is
     * limited to under 4K in the current implementation.
     */
    hot_err_t hot_msg_Write(hot_msg_t msg, void* data, unsigned nbytes);

    /* Read data from the message.  In case of success, the data is copied to the
     * specified buffer preallocated by the caller.
     * The message becomes read-only.
     */
    hot_err_t hot_msg_Read(hot_msg_t msg, void* buf, unsigned nbytes);

    /* Read the message without copying data.  Current offset changes as in read.
     * In case of success, pointer to data is returned in datap.
     * The message becomes read-only.
     */
    hot_err_t hot_msg_Look(hot_msg_t msg, unsigned nbytes, /*OUT*/ void** datap);

    /* Pack messages into the specified destination.
     *
     * Only tails of messages are packed (unread portions).  Note that the
     * tail of a writeable message is the entire message.
     */
    hot_err_t hot_msg_Pack(hot_msg_t msg, hot_msg_t* msgs, unsigned nmsgs);

    /* Unpack a message.  The message becomes read-only.
     * In case of success, returns the pointer an array of unpacked
     * messages in msgsp and array size in *nmsgsp.
     */
    hot_err_t hot_msg_Unpack(hot_msg_t msg,
                             /*OUT*/ hot_msg_t** msgsp,
                             /*OUT*/ unsigned* nmsgsp);

    /* Returns non-zero iff the message is writable.
     */
    int hot_msg_IsWritable(hot_msg_t msg);

    /* Write an unsigned 32-bit integer to the message.
     */
    hot_err_t hot_msg_WriteUint32(hot_msg_t msg, hot_uint32* u);

    /* Read an unsigned 32-bit integer from the message.
     */
    hot_err_t hot_msg_ReadUint32(hot_msg_t msg, hot_uint32* u);

#if defined(__cplusplus)
}
#endif

#endif /*__HOT_MSG_H__*/
