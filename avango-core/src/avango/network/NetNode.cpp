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

#include "avango/NetNode.h"

#include <iostream>
#include <sstream> // std::ostringstream
#include <iterator>

#ifndef ZMQ_DISTRIBUTION_SUPPORT
#include <avango/ensemble/maestro/Maestro.h>
#endif

#include <avango/logging/Logger.h>
#include <avango/Distributed.h>
#include <avango/NetInfo.h>

#include "Helper.h"
//#include "NetGroup.h"

#include <avango/Config.h>

#if defined(ZMQ_DISTRIBUTION_SUPPORT)
#include "NetNodeClient.h"
#include "NetNodeServer.h"
#include <boost/tokenizer.hpp>
#endif

namespace
{
  av::Logger &logger(av::getLogger("av::NetNode"));

#if defined(ZMQ_DISTRIBUTION_SUPPORT)
  av::NetNodeClient* gClient = 0;
  av::NetNodeServer* gServer = 0;
  const std::string  gClientEndpoint("{Endpt:127.0.0.2:34818:215:0}");
  const std::string  gServerEndpoint("{Endpt:127.0.0.2:34818:215:0}");
#endif
}

/* static */ const unsigned int av::NetNode::sCreateMsg         = 42;
/* static */ const unsigned int av::NetNode::sUpdateMsg         = 43;
/* static */ const unsigned int av::NetNode::sDeleteMsg         = 44;
/* static */ const unsigned int av::NetNode::sPackedMsg         = 50;
/* static */ const unsigned int av::NetNode::sStateTransferMsg  = 45;

av::NetNode::NetNode()
  : mMember(0),
    mNumMembers(0),
    mWellKnownObjects(),
    mBlocked(false)
{}

av::NetNode::~NetNode()
{
  leave();
}

void
av::NetNode::join(const std::string& groupName)
{
#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::NetNode: joining group: " << groupName;
#endif

#ifdef ZMQ_DISTRIBUTION_SUPPORT
  bool isServer;
  std::string hostName;
  std::string port;
  uint64_t serverHWM = 2;
  {
    // from http://stackoverflow.com/questions/541561/using-boost-tokenizer-escaped-list-separator-with-different-parameters
    std::string gn(groupName);

    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    boost::escaped_list_separator<char> Separator( ' ', '|' );
    Tokenizer tok( gn, Separator );
    unsigned numTokens = 0;
    for (Tokenizer::iterator iter = tok.begin(); iter != tok.end(); ++iter) {
      ++numTokens;
    }
    if (3 == numTokens) {
      Tokenizer::iterator iter = tok.begin();
      isServer = ("AVSERVER" == *iter);
      ++iter;
      hostName = *iter;
      ++iter;
      port = *iter;
    } else if (4 == numTokens) {
      Tokenizer::iterator iter = tok.begin();
      isServer = ("AVSERVER" == *iter);
      ++iter;
      hostName = *iter;
      ++iter;
      port = *iter;
      ++iter;
      serverHWM = atoi((*iter).c_str());
    } else {
      std::stringstream msg;
      msg << "ERROR in av::NetNode::join(const std::string& groupName), could not join - invalid CONFIG";
      throw (std::runtime_error(msg.str()));
    }

  }

  if (isServer) {
    gServer = new NetNodeServer(hostName,port, this, gClientEndpoint, gServerEndpoint, serverHWM);
    joined(gServerEndpoint);

    av::Msg av_msg;
    getStateFragment(gServerEndpoint,av_msg);
    setStateFragment(gClientEndpoint,av_msg);
  } else {
    gClient = new NetNodeClient(hostName,port, this, gClientEndpoint, gServerEndpoint);
    joined(gClientEndpoint);
    gClient->start();
  }
  //handleNetworkSends();
  //handleNetworkReceives();

  mIdCounter = 0;

#else
  leave();

  Maestro_CSX_Options ops;

  // heartbeat is once per second
  ops.heartbeatRate = 1000;
  ops.groupName = (char*) groupName.c_str();
  ops.transports = "DEERING";
  ops.properties = "Gmp:Sync:Heal:Switch:Frag:Suspect:Flow:Total";
  ops.params = "suspect_max_idle=3:int;suspect_sweep=1.000:time";
  ops.mbrshipType = MAESTRO_SERVER;
  ops.xferType = MAESTRO_ATOMIC_XFER;

  // get the command-line options from the database
  ops.argv = getEnsOptionsArgv();

  // check for usage of a groupd and set the corresponding maestro flag
  std::string dummy_value;
  if (getEnsOption("-groupd", dummy_value))
    ops.groupdFlag = true;

  if (getEnsOption("-properties", dummy_value))
    ops.properties = const_cast<char*> (dummy_value.c_str());

  if (getEnsOption("-verbose", dummy_value)) {
    if (getEnsOption("-modes", dummy_value))
      std::cout << " modes       : " << dummy_value << std::endl;
    if (getEnsOption("-deering_port", dummy_value))
      std::cout << " deering_port: " << dummy_value << std::endl;
    if (getEnsOption("-gossip_port", dummy_value))
      std::cout << " gossip_port : " << dummy_value << std::endl;
    if (getEnsOption("-gossip_hosts", dummy_value))
      std::cout << " gossip_hosts: " << dummy_value << std::endl;
    if (getEnsOption("-properties", dummy_value))
      std::cout << " properties  : " << dummy_value << std::endl;
    if (getEnsOption("-multiread", dummy_value))
      std::cout << " multiread   : " << (dummy_value.empty () ? "on" : dummy_value) << std::endl;
  }

  mMember = new NetGroup(this, ops);
  mMember->join();
  mEID =  mMember->eid().getHotEndpt().name;
  joined(mEID);

  handleNetworkSends();
  handleNetworkReceives();

#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::NetNode: "
            << "joined group '"
            << groupName.c_str()
            << "', my eid: "
            << netEID();
#endif

  mIdCounter = 0;

#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::join: was called from: " << "  pid: " << getpid();
  logger.debug() << "av::NetNode::join: completed.";
#endif

#endif // #ifdef ZMQ_DISTRIBUTION_SUPPORT
}

