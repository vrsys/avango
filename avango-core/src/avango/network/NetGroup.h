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

#if !defined(AVANGO_NETGROUP_H)
#define AVANGO_NETGROUP_H

#include <unistd.h>

// #include <avango/NetMsg.h>
#include <avango/NetID.h>

#include "MaestroMerge.h"
#include "UpcallSerializer.h"

class Maestro_ClSv_Options;

namespace av
{
  //
  // NetGroup -- interface to ensemble, contain all Maestro include files,
  //                 bridge the process barrier for the callbacks.
  //

  // This class implements the ensemble group member

  class NetGroup : public MaestroMerge {
  public:

    // create a new group member and join the named group
    NetGroup(NetNode* netNode, Maestro_ClSv_Options &options);

    // destroy this group member and leave the group
    ~NetGroup();

    UpcallSerializer& upcallSerializer();

    // Sending messages.
    virtual void cast(Msg &msg);

    virtual void send(const std::string &destEID, Msg &msg);

  private:

    // the NetNode which created us
    NetNode* mNetNode;

    UpcallSerializer mUpcallSerializer;

    // State transfer callbacks
    virtual void Merge_GetState_Callback(Maestro_EndpID& fragment, Maestro_Message& state_message);
    virtual void Merge_SetState_Callback(Maestro_EndpID& fragment, Maestro_Message& state_message);
    virtual void Merge_RemoveState_Callback(Maestro_EndpID& fragment);

    // Message callbacks:
    virtual void Merge_ReceiveCast_Callback(Maestro_EndpID &origin, Maestro_Message &msg);
    virtual void Merge_ReceiveSend_Callback(Maestro_EndpID &origin, Maestro_Message &msg);

    // view change callbacks
    virtual void Merge_AcceptedView_Callback(MaestroMergeViewData&, Maestro_Message&);
    virtual void Merge_Block_Callback();
    virtual void Merge_UnBlock_Callback();

    // The member has left the group:
    virtual void Merge_Join_Callback(Maestro_EndpID& myeid);

    // The member has left the group:
    virtual void Merge_Exit_Callback();

  private:

    // declared but never defined
    //NetGroup();
    //NetGroup(const NetGroup&);
    //const NetGroup& operator= (const NetGroup&);

  };

} // namespace av

#endif // #if !defined(AVANGO_NETGROUP_H)

