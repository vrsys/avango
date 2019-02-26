// $Header$
//
// Author:  Alexey Vaysburd, October 1997.
//
// Contents:  Adaptor/communicator classes for connecting the application
//            with Maestro.
//
// With the adaptor/communicator approach, the application defines
// a subclass of the Maestro_Communicator class, overloading Maestro
// callbacks as needed.  Then an instance of a subclass of Maestro_AdaptorBase
// is created (a wrapper for top classes in the Maestro group-member/
// client-server hierarchy) and attached to the communicator.
// Maestro_AdaptorBase defines Maestro downcalls and passes incoming Maestro
// callbacks up to the communicator.

#ifndef __MAESTRO_ADAPTOR_H__
#define __MAESTRO_ADAPTOR_H__

#include <avango/ensemble/maestro/Maestro_CSX.h>
#include <avango/ensemble/maestro/Maestro_Prim.h>
#include <avango/ensemble/maestro/Maestro_ORB.h>
#include <avango/ensemble/maestro/Maestro_ETC.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

/*********************** Maestro_AdaptorBase **********************/

// A base for wrapper classes over top classes in the Maestro hierarchy,
// such as Maestro_CSX and Maestro_Prim.
class Maestro_AdaptorBase
{
  public:
    virtual ~Maestro_AdaptorBase() {}

    MAESTRO_INLINE virtual void myEndpID(Maestro_EndpID& eid) = 0;

    MAESTRO_INLINE virtual void cast(Maestro_Message& msg) = 0;

    MAESTRO_INLINE virtual void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView) = 0;

    MAESTRO_INLINE virtual void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) = 0;

    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg) = 0;

    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView) = 0;

    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) = 0;

    MAESTRO_INLINE virtual void scast(Maestro_Message& msg) = 0;

    MAESTRO_INLINE virtual void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView) = 0;

    MAESTRO_INLINE virtual void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) = 0;

    MAESTRO_INLINE virtual void lsend(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) = 0;
    virtual void join() = 0;
    virtual void leave() = 0;
    virtual void resetState() = 0;

    MAESTRO_INLINE virtual void getState(Maestro_XferID& xferID, Maestro_Message& requestMsg, Maestro_Message& stateMsg, Maestro_XferStatus& xferStatus) = 0;

    MAESTRO_INLINE virtual void getState(Maestro_XferID& xferID, Maestro_Message& requestMsg, Maestro_XferStatus& xferStatus) = 0;

    MAESTRO_INLINE virtual void sendState(Maestro_EndpID& dest, Maestro_XferID& xferID, Maestro_Message& stateMsg) = 0;

    virtual void xferDone(Maestro_XferID& xferID) = 0;
};

/*********************** Maestro_Communicator **********************/

// A base for classes communicating via Maestro.
class Maestro_Communicator
{
  public:
    virtual ~Maestro_Communicator() {}

    virtual void viewMsg_Callback(Maestro_CSX_ViewData&,
                                  /*OUT*/ Maestro_Message&)
    {
    }

    virtual void acceptedView_Callback(Maestro_CSX_ViewData&, Maestro_Message&) {}

    virtual void receiveCast_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void receiveScast_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void receiveSend_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void receiveLsend_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void heartbeat_Callback(unsigned) {}
    virtual void block_Callback() {}
    virtual void exit_Callback() {}

    virtual void askState_Callback(Maestro_EndpID&, Maestro_XferID&, Maestro_Message&) {}

    virtual void stateTransfer_Callback(Maestro_XferID&) {}

    virtual void gotState_Callback(Maestro_XferID&, Maestro_Message&) {}

    virtual void xferCanceled_Callback(Maestro_XferID&) {}
};

/****************** Maestro_Adaptor Template **********************/

