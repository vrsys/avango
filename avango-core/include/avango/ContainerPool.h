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

#if !defined(AVANGO_CONTAINERPOOL_H)
#define AVANGO_CONTAINERPOOL_H

/**
 * \file
 * \ingroup av
 */

#include <list>
#include <map>
#include <map>
#include <string>

#include <avango/FieldContainer.h>

namespace av
{

  /**
   * All field containers are registered here.
   *
   * \ingroup av
   */
  class AV_DLL ContainerPool
  {

  public:

    typedef FieldContainer::IDType FieldContainerID;

    typedef std::map<FieldContainerID, FieldContainer*> InstancePoolType;
    typedef void (*NodeCreationCallback)(FieldContainer*, void*);
    typedef void (*NodeDeletionCallback)(FieldContainer*, void*);
    typedef void (*FieldConnectCallback)(Field*, void*);
    typedef void (*FieldDisconnectCallback)(Field*, void*);
    typedef void (*FieldHasChangedCallback)(Field*, void*);

    struct FieldHasChangedReg
    {
      FieldHasChangedReg(FieldHasChangedCallback c, Field* f, void* ud);
      FieldHasChangedCallback mCb;
      Field* mField;
      void* mUserData;
    };

    typedef std::list<std::pair<NodeCreationCallback, void*> >    NodeCreationCallbacksType;
    typedef std::list<std::pair<NodeDeletionCallback, void*> >    NodeDeletionCallbacksType;
    typedef std::list<std::pair<FieldConnectCallback, void*> >    FieldConnectCallbacksType;
    typedef std::list<std::pair<FieldDisconnectCallback, void*> > FieldDisconnectCallbacksType;
    typedef std::list<FieldHasChangedReg>                         FieldHasChangedCallbacksType;

    static const InstancePoolType& getContainerPool () {return sContainerPool;};
    static FieldContainerID registerInstance(FieldContainer*);
    static void unregisterInstance(FieldContainer*);

    static void              setNameForInstance(FieldContainer* fc, const std::string& name);
    static void              setNameForInstance(FieldContainer* fc, const char*        name);
    static void              removeNameForInstance(FieldContainer* fc);
    static FieldContainer*   getInstanceByName(const std::string& name);
    static std::string       getNameByInstance(FieldContainer* fc);
    static FieldContainer*   getContainerById(FieldContainerID id);

    static void              notifyConnect(Field* field);
    static void              notifyDisconnect(Field* field);

    static void              notifyFieldHasChanged( Field* field);
    static void              notifyCreation(FieldContainer* fc);
    static void              notifyDeletion(FieldContainer* fc);

    class NodeCreationCallbackID
    {
      friend class ContainerPool;

    public:
      NodeCreationCallbackID();

    private:
      NodeCreationCallbackID(NodeCreationCallbacksType::iterator);
      NodeCreationCallbacksType::iterator mCallbackIter;
    };

    static NodeCreationCallbackID registerNodeCreationCallback(NodeCreationCallback, void*);
    static void unregisterNodeCreationCallback(NodeCreationCallbackID);

    class NodeDeletionCallbackID
    {
      friend class ContainerPool;

    public:
      NodeDeletionCallbackID();

    private:
      NodeDeletionCallbackID(NodeDeletionCallbacksType::iterator);
      NodeDeletionCallbacksType::iterator mCallbackIter;
    };

    static NodeDeletionCallbackID registerNodeDeletionCallback(NodeDeletionCallback, void*);
    static void unregisterNodeDeletionCallback(NodeDeletionCallbackID);

    class FieldConnectCallbackID
    {
      friend class ContainerPool;

    public:
      FieldConnectCallbackID();

    private:
      FieldConnectCallbackID(FieldConnectCallbacksType::iterator);
      FieldConnectCallbacksType::iterator mCallbackIter;
    };

    static FieldConnectCallbackID registerFieldConnectCallback(FieldConnectCallback, void*);
    static void unregisterFieldConnectCallback(FieldConnectCallbackID);

    class FieldDisconnectCallbackID
    {
      friend class ContainerPool;

    public:
      FieldDisconnectCallbackID();

    private:
      FieldDisconnectCallbackID(FieldDisconnectCallbacksType::iterator);
      FieldDisconnectCallbacksType::iterator mCallbackIter;
    };

    static FieldDisconnectCallbackID registerFieldDisconnectCallback(
      FieldDisconnectCallback, void*);
    static void unregisterFieldDisconnectCallback(FieldDisconnectCallbackID);

    class FieldHasChangedCallbackID
    {
      friend class ContainerPool;

    public:
      FieldHasChangedCallbackID();

    private:
      FieldHasChangedCallbackID(FieldHasChangedCallbacksType::iterator);
      FieldHasChangedCallbacksType::iterator mCallbackIter;
    };

    static FieldHasChangedCallbackID registerFieldHasChangedCallback(
      FieldHasChangedCallback, Field*, void*);
    static void unregisterFieldHasChangedCallback(FieldHasChangedCallbackID);

  private:
    static InstancePoolType           sContainerPool;
    static NodeCreationCallbacksType  sCreationCallbacks;
    static NodeDeletionCallbacksType  sDeletionCallbacks;
    static FieldConnectCallbacksType  sConnectCallbacks;
    static FieldDisconnectCallbacksType sDisconnectCallbacks;
    static FieldHasChangedCallbacksType sFieldHasChangedCallbacks;

    typedef std::map<std::string, FieldContainer*> NameToContainerMap;
    typedef std::map<FieldContainer*, std::string> ContainerToNameMap;

    static NameToContainerMap sNameContainerMap;
    static ContainerToNameMap sContainerNameMap;

    // declared but never defined
    ContainerPool();
    ContainerPool(const ContainerPool&);
    ~ContainerPool();
    const ContainerPool& operator=(const ContainerPool&);

    // Holds the last assigned id value for a FieldContainer entering the pool.
    static FieldContainerID sLastId;

  };

} // namespace av

#endif
