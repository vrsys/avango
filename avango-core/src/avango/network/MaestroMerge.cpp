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

#include "MaestroMerge.h"

#include <iostream>
#include <boost/format.hpp>

#include <avango/Config.h>
#include <avango/Logger.h>
#include <avango/MaestroEID.h>

namespace
{
av::Logger& logger(av::getLogger("av::MaestroMerge"));
}

av::MaestroMerge::MaestroMerge(MaestroMergeOptions& ops) : Maestro_Base(), Maestro_GroupMember(ops), mFirstViewInstalled(0), mBlocked(false), mValidStateGuro(0), mXferInProgress(0)
{
    mMyView.stateCounter = 0;
}

av::MaestroMerge::MaestroMerge(MaestroMerge& hMerge)
    : Maestro_Base(hMerge), Maestro_GroupMember(hMerge){AVANGO_LOG(logger, logging::ERROR, "copy constructor not supported")}

                                av::MaestroMerge
                                & av::MaestroMerge::operator=(MaestroMerge&)
{
    AVANGO_LOG(logger, logging::ERROR, "operator= not supported")
    return *this;
}

void av::MaestroMerge::join()
{
    Maestro_GroupMember::join();
    AVANGO_LOG(logger, logging::DEBUG, "join")
}

void av::MaestroMerge::leave()
{
    Maestro_GroupMember::leave();
    mFirstViewInstalled = 0;
    AVANGO_LOG(logger, logging::DEBUG, "leave")
}

/**************************** cast **************************************/

void av::MaestroMerge::cast(Maestro_Message& msg)
{
    msg << (int)MAESTRO_MERGE_CAST;
    Maestro_MsgSendView sendView = MAESTRO_MSG_SEND_CURRENT_VIEW;

    if(mXferInProgress)
    {
        delayMessage(msg);
    }
    else
    {
        Maestro_GroupMember::cast(msg, sendView);
    }
}

void av::MaestroMerge::cast(Maestro_Message& msg, Maestro_MsgSendView& sendView)
{
    msg << (int)MAESTRO_MERGE_CAST;

    if(mXferInProgress)
    {
        delayMessage(msg, sendView);
    }
    else
    {
        Maestro_GroupMember::cast(msg, sendView);
    }
}

/**************************** send **************************************/

void av::MaestroMerge::send(Maestro_EndpID& dest, Maestro_Message& msg)
{
    msg << (int)MAESTRO_MERGE_SEND;
    Maestro_MsgSendView sendView = MAESTRO_MSG_SEND_CURRENT_VIEW;

    if(mXferInProgress)
    {
        delayMessage(dest, msg);
    }
    else
    {
        Maestro_GroupMember::send(dest, msg, sendView);
    }
}

void av::MaestroMerge::send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView)
{
    msg << (int)MAESTRO_MERGE_SEND;

    if(mXferInProgress)
    {
        delayMessage(dest, msg, sendView);
    }
    else
    {
        Maestro_GroupMember::send(dest, msg, sendView);
    }
}

/************************************************************************/
/****************** Overloaded Maestro_GroupMember upcalls **************/
/************************************************************************/

/******************* grpMemb_AcceptedView_Callback **********************/

void av::MaestroMerge::grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData& gmView, Maestro_Message& msg)
{
    // Extract membership info from the view message.
    MaestroMergeViewData vd(gmView);
    vd.blend_in(msg);

    if(logger.isActive(logging::DEBUG))
    {
        logger.debug() << "grpMemb_AcceptedView_Callback: "
                       //<< vd.viewID
                       << " [" << (vd.coordinator == vd.myEndpID ? " coordinator" : "") << "]";

        for(size_t i = 0; i < vd.members.size(); i++)
        {
            MaestroEID eid(vd.members[i]);

            logger.debug() << "grpMemb_AcceptedView_Callback: " << eid << (vd.newMembers.contains(eid) ? " (new)" : "");
        }

        for(size_t j = 0; j < vd.departedMembers.size(); j++)
        {
            MaestroEID eid(vd.departedMembers[j]);
            logger.debug() << "grpMemb_AcceptedView_Callback: " << eid << " (departed)";
        }
    }

    if(mFirstViewInstalled == 0)
    {
        // block_client();
        // mXferInProgress = true;

        Merge_Join_Callback(vd.myEndpID);

        mFirstViewInstalled = 1;
    }

    // see who is new and will expect a state from us
    if(vd.members.size() > 1 && vd.newMembers.size() > 0)
    {
        Maestro_Message state_msg;

        // here the application must fill in the fragment of the current state
        // that is owned by itself
        Merge_GetState_Callback(mMyView.myEndpID, state_msg);

        int token = MAESTRO_MERGE_STATE_CAST;
        state_msg << token;

        Maestro_MsgSendView sendView = MAESTRO_MSG_SEND_CURRENT_VIEW;
        Maestro_GroupMember::cast(state_msg, sendView);

        AVANGO_LOG(logger, logging::DEBUG, "grpMemb_AcceptedView_Callback: casted state.")
    }

    // for all departed members tell the application to remove that state fragment
    for(unsigned int k = 0; k < vd.departedMembers.size(); k++)
    {
        Merge_RemoveState_Callback(vd.departedMembers[k]);
    }

    // Send out xfer-delayed messages.
    mXferInProgress = false;
    deliverDelayedMessages();

    // Update private view data.
    mMyView = vd;

    // Invoke user upcall.
    Merge_AcceptedView_Callback(vd, msg);

    unblock_client();

#ifdef AVANGO_DEBUG
    // dump("av::MaestroMerge::grpMemb_AcceptedView_Callback: leave");
#endif
}