// Adaptor classes
#define Maestro_Adaptor(Name, Prefix)                                                                                                                                                                  \
                                                                                                                                                                                                       \
    class Maestro_Adaptor_##Name##Prefix : virtual public Maestro_AdaptorBase, virtual public Maestro_##Name                                                                                           \
    {                                                                                                                                                                                                  \
      public:                                                                                                                                                                                          \
        Maestro_Adaptor_##Name##Prefix(Maestro_Communicator* comm_, Maestro_##Name##_Options& ops) : Maestro_##Name(ops)                                                                               \
        {                                                                                                                                                                                              \
            assert(comm_ != NULL);                                                                                                                                                                     \
            comm = comm_;                                                                                                                                                                              \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        virtual ~Maestro_Adaptor_##Name##Prefix() { leave(); }                                                                                                                                         \
                                                                                                                                                                                                       \
        void myEndpID(Maestro_EndpID& eid) { Maestro_##Name ::myEndpID(eid); }                                                                                                                         \
                                                                                                                                                                                                       \
        void cast(Maestro_Message& msg) { Maestro_##Name ::cast(msg); }                                                                                                                                \
                                                                                                                                                                                                       \
        void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView) { Maestro_##Name ::cast(msg, sendView); }                                                                                       \
                                                                                                                                                                                                       \
        void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) { Maestro_##Name ::cast(msg, sendView, msgOps); }                                              \
                                                                                                                                                                                                       \
        void send(Maestro_EndpID& dest, Maestro_Message& msg) { Maestro_##Name ::send(dest, msg); }                                                                                                    \
                                                                                                                                                                                                       \
        void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView) { Maestro_##Name ::send(dest, msg, sendView); }                                                           \
                                                                                                                                                                                                       \
        void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) { Maestro_##Name ::send(dest, msg, sendView, msgOps); }                  \
                                                                                                                                                                                                       \
        void scast(Maestro_Message& msg) { Maestro_##Name ::scast(msg); }                                                                                                                              \
                                                                                                                                                                                                       \
        void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView) { Maestro_##Name ::scast(msg, sendView); }                                                                                     \
                                                                                                                                                                                                       \
        void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) { Maestro_##Name ::scast(msg, sendView, msgOps); }                                            \
                                                                                                                                                                                                       \
        void lsend(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps) { Maestro_##Name ::lsend(msg, sendView, msgOps); }                                            \
                                                                                                                                                                                                       \
        virtual void join() { Maestro_##Name ::join(); }                                                                                                                                               \
        virtual void leave() { Maestro_##Name ::leave(); }                                                                                                                                             \
        virtual void resetState() { Maestro_##Name ::resetState(); }                                                                                                                                   \
                                                                                                                                                                                                       \
        void getState(Maestro_XferID& xferID, Maestro_Message& requestMsg, Maestro_Message& stateMsg, Maestro_XferStatus& xferStatus)                                                                  \
        {                                                                                                                                                                                              \
            Maestro_##Name ::getState(xferID, requestMsg, stateMsg, xferStatus);                                                                                                                       \
        }                                                                                                                                                                                              \
                                                                                                                                                                                                       \
        void getState(Maestro_XferID& xferID, Maestro_Message& requestMsg, Maestro_XferStatus& xferStatus) { Maestro_##Name ::getState(xferID, requestMsg, xferStatus); }                              \
                                                                                                                                                                                                       \
        void sendState(Maestro_EndpID& dest, Maestro_XferID& xferID, Maestro_Message& stateMsg) { Maestro_##Name ::sendState(dest, xferID, stateMsg); }                                                \
                                                                                                                                                                                                       \
        void xferDone(Maestro_XferID& xferID) { Maestro_##Name ::xferDone(xferID); }                                                                                                                   \
                                                                                                                                                                                                       \
      protected:                                                                                                                                                                                       \
        void Prefix##_ViewMsg_Callback(Maestro_##Name##_ViewData& viewData, Maestro_Message& viewMsg) { comm->viewMsg_Callback(viewData, viewMsg); }                                                   \
                                                                                                                                                                                                       \
        void Prefix##_AcceptedView_Callback(Maestro_##Name##_ViewData& viewData, Maestro_Message& msg) { comm->acceptedView_Callback(viewData, msg); }                                                 \
                                                                                                                                                                                                       \
        void Prefix##_ReceiveCast_Callback(Maestro_EndpID& origin, Maestro_Message& msg) { comm->receiveCast_Callback(origin, msg); }                                                                  \
                                                                                                                                                                                                       \
        void Prefix##_ReceiveScast_Callback(Maestro_EndpID& origin, Maestro_Message& msg) { comm->receiveScast_Callback(origin, msg); }                                                                \
                                                                                                                                                                                                       \
        void Prefix##_ReceiveSend_Callback(Maestro_EndpID& origin, Maestro_Message& msg) { comm->receiveSend_Callback(origin, msg); }                                                                  \
                                                                                                                                                                                                       \
        void Prefix##_ReceiveLsend_Callback(Maestro_EndpID& origin, Maestro_Message& msg) { comm->receiveLsend_Callback(origin, msg); }                                                                \
                                                                                                                                                                                                       \
        void Prefix##_Heartbeat_Callback(unsigned time) { comm->heartbeat_Callback(time); }                                                                                                            \
                                                                                                                                                                                                       \
        void Prefix##_Block_Callback() { comm->block_Callback(); }                                                                                                                                     \
        void Prefix##_Exit_Callback() { comm->exit_Callback(); }                                                                                                                                       \
                                                                                                                                                                                                       \
        void askState_Callback(Maestro_EndpID& origin, Maestro_XferID& xferID, Maestro_Message& requestMsg) { comm->askState_Callback(origin, xferID, requestMsg); }                                   \
                                                                                                                                                                                                       \
        void stateTransfer_Callback(Maestro_XferID& xferID) { comm->stateTransfer_Callback(xferID); }                                                                                                  \
                                                                                                                                                                                                       \
        void gotState_Callback(Maestro_XferID& xferID, Maestro_Message& stateMsg) { comm->gotState_Callback(xferID, stateMsg); }                                                                       \
                                                                                                                                                                                                       \
        void xferCanceled_Callback(Maestro_XferID& xferID) { comm->xferCanceled_Callback(xferID); }                                                                                                    \
                                                                                                                                                                                                       \
        Maestro_Communicator* comm;                                                                                                                                                                    \
    }

//
// Adaptors for two top classes in the Maestro hierarchy:
// Maestro_CSX (partitionable membership + state transfer) and
// Maestro_Prim (primary-partition membership + state transfer).
//
typedef Maestro_Adaptor(Prim, prim) Maestro_AdaptorPrim;
typedef Maestro_Adaptor(CSX, csx) Maestro_AdaptorCSX;

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_ADAPTOR_H__
