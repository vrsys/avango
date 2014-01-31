// $Header$

#ifndef __MAESTRO_CSX_H__
#define __MAESTRO_CSX_H__

/************************************************************************
 *                                                                      *
 * Author:  Alexey Vaysburd  December 96                                *
 *                                                                      *
 * Contents:  Definition of Maestro_CSX class (higher-level state xfer) *
 *                                                                      *
 ************************************************************************/

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_ClSv.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

class Maestro_CSX;

class Maestro_XferID : public Maestro_Base {
    friend class Maestro_CSX;
public:
    Maestro_XferID() : id(0) {}
    Maestro_XferID(int theID) : id(theID) {}
    void operator>> (std::ostream& out) const { out << id; }
private:
    int id;
};

struct Maestro_CSX_XferEnv {
  Maestro_CSX_XferEnv(Maestro_CSX *theCsx, int theID) :
    csx(theCsx), xid(theID) {}

  Maestro_CSX *csx;
  Maestro_XferID xid;
};

// Each time a piece of the state is requested, xfer status is returned.
typedef enum {
  MAESTRO_XFER_OK,               // may continue with xfer
  MAESTRO_XFER_TERMINATED        // this xfer transaction has been terminated 
} Maestro_XferStatus;

struct Maestro_XferData {
  Maestro_EndpID server; 
  Maestro_Message request, reply;
  Maestro_Semaphore sema;
  Maestro_XferStatus status;
};

typedef Maestro_ClSv_Options     Maestro_CSX_Options;
typedef Maestro_ClSv_ViewData    Maestro_CSX_ViewData;
typedef Maestro_ClSv_MsgOptions  Maestro_CSX_MsgOptions;

class Maestro_CSX : public Maestro_ClSv {
public:

  Maestro_CSX(Maestro_CSX_Options &ops);

  Maestro_CSX(Maestro_CSX &mcsx);
  virtual ~Maestro_CSX();
  virtual Maestro_CSX& operator= (Maestro_CSX &mcsx);

  // State transfer downcalls:

  virtual void resetState() {} // reset state when (re)starting xfer

  MAESTRO_INLINE virtual void getState(Maestro_XferID &xferID, 
			       Maestro_Message &requestMsg, 
			       Maestro_Message &stateMsg,
			       Maestro_XferStatus &xferStatus);

  MAESTRO_INLINE virtual void getState(Maestro_XferID &xferID, 
			       Maestro_Message &requestMsg, 
			       Maestro_XferStatus &xferStatus);

  MAESTRO_INLINE virtual void sendState(Maestro_EndpID &dest,
				Maestro_XferID &xferID, 
				Maestro_Message &stateMsg);
  
  void xferDone(Maestro_XferID &xferID);

protected:

  // View callbacks:
  
  virtual void csx_ViewMsg_Callback(Maestro_CSX_ViewData &, 
			            /*OUT*/ Maestro_Message &) {}

  virtual void csx_AcceptedView_Callback(Maestro_CSX_ViewData& ,
					 Maestro_Message &) {}

  // Message callbacks:

  virtual void csx_ReceiveCast_Callback(Maestro_EndpID &, 
					Maestro_Message &)  {}

  virtual void csx_ReceiveScast_Callback(Maestro_EndpID &, 
					 Maestro_Message &) {}

  virtual void csx_ReceiveSend_Callback(Maestro_EndpID &, 
					Maestro_Message &)  {}

  virtual void csx_ReceiveLsend_Callback(Maestro_EndpID &, 
					 Maestro_Message &) {}

  // Heartbeat callback:

  virtual void csx_Heartbeat_Callback(unsigned ) {}

  // The group is blocked for a view change:

  virtual void csx_Block_Callback() {}

  // The member has left the group:

  virtual void csx_Exit_Callback() {}

  // State transfer callbacks:  

  virtual void askState_Callback(Maestro_EndpID &, 
				 Maestro_XferID &,
				 Maestro_Message &) {}

  virtual void stateTransfer_Callback(Maestro_XferID &xferID) { 
    xferDone(xferID); 
  }
  
  // State transfer callbacks (async mode):

  virtual void gotState_Callback(Maestro_XferID &,
				 Maestro_Message &) {}

  virtual void xferCanceled_Callback(Maestro_XferID &) {}

private:

  static void xferDispatcher(void *env);
  Maestro_XferID currentXferID;
  Maestro_XferData *xferData;
  Maestro_Lock xferLock;
  Maestro_ThreadMode xferMode;
  enum { GETSTATE_NONE, GETSTATE_SYNC, GETSTATE_ASYNC} getstateStatus;

  Maestro_ClSvState state;
  Maestro_EndpList servers, xferServers;


  /************************* Callbacks ************************************/

  // View callbacks:

  void clSv_ViewMsg_Callback(Maestro_ClSv_ViewData &viewData, 
			     /*OUT*/ Maestro_Message &viewMsg) {
      csx_ViewMsg_Callback(viewData, viewMsg);
  }

  MAESTRO_INLINE void clSv_AcceptedView_Callback(Maestro_ClSv_ViewData& viewData,
				  Maestro_Message &viewMsg);

  // Message callbacks:

  void clSv_ReceiveCast_Callback(Maestro_EndpID &origin, 
				 Maestro_Message &msg) {
    csx_ReceiveCast_Callback(origin, msg);
  }

  void clSv_ReceiveScast_Callback(Maestro_EndpID &origin, 
				  Maestro_Message &msg) {
    csx_ReceiveScast_Callback(origin, msg);
  }

  void clSv_ReceiveSend_Callback(Maestro_EndpID &origin, 
				 Maestro_Message &msg) {
    csx_ReceiveSend_Callback(origin, msg);
  }
  
  void clSv_ReceiveLsend_Callback(Maestro_EndpID &origin, 
				  Maestro_Message &msg) {
    csx_ReceiveLsend_Callback(origin, msg);
  }


  // Heartbeat callback:

  void clSv_Heartbeat_Callback(unsigned time) { csx_Heartbeat_Callback(time); }

  // The member has left the group:

  void clSv_Exit_Callback() { csx_Exit_Callback(); }

  // The group is blocked for a view change:

  void clSv_Block_Callback() { csx_Block_Callback(); }

  // State transfer callbacks:

  MAESTRO_INLINE void askState_Callback(Maestro_EndpID &origin, Maestro_Message &msg);
  MAESTRO_INLINE void rcvState_Callback(Maestro_EndpID &origin, Maestro_Message &msg);

  /************************************************************************/

  // askState and sendState are made private to prevent external invocation. 

  void askState(Maestro_EndpID &server, Maestro_Message &msg) {
    Maestro_ClSv::askState(server, msg);
  }

  void sendState(Maestro_EndpID &dest, Maestro_Message &msg) {
    Maestro_ClSv::sendState(dest, msg);
  }

  void becomeServer(Maestro_ClSv_ViewData& vd);
  void xferDone(Maestro_ClSv_ViewData& vd);
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif  // __MAESTRO_CSX_H__