void
av::NetNode::leave()
{
  if (mMember) {
#ifdef AVANGO_DEBUG
    logger.debug() << "av::NetNode::leave: ";
#endif
    // destroy or donate all objects created by ourselves
    delete mMember; // leave group
    mMember = 0;
  }
}

av::NetID
av::NetNode::generateUniqueId()
{
  assert(onAir());
#ifdef ZMQ_DISTRIBUTION_SUPPORT
  if(gServer)
     return NetID(gServerEndpoint, ++mIdCounter);
  else
     return NetID(gClientEndpoint, ++mIdCounter);
#else
  return NetID(netEID(), ++mIdCounter);
#endif
  assert(mIdCounter > 0); // detect overflow
}

bool
av::NetNode::onAir() const
{
#ifdef ZMQ_DISTRIBUTION_SUPPORT
  return true;
#else
  return (mMember != 0);
#endif
}

const std::string&
av::NetNode::netEID() const
{
  return mEID;
}

// handle network callbacks.
// this gives the network process time to things
void
av::NetNode::handleNetworkSends()
{
  // consume_received_messages();
#ifdef ZMQ_DISTRIBUTION_SUPPORT
  if (0 != gServer) {
    notifyGroup();
  }
#else
  if (!mBlocked)
    notifyGroup();
#endif
}

void
av::NetNode::handleNetworkReceives()
{
#ifdef ZMQ_DISTRIBUTION_SUPPORT
  if (gClient) {
    consumeReceivedMessages();
  }
#else
  while (mMember && mMember->upcallSerializer().handleNextUpcall(this))
    ;
#endif
}

void
av::NetNode::queueStateFragment(const std::string& fragment, av::Msg& stateMsg)
{
  mStateUpdates.push_back(std::pair<std::string, Msg>(fragment, stateMsg));
}

void
av::NetNode::queueRemoveFragment(const std::string& fragment)
{
  mStateRemoves.push_back(fragment);
}

