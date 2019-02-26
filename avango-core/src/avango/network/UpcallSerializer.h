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

#if !defined(AVANGO_UPCALLSERIALIZER_H)
#define AVANGO_UPCALLSERIALIZER_H

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>
#include <vector>

#include <avango/Msg.h>
#include <avango/Semaphore.h>

// #include "MaestroMerge.h"

namespace av
{
class NetID;
class NetNode;
class UpcallSerializer;

class Upcall
{
  public:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer) = 0;
    // called from app process
    virtual void handle(NetNode* net_node, UpcallSerializer& serializer) = 0;

    virtual ~Upcall();

  protected:
    Upcall();
};

class JoinUpcall : public Upcall
{
  public:
    JoinUpcall(const std::string& eid) : mEID(eid) {}
    ~JoinUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* net_node, UpcallSerializer& serializer);

  private:
    std::string mEID;
};

class GetStateUpcall : public Upcall
{
  public:
    GetStateUpcall(const std::string& fragment) : mFragment(fragment) {}
    ~GetStateUpcall() {}

    Msg stateMsg() { return mStateMsg; }

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* net_node, UpcallSerializer& serializer);

  private:
    std::string mFragment;
    Msg mStateMsg;
};

class SetStateUpcall : public Upcall
{
  public:
    SetStateUpcall(const std::string& fragment, Msg& stateMsg) : mFragment(fragment), mStateMsg(stateMsg) {}
    ~SetStateUpcall() {}

    const Msg& stateMsg() { return mStateMsg; }

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);

  private:
    std::string mFragment;
    Msg mStateMsg;
};

class RemoveStateUpcall : public Upcall
{
  public:
    RemoveStateUpcall(const std::string& fragment) : mFragment(fragment) {}
    ~RemoveStateUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);

  private:
    std::string mFragment;
};

class MessageUpcall : public Upcall
{
  public:
    MessageUpcall(Msg& msg) : mMsg(msg) {}
    ~MessageUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);

  private:
    Msg mMsg;
};

class AcceptedViewUpcall : public Upcall
{
  public:
    AcceptedViewUpcall(const std::vector<std::string>& members, const std::vector<std::string>& newMembers, const std::vector<std::string>& departedMembers, Msg& msg)
        : mMembers(members), mNewMembers(newMembers), mDepartedMembers(departedMembers), mMsg(msg)
    {
    }
    ~AcceptedViewUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);

  private:
    std::vector<std::string> mMembers;
    std::vector<std::string> mNewMembers;
    std::vector<std::string> mDepartedMembers;
    Msg mMsg;
};

class BlockUpcall : public Upcall
{
  public:
    BlockUpcall() {}
    ~BlockUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);
};

class UnblockUpcall : public Upcall
{
  public:
    UnblockUpcall() {}
    ~UnblockUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);
};

class ExitUpcall : public Upcall
{
  public:
    ExitUpcall() {}
    ~ExitUpcall() {}

  protected:
    // called from net process
    virtual void upcallQueued(UpcallSerializer& serializer);
    // called from app process
    virtual void handle(NetNode* netNode, UpcallSerializer& serializer);
};

class UpcallSerializer
{
  public:
    UpcallSerializer();
    ~UpcallSerializer();

    // called from net process
    void makeUpcall(boost::shared_ptr<Upcall> upcall);
    // called from app process
    bool handleNextUpcall(NetNode* netNode);

    void netWait();
    void signalWait();

  protected:
    // called from app process
    void handleUpcall(boost::shared_ptr<Upcall> upcall, NetNode* netNode);

  private:
    boost::mutex mUpcallQueueLock;
    std::deque<boost::shared_ptr<Upcall>> mUpcallQueue;
    Semaphore mSyncSema;
};

} // namespace av

#endif // #if !defined(AVANGO_UPCALLSERIALIZER_H)
