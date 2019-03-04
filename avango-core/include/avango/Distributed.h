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

#if !defined(AVANGO_DISTRIBUTED_H)
#define AVANGO_DISTRIBUTED_H

/**
 * \file
 * \ingroup av
 */

#include <unordered_set>

#include <avango/Base.h>
#include <avango/Config.h>
#include <avango/Link.h>

namespace av
{
#if defined(AVANGO_DISTRIBUTION_SUPPORT)

class NetID;
class NetInfo;
class Msg;
class NetNode;

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)

/**
 * The Distributed class is the base class for all classes that can be distributed.
 * It provides a simple public interface to some of the internals of the distribution.
 * Although, normally the application programmer should not need to be concerned with such
 * details.
 *
 * \ingroup av
 *
 */
class AV_DLL Distributed : public Base
{
    AV_BASE_DECLARE_ABSTRACT();

  public:
    /**
     * Constructor
     */
    Distributed();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Distributed();

    /**
     * Returns \c true if this object has been created by the calling process.
     * If this object is a replicated copy created by some other process, \c false
     * is returned.
     */
    bool isOwned() const;

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

  public:
    /**
     * Returns \c true if this node is distributed.
     */
    bool isDistributed() const;

    /**
     * Returns the distribution group wide unique identifier for this object.
     * If the this object is not distributed NetID::NullID is returned.
     */
    const NetID& netID() const;

    /**
     * Returns the network wide unique identifier for the process which created this object.
     */
    const std::string& netCreator() const;

    /**
     * Returns the NetNode of this instance.
     */
    const NetNode* netNode() const;

    /**
     * Returns the NetNode of this instance.
     */
    NetNode* netNode();

    /**
     * Write to a net msg
     */
    virtual void push(Msg& netMsg) = 0;

    /**
     * Read from a net msg
     */
    virtual void pop(Msg& netMsg) = 0;

  protected:
    friend class NetNode;
    void setNetInfo(NetInfo* netInfo);
    NetInfo* getNetInfo();

    // notify the network of a local change
    void notifyLocalChange();

    // client code can do something here
    virtual void becomingDistributed();
    virtual void becomingUndistributed();

  private:
    NetInfo* mNetInfo;

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)
};

using DistributedSet = std::unordered_set<Link<Distributed>, AnyLink::Hasher, std::equal_to<AnyLink>>;
} // namespace av

#endif // #if !defined(AVANGO_DISTRIBUTED_H)