void
av::NetNode::applyStateFragments()
{
#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::applyStateFragments:" ;
#endif
  {
    StateUpdateList::iterator current(mStateUpdates.begin());

    while (current != mStateUpdates.end()) {
      setStateFragment((*current).first, (*current).second);
      ++current;
    }

    mStateUpdates.clear();
  }

  {
    StateRemoveList::iterator current(mStateRemoves.begin());

    while (current != mStateRemoves.end()) {
      removeStateFragment(*current);

      ++current;
    }

    mStateRemoves.clear();
  }

}

// registration of well known nodes. only when registered, well-known nodes can
// receive and send update messages. because they are well known, creation and
// destruction is not announced via the net.
void
av::NetNode::registerWellKnown(const Link<Distributed>& obj, const NetID& id)
{
#ifdef AVANGO_DEBUG
  //LOG_TRACE(logger) << "av::NetNode::register_well_known: "
  logger.debug()<< "av::NetNode::register_well_known: "
  << "well-known id: "
  << id;
#endif

  // make sure we have already joined a group
  AV_ASSERT(onAir());

  if (!obj->isDistributed()) {
    // just check wether this is valid well known id
    AV_ASSERT(id.isWellKnown());

    // make also sure this is unique
    if (mWellKnownObjects.find(id) != mWellKnownObjects.end()) {
      logger.info() << "register_well_known: "
                    << "<%s> of type <%s> already known!",
        getName(obj), obj->getTypeId().getName().c_str();

      return;
    }

    // add to the map
    mWellKnownObjects[id] = obj;

    // setup the distribution info structure
    obj->setNetInfo(new NetInfo (id, this));
  } else
    logger.info() << "register_well_known: "
                  << "<%s> of type <%s> already distributed!",
      getName(obj), obj->getTypeId().getName().c_str();
}

void
av::NetNode::unregisterWellKnown(const Link<Distributed>& obj)
{
  // make sure we have already joined a group
  AV_ASSERT(onAir());

  if (obj->isDistributed()) {
#ifdef AVANGO_DEBUG
    logger.debug()<< "av::NetNode::unregister_well_known: "
              << "well-known id: "
              << obj->netID();
#endif

    // make sure the obj is really registered
    NetIDDstMap::iterator i = mWellKnownObjects.find(obj->netID());
    AV_ASSERT(i != mWellKnownObjects.end());

    // remove the network info structure
    delete obj->getNetInfo();
    obj->setNetInfo(0);

    // remove object from the map
    mWellKnownObjects.erase(i);
  } else
    logger.info() << "unregister_well_known: "
                  << "<%s> of type <%s> not yet distributed!",
      getName(obj), obj->getTypeId().getName().c_str();
}

// registration of distributables. an object can only be distributed in one
// group at a time. all other group members will be able to see and modify a registered
// object. an unregistered object is only visible to the creator.
void
av::NetNode::registerObj(const Link<Distributed>& obj)
{
  registerObj(obj, generateUniqueId());
}

// if an object is created by someone else we use this version
void
av::NetNode::registerObj(const Link<Distributed>& obj, const NetID& id)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::register_obj: "
            << "id: "
            << id;
#endif

  // make sure we have already joined a group
  AV_ASSERT(onAir());

  // make sure this object is not already distributed
  if (!obj->isDistributed()) {
    AV_ASSERT(!id.isNull());
    AV_ASSERT(!id.isWellKnown());

    // take the supplied id and
    // setup the distribution info structure
    obj->setNetInfo(new NetInfo (id, this));

    // add to the map
    mObjectMap.registerObj(obj);

    // notify the object
    obj->becomingDistributed();
  } else
    logger.info() << "register_obj: "
                  << "<%s> of type <%s> already distributed!",
                  getName(obj), obj->getTypeId().getName().c_str();
}