void av::MaestroMerge::grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData& viewData, Maestro_Message& view_msg)
{
    MaestroMergeViewData vd(viewData);

    // Invoke user callback first, in case layers above want to push headers
    // in the view message.
    Merge_ViewMsg_Callback(vd, view_msg);

    vd.stateCounter = mMyView.stateCounter;

    // Push this layer's headers as the view message.
    vd.splice_out(view_msg);

    // The new state transfer magic from roy
    if(vd.myRank == 0)
    {
        mValidStateGuro = 0;
        mMaxStateCounter = -1;
        mNumStates = 0;
    }
}

void av::MaestroMerge::grpMemb_ViewStateInfo_Callback(Maestro_EndpID& origin, Maestro_GrpMemb_ViewData& viewData, Maestro_Message& view_msg, Maestro_Message& next_view_msg, int& final)
{
    int upStateCounter;

    final = 0;
    if(viewData.myRank == 0)
    {
        mNumStates++;
        view_msg >> upStateCounter;
        if(upStateCounter > mMaxStateCounter)
        {
            mValidStateGuro = 1;
            mMaxStateCounter = upStateCounter;
            mStateMsg = view_msg;
            mStateGuro = origin;
        }
        if(mNumStates == viewData.nmembers)
        {
            next_view_msg <<= mStateMsg;
            next_view_msg << mMaxStateCounter;
            final = 1;
        }
    }
}

void av::MaestroMerge::Merge_Heartbeat_Callback(unsigned) {}

/********************** grpMemb_ReceiveCast_Callback ********************/

void av::MaestroMerge::grpMemb_ReceiveCast_Callback(Maestro_EndpID& origin, Maestro_Message& msg)
{
    // ignore any casts before we properly joined the group
    if(!mFirstViewInstalled)
    {
        AVANGO_LOG(logger, logging::INFO, "grpMemb_ReceiveCast_Callback: received cast before join.")
        return;
    }

    if(origin == mMyView.myEndpID)
    {
        // don't accept messages from myself
        return;
    }

    bool non_member = false;
    if(!mMyView.members.contains(origin))
    {
        // discard stray messages from departed members
        AVANGO_LOG(logger, logging::INFO, "grpMemb_ReceiveCast_Callback: received cast from nonmember.")

        // return;
        non_member = true;
    }

    int mtype_int;
    msg >> mtype_int;
    Maestro_MergeMsgType mtype = Maestro_MergeMsgType(mtype_int);

    Maestro_EndpList dests;

    switch(mtype)
    {
    case MAESTRO_MERGE_CAST:
        Merge_ReceiveCast_Callback(origin, msg);
        if(non_member)
        {
            AVANGO_LOG(logger, logging::INFO, "grpMemb_ReceiveCast_Callback: was: MAESTRO_MERGE_CAST")
        }
        break;

    case MAESTRO_MERGE_STATE_CAST:
        receive_state(origin, msg);
        if(non_member)
        {
            AVANGO_LOG(logger, logging::INFO, "grpMemb_ReceiveCast_Callback: was: MAESTRO_STATE_CAST")
        }
        break;

    default:
        AVANGO_LOG(logger, logging::ERROR, "grpMemb_ReceiveCast_Callback: Bad message type")
    }
}

/*********************** grpMemb_ReceiveSend_Callback *******************/

