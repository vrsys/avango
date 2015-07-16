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

#if !defined(AVANGO_NETNODE_H)
#define AVANGO_NETNODE_H

#include <boost/thread/mutex.hpp>
#include <deque>
#include <map>
#include <string>
#include <vector>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

//#include <avango/ensemble/maestro/Maestro.h>
#include <avango/Distributed.h>
#include <avango/FieldContainer.h>
#include <avango/Link.h>
#include <avango/NetID.h>
#include <avango/NetMap.h>
//#include <avango/NetMsg.h>
#include <avango/Msg.h>

namespace av
{
  class NetGroup;

  using NetIDDstMap = std::tr1::unordered_map<NetID, Link<Distributed>, NetID::Hasher>;
  using MsgDeq = std::deque<Msg>;
  using MaestroStateUpdateList = std::vector<std::pair<std::string, Msg> >;

  /**
   * This class is the link between avango and the group communication toolkit. It represents the avango process in the named group
   * this NetNode joins.
   */
  class NetNode
  {

  public:

    /**
     * Create a new group member and join the group
     * @param groupName The group to join
     */
    void join(const std::string& groupName);

    /**
     * Leave the currently joined group
     */
    void leave();

    /**
     * Wether we are connected to some group or not
     * @return true if connected, false otherwise
     */
    bool onAir() const;

    /**
     * Look for a distributed node which matches the specified id
     * @param id Id of the wanted node
     * @param result The found node
     * @return true if node was found, false otherwise
     */
    bool lookup(const NetID& id, Link<Distributed>& result);

    /**
     * Make an local object distributed
     * @param obj The object that should be shared
     */
    void distributeObject(Link<Distributed> obj);
    /**
     * Make a distributed object local
     * @param obj The distributed object
     */
    void undistributeObject(Link<Distributed> obj);

    /**
     * Specialized version of distributeObject, used for Python binding
     * @param obj
     */
    void distributeFieldContainer(Link<FieldContainer> obj);
    /**
     * Specialized version of undistributeObject, used for Python binding
     * @param obj
     */
    void undistributeFieldContainer(Link<FieldContainer> obj);

    /**
     * Tell the group that this object has changed. Schedules object updates for group distribution
     * @param obj The object that has changed
     */
    void updateDistributedObject(const Link<Distributed>& obj);

    /**
     * Handle network callbacks. All callbacks which have occured until now
     *  are handled in the correct order.
     */
    void handleNetworkSends();
    void handleNetworkReceives();

    /**
     * This function is called from the network side when this process has successfully joined the group
     * @param myeid The EndpointID of this process
     */
    void joined(const std::string& myeid);

    /**
     * This function is called from the network side when the group is blocked for a view change
     * (processes enter or leave the group)
     */
    void block();
    /**
     * This function is called from the network side when the group is unblocked for a view change
     * (processes enter or leave the group)
     */
    void unblock();

    void queueStateFragment(const std::string& fragment, Msg& stateMsg);
    void queueRemoveFragment(const std::string& fragment);
    void setStateFragment(const std::string& fragment, Msg& stateMsg);
    void getStateFragment(const std::string& fragment, Msg&stateMsg);
    void removeStateFragment(const std::string& fragment);
    void receiveMessage(const std::string& origin, Msg& msg);
    void exitCompleted();
    void acceptNewView(const std::vector<std::string>& members,
                       const std::vector<std::string>& newMembers,
                       const std::vector<std::string>& departedMembers,
                       Msg& msg);

    // return the enpoint id of the group we joined
    const std::string& netEID() const ;

    void consumeMessage(Msg& msg);

  protected:

    // hide ctor and dtor
    NetNode();
    virtual ~NetNode();

    // registration of well known nodes. only when registered, well-known nodes can
    // receive and send update messages. because they are well known, creation and
    // destruction is not announced via the net. every participant is supposed to
    // to create his own versions of these nodes.
    void registerWellKnown(const Link<Distributed>& obj, const NetID& id);
    void unregisterWellKnown(const Link<Distributed>& obj);

    // registration of distributables. an object can only be distributed in one
    // group. all other group members will be able to see and modify a registered
    // object. an unregistered object is only visible to the creator.
    void registerObj(const Link<Distributed>& obj);
    void registerObj(const Link<Distributed>& obj, const NetID& id);
    void unregisterObj(const Link<Distributed>& obj);

    // announce locally made changes to the group
    void notifyGroup();

    // create messages for every occassion
    void makeCreateMessage(Msg& msg, const Link<Distributed>& obj);
    void makeUpdateMessage(Msg& msg, const Link<Distributed>& obj);
    void makeDeleteMessage(Msg& msg, const Link<Distributed>& obj);

    // take them from the net and do the right thing
    void consumeReceivedMessages();
    void consumeCreateMessage(Msg& msg);
    void consumeUpdateMessage(Msg& msg);
    void consumeDeleteMessage(Msg& msg);
    void consumePackedMessage(Msg& msg);

    // the state transfer is special
    void makeStateTransferMessage(Msg& msg);
    void consumeStateTransferMessage(Msg& msg);

    // tell our clients we joined
    virtual void _join(const std::string& fragment) {}

    // overloaded from NetNode to react to view changes
    virtual void _acceptNewView(const std::vector<std::string>& members,
                                const std::vector<std::string>& newMembers,
                                const std::vector<std::string>& departedMembers) {}

    // overloaded to participate in state transfers
    virtual void _getStateFragment(const std::string& , Msg& ) {}
    virtual void _setStateFragment(const std::string& , Msg& ) {}
    virtual void _removeStateFragment(const std::string& ) {}

  private:

    // don't allow copy ctor and assignment otor
    NetNode(const NetNode&);
    NetNode& operator=(const NetNode&);

    // message type tokens
    static const unsigned int sCreateMsg;
    static const unsigned int sUpdateMsg;
    static const unsigned int sDeleteMsg;
    static const unsigned int sPackedMsg;
    static const unsigned int sStateTransferMsg;

    // generate a new id, which is unique with respect to this group
    // it consists of the eid of the creator and a number
    std::string mEID;
    int mIdCounter;
    NetID generateUniqueId();

    // the actual group member, derived from some maestro class
    NetGroup* mMember;
    int       mNumMembers;

    // all well-known objects in a map for fast lookup by id
    NetIDDstMap mWellKnownObjects;

    // all other distributed objects
    NetMap mObjectMap;

    // send over the changes
    void notifyCreations();
    void notifyUpdates();
    void notifyDeletes();

    // messages are pending to be send for these objects
    DistributedSet mPendingCreations;
    DistributedSet mPendingUpdates;
    DistributedSet mPendingDeletes;

    // messages we receive while not being a server are stacked up here for later processing
    MsgDeq    mReceivedMessages;
    boost::mutex mMessageMutex;
    bool         mBlocked;

    using StateUpdateList = std::vector<std::pair<std::string, Msg> >;
    using StateRemoveList = std::vector<std::string>;

    StateUpdateList mStateUpdates;
    StateRemoveList mStateRemoves;
    void applyStateFragments();

    // ensemble command line option interface

  protected:

    static std::map<std::string, std::string, std::less<std::string> > sEnsOptions;
    static char**                                                      sEnsOptionsArgv;
    static char** getEnsOptionsArgv();

  public:

    static void setEnsOption(const std::string& option, const std::string& value);
    static bool getEnsOption(const std::string& option, std::string& value);

  };

} // namespace av

#endif // #if !defined(AVANGO_NETNODE_H)