void
av::NetNode::unregisterObj(const Link<Distributed>& obj)
{
  // make sure we have already joined a group
  AV_ASSERT(onAir());

  // make sure this object is distributed
  if (obj->isDistributed()) {

#ifdef AVANGO_DEBUG
    logger.debug()<< "av::NetNode::unregister_obj: "
              << "id: "
              << obj->netID();
#endif

    // first remove object from the map ...
    mObjectMap.unregisterObj(obj);

    // notify the object
    obj->becomingUndistributed();
  } else
    logger.info() << "unregister_obj: "
                  << "<%s> of type <%s> not yet distributed!",
                  getName(obj), obj->getTypeId().getName().c_str();
}

void
av::NetNode::notifyGroup()
{
  // send all changes since the last send
  // order is, in fact, important; use only from within
  notifyCreations();
  notifyUpdates();
  notifyDeletes();
}

// announce a newly distributed object
void
av::NetNode::notifyCreations()
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::notify_creations: "
                    << "creations pending: "
                    << mPendingCreations.size();
#endif

  if (!mPendingCreations.size()) {
    return;
  }

  typedef std::vector<DistributedSet::value_type> av_dst_vector;

  av_dst_vector tmp(mPendingCreations.begin(), mPendingCreations.end());

  // erase the pending objects
  mPendingCreations.clear();

  Msg p_msg;
  p_msg.setNetNode(this);

  int msg_count(0);

  av_dst_vector::reverse_iterator i;

  // follow it by an absolute update message to fill in the fields
  // because the creations go first, there is no possibility of dangling
  // references between newly created objects
  for (i=tmp.rbegin(); i!=tmp.rend(); ++i) {
    p_msg.setType(Msg::absolute);
    makeUpdateMessage(p_msg, *i);
    ++msg_count;
  }

  // send all of the creation messages to the rest of the group
  for (i=tmp.rbegin(); i!=tmp.rend(); ++i) {
    p_msg.setType(Msg::relative);
    makeCreateMessage(p_msg, *i);
    ++msg_count;
  }

  av_pushMsg(p_msg, msg_count);
  // p_msg << packed_msg;
  av_pushMsg(p_msg, sPackedMsg);

#ifdef ZMQ_DISTRIBUTION_SUPPORT
  gServer->cast(p_msg);
#else
  mMember->cast(p_msg);
#endif
}

// announce changes to registered distributed objects
void
av::NetNode::notifyUpdates()
{
#ifdef AVANGO_DEBUG
    logger.debug()<< "av::NetNode::notify_updates: "
                      << "updates pending: "
                      << mPendingUpdates.size();
#endif

  if (!mPendingUpdates.size())
    return;

  typedef std::vector<DistributedSet::value_type> av_dst_vector;

  av_dst_vector tmp(mPendingUpdates.begin(), mPendingUpdates.end());

  // erase the pending objects
  mPendingUpdates.clear();

  Msg p_msg;
  p_msg.setNetNode(this);

  int        msg_count(0);

  av_dst_vector::reverse_iterator i;

  for (i=tmp.rbegin(); i!=tmp.rend(); ++i) {
    p_msg.setType(Msg::relative);
    makeUpdateMessage(p_msg, *i);
    ++msg_count;
  }

  av_pushMsg(p_msg,msg_count);
  av_pushMsg(p_msg, sPackedMsg);

#ifdef ZMQ_DISTRIBUTION_SUPPORT
  gServer->cast(p_msg);
#else
  mMember->cast(p_msg);
#endif
}

// announce deletion of a distributed object
void
av::NetNode::notifyDeletes()
{
#ifdef AVANGO_DEBUG
    logger.debug()<< "av::NetNode::notify_deletes: "
                      << "deletes pending: "
                      << mPendingDeletes.size();
#endif

  if (!mPendingDeletes.size())
    return;

  typedef std::vector<DistributedSet::value_type> av_dst_vector;

  av_dst_vector tmp(mPendingDeletes.begin(), mPendingDeletes.end());

  // erase the pending objects
  mPendingDeletes.clear();

  Msg p_msg;
  p_msg.setNetNode(this);

  int        msg_count(0);

  av_dst_vector::reverse_iterator i;

  for (i=tmp.rbegin(); i!=tmp.rend(); ++i) {
    makeDeleteMessage(p_msg, *i);
    ++msg_count;
  }

  av_pushMsg(p_msg,msg_count);
  av_pushMsg(p_msg, sPackedMsg);

#ifdef ZMQ_DISTRIBUTION_SUPPORT
  gServer->cast(p_msg);
#else
  mMember->cast(p_msg);
#endif
}

