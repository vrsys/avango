// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_MAESTRO_MERGE_H)
#define AVANGO_MAESTRO_MERGE_H

#include <vector>

#include <avango/ensemble/maestro/Maestro_GroupMember.h>

namespace av
{
enum Maestro_MergeMsgType
{
    MAESTRO_MERGE_CAST,
    MAESTRO_MERGE_SEND,
    MAESTRO_MERGE_STATE_CAST
};

struct MaestroMergeViewData : public Maestro_GrpMemb_ViewData
{
    MaestroMergeViewData();
    MaestroMergeViewData(Maestro_GrpMemb_ViewData& gmView);
    void blend_in(Maestro_Message& msg);
    void splice_out(Maestro_Message& msg);

    int stateCounter; // counts the number of cast and send upcalls, added by Roy Friedman
};

using MaestroMergeOptions = Maestro_GrpMemb_Options;

/**
 * MaestroMerge is a specialization of Maestro_GroupMember providing additional features
 * for clients
 * - State transfer callbacks
 * - Automatic delaying of messages send during a state transfer
 * - ...
 */
class MaestroMerge : public Maestro_GroupMember
{
  public:
    MaestroMerge(MaestroMergeOptions& ops);
    MaestroMerge(MaestroMerge& hMerge);
    virtual ~MaestroMerge() {}
    virtual MaestroMerge& operator=(MaestroMerge& hMerge);

    // Join the group.
    virtual void join();

    // Sending messages.
    virtual void cast(Maestro_Message& msg);
    virtual void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView);

    virtual void send(Maestro_EndpID& dest, Maestro_Message& msg);
    virtual void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView);

    virtual void leave();

    virtual const Maestro_EndpID& eid();

  protected:
    /**************** Client-Server interface upcalls ***********************/

    // State transfer callbacks
    virtual void Merge_GetState_Callback(Maestro_EndpID&, Maestro_Message&) {}
    virtual void Merge_SetState_Callback(Maestro_EndpID&, Maestro_Message&) {}
    virtual void Merge_RemoveState_Callback(Maestro_EndpID&) {}

    // View callbacks
    virtual void Merge_ViewMsg_Callback(MaestroMergeViewData&, /*OUT*/ Maestro_Message&) {}
    virtual void Merge_AcceptedView_Callback(MaestroMergeViewData&, Maestro_Message&) {}

    // Message callbacks
    virtual void Merge_ReceiveCast_Callback(Maestro_EndpID&, Maestro_Message&) {}
    virtual void Merge_ReceiveSend_Callback(Maestro_EndpID&, Maestro_Message&) {}

    // The group is blocked for a view change
    virtual void Merge_Block_Callback() {}
    virtual void Merge_UnBlock_Callback() {}

    // The member has joined the group and the first view is just beeing installed

    virtual void Merge_Join_Callback(Maestro_EndpID&) {}

    // The member has left the group
    virtual void Merge_Exit_Callback() {}

    // Heartbeat callback
    virtual void Merge_Heartbeat_Callback(unsigned time);

  private:
    int mFirstViewInstalled;
    MaestroMergeViewData mMyView;

    bool mBlocked;
    void block_client();
    void unblock_client();

    void cast_state();
    void receive_state(Maestro_EndpID& origin, Maestro_Message& msg);

    // Added by Roy Friedman for new state xfer
    int mValidStateGuro;
    Maestro_Message mStateMsg;
    int mMaxStateCounter;
    Maestro_EndpID mStateGuro;
    int mNumStates;

    int mXferInProgress;

    /**************************** Callbacks ************************************/

    // View callbacks
    void grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData& viewData,
                                  /*OUT*/ Maestro_Message& viewMsg);
    void
    grpMemb_ViewStateInfo_Callback(Maestro_EndpID /*in*/& origin, Maestro_GrpMemb_ViewData /*in*/& viewData, Maestro_Message /*in*/& msg, Maestro_Message /*out*/& next_view_msg, int /*out*/& final);
    void grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData& viewData, Maestro_Message& msg);

    // Message Callbacks
    void grpMemb_ReceiveCast_Callback(Maestro_EndpID& origin, Maestro_Message& msg);
    void grpMemb_ReceiveSend_Callback(Maestro_EndpID& origin, Maestro_Message& msg);

    // The group is blocked for a view change
    void grpMemb_Block_Callback();

    // The member has left the group
    void grpMemb_Exit_Callback();

    // Heartbeat Callback:
    void grpMemb_Heartbeat_Callback(unsigned time) { Merge_Heartbeat_Callback(time); }

    // delay messages sent during xfer
    class DelayMsg
    {
      public:
        DelayMsg() : _use_view(false), _use_dest(false) {}

        DelayMsg(const DelayMsg& m) : _msg((Maestro_Message&)m._msg), _view(m._view), _dest((Maestro_EndpID&)m._dest), _use_view(m._use_view), _use_dest(m._use_dest) {}

        const DelayMsg& operator=(const DelayMsg& m)
        {
            _msg = (Maestro_Message&)m._msg;
            _view = m._view;
            _dest = (Maestro_EndpID&)m._dest;
            _use_dest = m._use_dest;
            _use_view = m._use_view;
            return *this;
        };

        Maestro_Message _msg;
        Maestro_MsgSendView _view;
        Maestro_EndpID _dest;
        bool _use_view;
        bool _use_dest;
    };

    using DelayMsgVec = std::vector<DelayMsg>;
    DelayMsgVec mDelayedMsgs;

    void delayMessage(Maestro_Message& msg);
    void delayMessage(Maestro_Message& msg, Maestro_MsgSendView& sendView);
    void delayMessage(Maestro_EndpID& dest, Maestro_Message& msg);
    void delayMessage(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView);
    void deliverDelayedMessages();
};

} // namespace av

#endif // #if !defined(AVANGO_MAESTRO_MERGE_H)