void av::MaestroMerge::grpMemb_ReceiveSend_Callback(Maestro_EndpID& origin, Maestro_Message& msg)
{
    int mtype_int;
    msg >> mtype_int;
    Maestro_MergeMsgType mtype = Maestro_MergeMsgType(mtype_int);

    switch(mtype)
    {
    case MAESTRO_MERGE_SEND:
        Merge_ReceiveSend_Callback(origin, msg);
        break;

    default:
        AVANGO_LOG(logger, logging::ERROR, "grpMemb_ReceiveSend_Callback: Bad message class")
    }
}

void av::MaestroMerge::grpMemb_Block_Callback()
{
    block_client();
    mXferInProgress = true;
}

void av::MaestroMerge::grpMemb_Exit_Callback()
{
    Merge_Exit_Callback();
    mXferInProgress = false;
    unblock_client();
}

/********************************* stuff ********************************/

void av::MaestroMerge::block_client()
{
    AVANGO_LOG(logger, logging::TRACE, "block_client: enter")

    if(mFirstViewInstalled)
    {
        if(!mBlocked)
        {
            Merge_Block_Callback();
            mBlocked = true;
        }
    }
    AVANGO_LOG(logger, logging::TRACE, "block_client: leave")
}

void av::MaestroMerge::unblock_client()
{
    AVANGO_LOG(logger, logging::TRACE, "unblock_client: enter")
    if(mBlocked)
    {
        Merge_UnBlock_Callback();
        mBlocked = false;
    }
    AVANGO_LOG(logger, logging::TRACE, "unblock_client: leave")
}

void av::MaestroMerge::receive_state(Maestro_EndpID& origin, Maestro_Message& state_msg)
{
    AVANGO_LOG(logger, logging::DEBUG, boost::str(boost::format("av::Maestro_Merge::receive_state: received state from %1%") % origin))

    // must deliver to application here
    Merge_SetState_Callback(origin, state_msg);
}

/**************************** ViewData **********************************/

av::MaestroMergeViewData::MaestroMergeViewData(Maestro_GrpMemb_ViewData& gmView) : Maestro_GrpMemb_ViewData(gmView) {}

av::MaestroMergeViewData::MaestroMergeViewData() {}

void av::MaestroMergeViewData::blend_in(Maestro_Message& msg)
{
    // fill in the merge specific data from the view message
    msg >> stateCounter;
}

void av::MaestroMergeViewData::splice_out(Maestro_Message& msg)
{
    // extract the merge specific data into the new view message
    msg << stateCounter;
}

/************************* Delaying Message *****************************/

void av::MaestroMerge::delayMessage(Maestro_Message& msg)
{
    DelayMsg dm;
    dm._msg = msg;
    dm._use_view = false;
    dm._use_dest = false;
    mDelayedMsgs.push_back(dm);
}

void av::MaestroMerge::delayMessage(Maestro_Message& msg, Maestro_MsgSendView& sendView)
{
    DelayMsg dm;
    dm._msg = msg;
    dm._view = sendView;
    dm._use_view = true;
    dm._use_dest = false;
    mDelayedMsgs.push_back(dm);
}

void av::MaestroMerge::delayMessage(Maestro_EndpID& dest, Maestro_Message& msg)
{
    DelayMsg dm;
    dm._msg = msg;
    dm._dest = dest;
    dm._use_view = false;
    dm._use_dest = true;
    mDelayedMsgs.push_back(dm);
}

void av::MaestroMerge::delayMessage(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView)
{
    DelayMsg dm;
    dm._msg = msg;
    dm._dest = dest;
    dm._view = sendView;
    dm._use_view = true;
    dm._use_dest = true;
    mDelayedMsgs.push_back(dm);
}

void av::MaestroMerge::deliverDelayedMessages()
{
    DelayMsgVec::iterator i;
    for(i = mDelayedMsgs.begin(); i != mDelayedMsgs.end(); i++)
    {
        DelayMsg& dm = *i;
        if(dm._use_dest)
        {
            if(dm._use_view)
            {
                Maestro_GroupMember::send(dm._dest, dm._msg, dm._view);
            }
            else
            {
                Maestro_MsgSendView sendView = MAESTRO_MSG_SEND_CURRENT_VIEW;
                Maestro_GroupMember::send(dm._dest, dm._msg, sendView);
            }
        }
        else
        {
            if(dm._use_view)
            {
                Maestro_GroupMember::cast(dm._msg, dm._view);
            }
            else
            {
                Maestro_MsgSendView sendView = MAESTRO_MSG_SEND_CURRENT_VIEW;
                Maestro_GroupMember::cast(dm._msg, sendView);
            }
        }
    }
    mDelayedMsgs.clear();
}

const Maestro_EndpID& av::MaestroMerge::eid() { return mMyView.myEndpID; }