// look for a distributed node which matches the specified id
// this is meant to be used to resolve object ids which come in over the net
// you must be sure that the object exists and is properly registered
bool
av::NetNode::lookup(const NetID& id, Link<Distributed>& result)
{
  // distinguish between well-known and normal objects
  if (id.isWellKnown()) {

#ifdef AVANGO_DEBUG
    {
      logger.debug()<< "av::NetNode::lookup: "
                        << "searching w/in "
                        << mWellKnownObjects.size()
                        << " well-known objects";

      NetIDDstMap::iterator i = mWellKnownObjects.begin();

      while (i != mWellKnownObjects.end()) {
        logger.debug()<< "av::NetNode::lookup: "
                          << (*i).first
                          << ":"
                          << (*i).second->netID();
        ++i;
      }
    }
#endif

    NetIDDstMap::iterator i = mWellKnownObjects.find(id);

    if (i == mWellKnownObjects.end()) {
#ifdef AVANGO_DEBUG
      logger.debug()<< "av::NetNode::lookup: "
                        << "failed for: "
                        << id;
#endif
      return false;
    } else {
      result = (*i).second;
#ifdef AVANGO_DEBUG
      logger.debug()<< "av::NetNode::lookup: "
                         << "succeeded for: "
                         << id;
#endif
      return true;
    }
  } else {
    return mObjectMap.lookup(id, result);
  }
}

void
av::NetNode::joined(const std::string& myeid)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::joined: "
                    << "called";
#endif
  // add a slot for ourselfes
  mObjectMap.addSlot(myeid);
  // tell our clients we joined
  _join(myeid);
}

void
av::NetNode::block()
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::block: "
                    << "called";
#endif
  mBlocked = true;
}

void
av::NetNode::unblock()
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::unblock: "
                    << "called";
#endif
  mBlocked = false;
}

void
av::NetNode::setStateFragment(const std::string& fragment, av::Msg& stateMsg)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::set_state_fragment: from "
                    << fragment << " size: "
                    << stateMsg.getSize() << " bytes.";
#endif

  // this is for the Maestro_Merge_Simple test
  if (mObjectMap.slotExists(fragment))
  {
    return;
  }

  // we need to reset our old state fragment before we receive the new state fragment
  if (mObjectMap.slotExists(fragment))
  {
    mObjectMap.emptySlot(fragment);
  } else
  {
    mObjectMap.addSlot(fragment);
  }

  // decode the state message
  Msg sm(stateMsg);
  sm.setNetNode(this);

  // first the creations
  int num_objects, i;

  av_popMsg(sm, num_objects);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::set_state_fragment: create messages: "
                    << num_objects;
#endif

  for (i=0; i<num_objects; ++i)
  {
    consumeMessage(sm);
  }
  // then the updates
  av_popMsg(sm, num_objects);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::set_state_fragment: update messages: "
                    << num_objects;
#endif

  for (i=0; i<num_objects; ++i)
  {
    consumeMessage(sm);
  }

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::set_state_fragment: setting state fragment for "
                    << fragment;
#endif

  // finally whatever the client needs
  _setStateFragment(fragment, sm);
}

