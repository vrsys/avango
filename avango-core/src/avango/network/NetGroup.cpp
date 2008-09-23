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

#include "NetGroup.h"

#include <stdexcept>


#include <avango/ensemble/maestro/Maestro_CSX.h>

#include <avango/Distributed.h>
#include <avango/NetNode.h>
#include <avango/Msg.h>
#include <avango/Logger.h>

#include <avango/Config.h>

namespace
{
  av::logging::Logger& logger(av::logging::Logger::getLogger("av::NetGroup"));
}

// create a new group member
av::NetGroup::NetGroup(NetNode* net_node, Maestro_ClSv_Options &options)
  : MaestroMerge(options),
    mNetNode(net_node),
    mUpcallSerializer()
{

  if (!net_node)
  {
    throw std::invalid_argument("net_node may not be 0");
  }
}

// destroy this group member and leave the group
av::NetGroup::~NetGroup()
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::~NetGroup: ";
#endif
}

av::UpcallSerializer&
av::NetGroup::upcallSerializer()
{
  return mUpcallSerializer;
}

void
av::NetGroup::cast(av::Msg& msg)
{
  Maestro_Message maestro_msg;
  maestro_msg.write(msg.getBuffer(), msg.getSize());
  MaestroMerge::cast(maestro_msg);
}

void
av::NetGroup::send(const std::string &destEID, av::Msg& msg)
{
  Maestro_Message maestro_msg;
  maestro_msg.write(msg.getBuffer(), msg.getSize());

  hot_endpt_t endpID;
  ::strncpy(endpID.name, destEID.c_str(), HOT_ENDP_MAX_NAME_SIZE);
  Maestro_EndpID maestro_endpID(endpID);
  MaestroMerge::send(maestro_endpID, maestro_msg);
}

void
av::NetGroup::Merge_Join_Callback(Maestro_EndpID& /*myeid*/)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_Join_Callback: ";
#endif
  // maestro_eid dummy(myeid);
  // _net_node->joined(dummy);

  //fp_join_upcall* join = new fp_join_upcall(myeid);
  //_upcall_serializer.make_upcall(join);
}

// State transfer callbacks
void
av::NetGroup::Merge_SetState_Callback(Maestro_EndpID& fragment, Maestro_Message& stateMessage)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_SetState_Callback: ";
#endif

  // _net_node->set_state_fragment(fragment, state_message);
  // _net_node->queue_state_fragment(fragment, state_message);

  Msg av_msg;
  av_msg.resize(stateMessage.getPos());
  stateMessage.read(av_msg.getBuffer(), av_msg.getSize());

  boost::shared_ptr<SetStateUpcall> set_state(new SetStateUpcall(fragment.getHotEndpt().name, av_msg));
  mUpcallSerializer.makeUpcall(set_state);
}

void
av::NetGroup::Merge_GetState_Callback(Maestro_EndpID& fragment, Maestro_Message& stateMessage)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_GetState_Callback: ";
#endif

  boost::shared_ptr<GetStateUpcall> get_state(new GetStateUpcall(fragment.getHotEndpt().name));

  mUpcallSerializer.makeUpcall(get_state);

  stateMessage.reset();

  stateMessage.write(get_state->stateMsg().getBuffer(), get_state->stateMsg().getSize());

}

void
av::NetGroup::Merge_RemoveState_Callback(Maestro_EndpID& fragment)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_RemoveState_Callback: ";
#endif

  boost::shared_ptr<RemoveStateUpcall> remove_state(new RemoveStateUpcall(fragment.getHotEndpt().name));
  mUpcallSerializer.makeUpcall(remove_state);
}

// callbacks for messages
void
av::NetGroup::Merge_ReceiveCast_Callback(Maestro_EndpID &/*origin*/, Maestro_Message &msg)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_ReceiveCast_Callback: ";
#endif

  // _net_node->receive_message(origin, msg);
  Msg av_msg;
  av_msg.resize(msg.getPos());
  msg.read(av_msg.getBuffer(), av_msg.getSize());

  boost::shared_ptr<MessageUpcall> message(new MessageUpcall(av_msg));
  mUpcallSerializer.makeUpcall(message);
}

void
av::NetGroup::Merge_ReceiveSend_Callback(Maestro_EndpID &/*origin*/, Maestro_Message &msg)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_ReceiveSend_Callback: ";
#endif

  // _net_node->receive_message(origin, msg);
  Msg av_msg;
  av_msg.resize(msg.getPos());
  msg.read(av_msg.getBuffer(), av_msg.getSize());

  boost::shared_ptr<MessageUpcall> message(new MessageUpcall(av_msg));
  mUpcallSerializer.makeUpcall(message);
}

// view change callbacks
void
av::NetGroup::Merge_AcceptedView_Callback(MaestroMergeViewData &view_data, Maestro_Message &msg)
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_AcceptedView_Callback: ";
#endif
  Msg av_msg;
  av_msg.resize(msg.getPos());
  msg.read(av_msg.getBuffer(), av_msg.getSize());

  std::vector<std::string> members;
  for (int member = 0; member < view_data.members.size(); ++member) {
    members.push_back(view_data.members[member].getHotEndpt().name);
  }

  std::vector<std::string> new_members;
  for (int new_member = 0; new_member < view_data.newMembers.size(); ++new_member) {
    new_members.push_back(view_data.newMembers[new_member].getHotEndpt().name);
  }

  std::vector<std::string> departed_members;
  for (int departed_member = 0; departed_member < view_data.departedMembers.size(); ++departed_member) {
    departed_members.push_back(view_data.departedMembers[departed_member].getHotEndpt().name);
  }


  boost::shared_ptr<AcceptedViewUpcall> view(new AcceptedViewUpcall(members, new_members, departed_members, av_msg));
  mUpcallSerializer.makeUpcall(view);
}

void
av::NetGroup::Merge_Block_Callback()
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_Block_Callback: ";
#endif

  boost::shared_ptr<BlockUpcall> block(new BlockUpcall);
  mUpcallSerializer.makeUpcall(block);
}

void
av::NetGroup::Merge_UnBlock_Callback()
{
#ifdef AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_UnBlock_Callback: ";
#endif

  boost::shared_ptr<UnblockUpcall> unblock(new UnblockUpcall);
  mUpcallSerializer.makeUpcall(unblock);
}

// The member has left the group
void
av::NetGroup::Merge_Exit_Callback()
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "av::NetGroup::Merge_Exit_Callback: ";
#endif

  boost::shared_ptr<ExitUpcall> exit(new ExitUpcall);
  mUpcallSerializer.makeUpcall(exit);
}
