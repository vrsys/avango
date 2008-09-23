/* HOT's C interface to Ensemble.
 */

#ifndef __HOT_ENS_H__
#define __HOT_ENS_H__

/* jsd: changed to allow INLINED defined empty */
#ifdef INLINE
#define HOT_INLINE INLINE
#else
#define HOT_INLINE
#endif

#include <memory.h>
#include <limits.h>

#if defined(__cplusplus)
extern "C" {
#endif

//#define HOT_MSG_PADDING	4	/* Pad messages to 4-byte boundaries */

#ifndef HOT_UINT32
#define HOT_UINT32
#if (ULONG_MAX == 0xFFFFFFFF)
typedef unsigned long hot_uint32 ;
typedef long hot_int32 ;
#elif (UINT_MAX == 0xFFFFFFFF)
typedef unsigned int hot_uint32 ;
typedef long hot_int32 ;
#elif (USHRT_MAX == 0xFFFFFFFF)
typedef unsigned short hot_uint32 ;
typedef long hot_int32 ;
#else
#error "HOT COMPILE TIME ERROR:  NO 32-BIT INTEGER TYPE DETECTED ???"
#endif
#endif

/* Default values.
 */
#define HOT_ENS_DEFAULT_TRANSPORT "UDP"
#if 0
#define HOT_ENS_DEFAULT_PROTOCOL  \
  "Top:Heal:Switch:Leave:Inter:Intra:Elect:Merge:Sync:Suspect:Top_appl:Pt2pt:Frag:Stable:Mnak:Bottom"
#else
#define HOT_ENS_DEFAULT_PROTOCOL \
   "BOTTOM:MNAK:PT2PT:CHK_FIFO:CHK_SYNC:TOP_APPL:STABLE:VSYNC:SYNC:ELECT:INTRA:INTER:LEAVE:SUSPECT:HEAL:TOP"
#endif

#define HOT_ENS_DEFAULT_PROPERTIES "Gmp:Sync:Heal:Switch:Frag:Suspect:Flow"
#define HOT_ENS_DEFAULT_HEARTBEAT_RATE 5000
#define HOT_ENS_DEFAULT_GROUP_NAME "<Ensemble_Default_Group>"

/* Group context descriptor.
 */
typedef struct hot_gctx *hot_gctx_t ;

#define HOT_ENDP_MAX_NAME_SIZE 127
#define HOT_ENDP_ADDR_SIZE sizeof(unsigned long)

/* Endpoint id.
 */
typedef struct { 
  char name[HOT_ENDP_MAX_NAME_SIZE + 1]; 
} hot_endpt_t ;

#define HOT_ENS_MAX_PROTO_NAME_LENGTH 256
#define HOT_ENS_MAX_PARAMS_LENGTH 256
#define HOT_ENS_MAX_PRINCIPAL_NAME_LENGTH 64
#define HOT_ENS_MAX_KEY_LEGNTH     40
#define HOT_ENS_MAX_PROPERTIES_LENGTH 256
#define HOT_ENS_MAX_VERSION_LENGTH 128
#define HOT_ENS_MAX_GROUP_NAME_LENGTH 128
#define HOT_ENS_MAX_IO_NAME_LENGTH 128

typedef struct {
  int ltime ;
  hot_endpt_t coord ;
} hot_view_id ;

typedef unsigned hot_rank_t ;
typedef int hot_bool_t ;

/* View state.
 */
typedef struct {
  char version[HOT_ENS_MAX_VERSION_LENGTH] ;
  char group_name[HOT_ENS_MAX_GROUP_NAME_LENGTH] ;
  hot_endpt_t *members ;
  hot_rank_t nmembers ;
  hot_rank_t rank ;
  char protocol[HOT_ENS_MAX_PROTO_NAME_LENGTH] ;
  hot_bool_t groupd ;
  hot_view_id view_id ;
  char params[HOT_ENS_MAX_PARAMS_LENGTH] ;
  hot_bool_t xfer_view ;
  hot_bool_t primary ;
  hot_bool_t *clients;
  char key[HOT_ENS_MAX_KEY_LEGNTH];
} hot_view_state_t ;

/*##########################################################################*/

/* Application callback types:
 */

/* Received a multicast message.
 */
typedef void (*hot_ens_ReceiveCast_cback)
    (hot_gctx_t gctx, void *env, hot_endpt_t *origin, hot_msg_t msg) ;

/* Received a point-to-point message.
 */
typedef void (*hot_ens_ReceiveSend_cback)
    (hot_gctx_t gctx, void *env, hot_endpt_t *origin, hot_msg_t msg) ;

/* Accepted a new view.
 */
typedef void (*hot_ens_AcceptedView_cback)
    (hot_gctx_t gctx, void *env, hot_view_state_t *view_state) ;

/* Got a heartbeat event (invoked periodically).
 * Current time (in milliseconds) is specified.
 */
typedef void (*hot_ens_Heartbeat_cback)
    (hot_gctx_t gctx, void *env, unsigned rate) ;

/* The group is about to block for a view change 
 */
typedef void (*hot_ens_Block_cback)
  (hot_gctx_t gctx, void *env) ;

/* The member has left the group.
 */
typedef void (*hot_ens_Exit_cback)
  (hot_gctx_t gctx, void *env) ;

/*##########################################################################*/

/* Application callback configuration.
 */
typedef struct {
  hot_ens_ReceiveCast_cback receive_cast ;
  hot_ens_ReceiveSend_cback receive_send ;
  hot_ens_AcceptedView_cback accepted_view ;
  hot_ens_Heartbeat_cback heartbeat ;
  hot_ens_Block_cback block ;
  hot_ens_Exit_cback exit ;
} hot_ens_cbacks_t ;

#define HOT_ENS_MAX_GROUP_NAME_LENGTH 128
#define HOT_ENS_MAX_TRANSPORTS_LENGTH 128

/* Join options.
 */
typedef struct {
    unsigned heartbeat_rate;	/* Rate of heartbeat upcalls, in millisec. */
    char transports[HOT_ENS_MAX_TRANSPORTS_LENGTH]; /* List of transports */
    char group_name[HOT_ENS_MAX_GROUP_NAME_LENGTH]; /* ASCII name of the group */
    char protocol[HOT_ENS_MAX_PROTO_NAME_LENGTH]; /* The protocol stack */
    char properties[HOT_ENS_MAX_PROPERTIES_LENGTH]; /* Protocol properties */
    hot_bool_t use_properties;		/* Set if properties are to be used */
    char params[HOT_ENS_MAX_PARAMS_LENGTH]; /* Protocol parameters */
    hot_bool_t groupd;			/* Set if group daemon is in use */
    hot_ens_cbacks_t conf;
    void *env;
    char **argv;
    hot_bool_t debug;
    hot_bool_t client;
    char outboard[HOT_ENS_MAX_IO_NAME_LENGTH]; /* Outboard module to use */

    /* Normally, Ensemble generates a unique endpoint name for each
     * group an application joins (this is what happens if you leave
     * 'endpt' unmodified).  The application can optionally provide
     * its own endpoint name.  It can, for instance, reuse an
     * endpoint name generated by Ensemble for another group (the
     * same endpoint name can be used to join any number of groups).
     * The application can even generate an endpoint on its own.
     * Such names should be unique.  It is best if they contain only
     * printable characters and do not contain spaces because
     * Ensemble my print them out in debugging or error messages.
     * (The names generated by Ensemble fit these characteristics.)
     * See Endpt.extern in ensemble/type/endpt.mli for more
     * information.
     */
    hot_endpt_t endpt ;

    /* [OR] Security options. 
     */
    char princ[HOT_ENS_MAX_PRINCIPAL_NAME_LENGTH]; /* Principal name */
    char key[HOT_ENS_MAX_KEY_LEGNTH];             /* key length */
    hot_bool_t secure;                      /* Secure group. */
} hot_ens_JoinOps_t;

/* Messages can be sent in the current view, in the next view, etc.  */
typedef enum {
  HOT_ENS_MSG_SEND_UNSPECIFIED_VIEW,
  HOT_ENS_MSG_SEND_CURRENT_VIEW,
  HOT_ENS_MSG_SEND_NEXT_VIEW
} hot_ens_MsgSendView ;

/*##########################################################################*/

/* Exported downcalls:
 */

/* Start Ensemble in the current thread.  
 * If successful, this call will block forever.
 */
hot_err_t hot_ens_Start(
        char **argv
) ;

/* Initialize/reset an options struct.
 */
void hot_ens_InitJoinOps(
        hot_ens_JoinOps_t *ops
) ;

/* Join a group.  On success, returns group context in *gctxp.
 */ 
hot_err_t hot_ens_Join(
        hot_ens_JoinOps_t *ops, 
	hot_gctx_t *gctxp /*OUT*/
) ;

/* Leave a group.  After this downcall, the context becomes invalid.
 */
hot_err_t hot_ens_Leave(hot_gctx_t gctx) ;

/* Send a multicast message to the group.
 * The view in which the message is going to be sent
 * is returned in send_view, if send_view is not NULL.
 */
hot_err_t hot_ens_Cast(
         hot_gctx_t g,
	 hot_msg_t msg, 
	 hot_ens_MsgSendView *send_view	/*OUT*/
) ;

/* Send a point-to-point message to the specified group member.
 * The view in which the message is going to be sent
 * is returned in send_view, if send_view is not NULL.
 */
hot_err_t hot_ens_Send(
        hot_gctx_t g, 
	hot_endpt_t *dest,
	hot_msg_t msg,
	hot_ens_MsgSendView *send_view /*OUT*/
) ;

/* Report specified group members as failure-suspected.
 */
hot_err_t hot_ens_Suspect(
        hot_gctx_t gctx,
	hot_endpt_t *suspects, 
	int nsuspects
) ;


/* Inform Ensemble that the state-transfer is complete. 
 */
hot_err_t hot_ens_XferDone(
        hot_gctx_t gctx
) ;

/* Request a protocol change.
 */
hot_err_t hot_ens_ChangeProtocol(
        hot_gctx_t gctx,
	char *protocol_name
) ;

/* Request a protocol change (specify properties).
 */
hot_err_t hot_ens_ChangeProperties(
        hot_gctx_t gctx, 
	char *properties
) ;

/* Request a new view to be installed.
 */
hot_err_t hot_ens_RequestNewView(
        hot_gctx_t gctx
) ;

/* Request a rekey operation. 
 */
hot_err_t hot_ens_Rekey(
        hot_gctx_t gctx
) ;

  hot_err_t hot_ens_MLPrintOverride(
	void (*handler)(char *msg)
) ;

hot_err_t hot_ens_MLUncaughtException(
	void (*handler)(char *info)
) ;

#if defined(__cplusplus)
}
#endif

#endif /* __HOT_ENS_H__ */