void
av::NetNode::getStateFragment(const std::string& fragment, av::Msg& stateOutMsg)
{
  // a state has been requested. send it.
  Msg state_msg(stateOutMsg);
  state_msg.setNetNode(this);

  // request the current state
  std::vector<Link<Distributed> >::const_iterator i;
  std::vector<Link<Distributed> >                 state;

  mObjectMap.lookup(fragment, state);

  // pack into one message

  // let any client add something to the state
  _getStateFragment(fragment, state_msg);

  // then updates for the same objects
  state_msg.setType(Msg::absolute);

  for (i=state.begin(); i!=state.end(); ++i)
  {
    makeUpdateMessage(state_msg, *i);
  }

  av_pushMsg(state_msg, state.size());

  // first the create messages
  for (i=state.begin(); i!=state.end(); ++i)
  {
    makeCreateMessage(state_msg, *i);
  }

  av_pushMsg(state_msg, state.size());

  // ultra nasty hack
  stateOutMsg = state_msg.clone();

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::get_state_fragment: for "
                    << fragment << " size: "
                    << state_msg.getSize() << " bytes.";
  logger.debug()<< "av::NetNode::get_state_fragment: create messages: " << state.size();
  logger.debug()<< "av::NetNode::get_state_fragment: update messages: " << state.size();
#endif
}

void
av::NetNode::removeStateFragment(const std::string& fragment)
{
  if (mObjectMap.slotExists(fragment))
  {
    mObjectMap.removeSlot(fragment);
  }

  // let any client add something to the state
  _removeStateFragment(fragment);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::remove_state_fragment: for "
                    << fragment;
#endif
}

void
av::NetNode::receiveMessage(const std::string &origin, av::Msg& msg)
{
  // stack up the received messages in any case
  // the message needs to carry a reference to the NetNode
  // which received it.
  boost::mutex::scoped_lock lock(mMessageMutex);

#ifdef ZMQ_DISTRIBUTION_SUPPORT
  // we need to reset our old state fragment before we receive the new state fragment
  if (!mObjectMap.slotExists(origin))
  {
    logger.debug() << "ALARM: in av::NetNode::receiveMessage !!!!!!!!!!!!!!!! slot was empty for origin " << origin;
    mObjectMap.addSlot(origin);
  }
#endif

  Msg av_msg(msg);
  av_msg.setNetNode(this);

  mReceivedMessages.push_back(av_msg);
}

void
av::NetNode::consumeReceivedMessages()
{
#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::consume_received_messages: "
                 << mReceivedMessages.size() << " messages";
#endif

  // now, just consume all the received messages
  boost::mutex::scoped_lock lock(mMessageMutex);

  while (mReceivedMessages.size()) {
    consumeMessage(mReceivedMessages.front());
    mReceivedMessages.pop_front();
  }
}

void
av::NetNode::exitCompleted()
{
#ifdef AVANGO_DEBUG
  logger.debug() << "av::NetNode::exit_completed: ";
#endif
}

void
av::NetNode::acceptNewView(const std::vector<std::string>& members,
                           const std::vector<std::string>& newMembers,
                           const std::vector<std::string>& departedMembers,
                           av::Msg& msg)
{
  // set our own eid
  mNumMembers = members.size();


  // notify derived classes
  _acceptNewView(members, newMembers, departedMembers);
}

// make an existing object distributed, make it local again, update it's state
void
av::NetNode::distributeObject(av::Link<av::Distributed> obj)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::distribute_object: '"
                    << getName (obj)
                    << "' w/ type id "
                    << obj->getTypeId().getName().c_str();
#endif

  if (!obj->getTypeId().isDistributable()) {
    logger.info() << "distribute_object: "
                  << "<%s> of type <%s> not distributable!",
      getName(obj), obj->getTypeId().getName().c_str();

    return;
  }

  registerObj(obj);
  mPendingCreations.insert(obj);
}

void
av::NetNode::undistributeObject(av::Link<av::Distributed> obj)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::undistribute_object: '"
                    << getName (obj)
                    << "' w/ type id "
                    << obj->getTypeId().getName().c_str();
#endif

  if (!obj->getTypeId().isDistributable()) {
    logger.info() << "undistribute_object: "
                  << "<%s> of type <%s> not distributable!",
      getName(obj), obj->getTypeId().getName().c_str();

    return;
  }

  unregisterObj(obj);
  mPendingDeletes.insert(obj);
}

// make an existing object distributed, make it local again, update it's state
void
av::NetNode::distributeFieldContainer(av::Link<av::FieldContainer> obj)
{
  distributeObject(Link<Distributed>(obj));
}

