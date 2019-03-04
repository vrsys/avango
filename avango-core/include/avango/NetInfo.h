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

#if !defined(AVANGO_NETINFO_H)
#define AVANGO_NETINFO_H

#include <avango/NetID.h>

namespace av
{
class Distributed;
class NetNode;

// per node bookkeeping

class NetInfo
{
    friend class Distributed;

  public:
    NetInfo(const NetID&, const NetNode*);
    ~NetInfo();

    const NetID& getId() const;
    const NetNode* getNode() const;
    NetNode* getNode();

  private:
    NetID mId;
    NetNode* mNode;

    // declared but never defined
    NetInfo();
    NetInfo(const NetInfo&);
    const NetInfo& operator=(const NetInfo&);
};

} // namespace av

#endif // #if !defined(AVANGO_NETINFO_H)