void
av::NetNode::undistributeFieldContainer(av::Link<FieldContainer> obj)
{
  undistributeObject(Link<Distributed>(obj));
}


void
av::NetNode::updateDistributedObject(const av::Link<av::Distributed>& obj)
{

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::update_distributed_object: '"
                    << getName (obj)
                    << "' w/ type id "
                    << obj->getTypeId().getName().c_str();
#endif

  Link<Distributed> dummy;

  // assert(lookup(obj->netID(), dummy));

  if (lookup(obj->netID(), dummy)) {
    mPendingUpdates.insert(obj);
  } else {
    logger.info() << "update_distributed_object: "
                  << "<%s> of type <%s> non-existent!",
      getName(obj), obj->getTypeId().getName().c_str();

    undistributeObject(obj);
  }
}

// create messages for any occassion
void
av::NetNode::makeCreateMessage(av::Msg& msg, const av::Link<av::Distributed>& obj)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_create_message: start: '"
                    << obj->getTypeId().getName().c_str()
                    << "' w/ id: "
                    << obj->netID();
#endif

  av_pushMsg(msg, obj->netID());     // push object id
  av_pushMsg(msg, obj->getTypeId()); // push object type id
  av_pushMsg(msg, sCreateMsg);       // push message token

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_create_message: done: '"
                    << obj->getTypeId().getName().c_str()
                    << "' w/ id: "
                    << obj->netID();
#endif
}

void
av::NetNode::makeUpdateMessage(av::Msg& msg, const av::Link<av::Distributed>& obj)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_update_message: start: '"
                    << obj->getTypeId().getName().c_str()
                    << "' w/ id: "
                    << obj->netID();
#endif

  av_pushMsg(msg, obj.getPtr());     // push only changes since last time
  av_pushMsg(msg, obj->netID());     // push object id
  av_pushMsg(msg, obj->getTypeId()); // push object type id
  av_pushMsg(msg, sUpdateMsg);       // push message token

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_update_message: done: '"
                    << obj->getTypeId().getName().c_str()
                    << "' w/ id: "
                    << obj->netID();
#endif
}

void
av::NetNode::makeDeleteMessage(av::Msg& msg, const av::Link<av::Distributed>& obj)
{
#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_delete_message: start: '"
                    << obj->getTypeId().getName().c_str()
                    << "' w/ id: "
                    << obj->netID();
#endif

  av_pushMsg(msg, obj->netID());
  av_pushMsg(msg, sDeleteMsg);

  {
    // remove the network info structure after message has been sent

    delete obj->getNetInfo();
    obj->setNetInfo(0);
  }

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::make_delete_message: done: '"
                    << obj->getTypeId().getName().c_str()
                    << "' (netID deleted)";
#endif
}

// take them from the net and do the right thing
void
av::NetNode::consumeMessage(av::Msg& msg)
{
  // pop the message token

  int msg_type;
  av_popMsg(msg, msg_type);

  // dispatch the message
  switch (msg_type) {
  case sCreateMsg: consumeCreateMessage(msg); break;
  case sUpdateMsg: consumeUpdateMessage(msg); break;
  case sDeleteMsg: consumeDeleteMessage(msg); break;
  case sPackedMsg: consumePackedMessage(msg); break;

  default:
    std::cout << "av::NetNode::consume_message: "
              << "got unknown msg type of '" << msg_type << "'" << std::endl;
    // assert(0);
    break;
  }
}

void
av::NetNode::consumeCreateMessage(av::Msg& msg)
{
  Type    obj_type;
  NetID obj_id;

  // check the type ...
  av_popMsg(msg, obj_type);

  if (!obj_type.isOfType(Distributed::getClassTypeId()))
  {
    std::cout << "av::NetNode::consume_create_message: bad ("
              << obj_type.getName().c_str() << ")"
              << std::endl;
  }

  AV_ASSERT(obj_type.isOfType(Distributed::getClassTypeId()));

  // ... and the id
  av_popMsg(msg, obj_id);
  AV_ASSERT(!obj_id.isWellKnown());

  Link<Distributed> test_obj;

  if (lookup(obj_id, test_obj))
  {
    return;
  }

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_create_message: creating a '"
                     << obj_type.getName().c_str() << "'";
#endif

  // create a new object of this type and register with the supplied id
  Link<Distributed> obj((Distributed*) obj_type.createInstance());

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_create_message: "
                    << "about to register object ";
#endif

  registerObj(obj, obj_id);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_create_message: '"
                    << obj_type.getName().c_str()
                    << "' w/ id: "
                    << obj_id;
#endif
}

void
av::NetNode::consumeUpdateMessage(av::Msg& msg)
{
  Type    obj_type;
  NetID obj_id;

  // check the type;
  av_popMsg(msg, obj_type);
  AV_ASSERT(obj_type.isOfType(Distributed::getClassTypeId()));

  // and the id
  av_popMsg(msg, obj_id);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_update_message: '"
                    << obj_type.getName().c_str()
                    << "' w/ id: "
                    << obj_id;
#endif

  Link<Distributed> obj;

  if (!lookup(obj_id, obj)) {
    obj = (Distributed*) obj_type.createInstance();

    std::cerr << "av::NetNode::consume_update_message: created dummy instance of type '"
              << obj_type.getName().c_str()
              << "' w/ id: "
              << obj_id
              << " for non-existent object"
              << std::endl;
  }

  if (obj->getTypeId() == obj_type) {
    // fill in the object
    av_popMsg(msg, obj.getPtr());
  }
}

void
av::NetNode::consumeDeleteMessage(av::Msg& msg)
{
  NetID obj_id;

  // check the id
  av_popMsg(msg, obj_id);

  // look it up
  Link<Distributed> obj;
  const bool            found = lookup(obj_id, obj);

  if (!found) {
    logger.warn() << "av::NetNode::consume_delete_message: "
                  << "object not found: "
                  << obj_id;
    return;
  }

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_delete_message: "
            << obj_id;
#endif

  unregisterObj(obj);
}

void
av::NetNode::consumePackedMessage(av::Msg& msg)
{
  int msg_count;
  av_popMsg(msg, msg_count);

#ifdef AVANGO_DEBUG
  logger.debug()<< "av::NetNode::consume_packed_message: " << msg_count;
#endif

  for (int i = 0; i < msg_count; ++i) {
    consumeMessage(msg);
  }
}

// ensemble command line option interface
/* static */ std::map<std::string, std::string,
                      std::less<std::string> > av::NetNode::sEnsOptions;
/* static */ char**                            av::NetNode::sEnsOptionsArgv = 0;

/* static */ void
av::NetNode::setEnsOption(const std::string& option, const std::string& value)
{
  sEnsOptions[option] = value;
}

/* static */ bool
av::NetNode::getEnsOption(const std::string& option, std::string& value)
{
  std::map<std::string, std::string,
    std::less<std::string> >::const_iterator i = sEnsOptions.find(option);

  if (i == sEnsOptions.end()) {
    return false;
  } else {
    value = (*i).second;
    return true;
  }
}

// for performance reasons, the value returned here is meant to be
// copied and then be forgotten.
/* static */ char**
av::NetNode::getEnsOptionsArgv()
{
  // delete the old argv if present
  if (sEnsOptionsArgv)
    ::delete [] sEnsOptionsArgv;

  // allocate space for a new one
  sEnsOptionsArgv = ::new char* [sEnsOptions.size() * 2 + 3];

  // assign references to the contents of the map
  std::map<std::string, std::string, std::less<std::string> >::const_iterator i;
  char** arg = sEnsOptionsArgv;

  // the program name goes first
  *arg++ = "avango";

  // print ensemble version/copyright stmt
  //*arg++ = "-v";

  for (i = sEnsOptions.begin(); i != sEnsOptions.end(); ++i) {
    *arg++ = const_cast<char*>((*i).first.c_str());
    if ((*i).second.size())
      *arg++ = const_cast<char*>((*i).second.c_str());
  }
  // mark the end
  *arg = 0;

  return sEnsOptionsArgv;
}
