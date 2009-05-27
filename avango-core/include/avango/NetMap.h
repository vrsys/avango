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

#if !defined(AVANGO_NETMAP_H)
#define AVANGO_NETMAP_H

#include <iosfwd>
#include <vector>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

// #include <avango/MaestroEID.h>
#include <avango/Distributed.h>

namespace av
{
  class NetID;

  // a catalog of all objects playing in netland, exept well-kowns

  class NetMap {

  public:

    NetMap();
    virtual ~NetMap();

    void addSlot(const std::string& eid);
    void removeSlot(const std::string& eid);
    bool slotExists(const std::string& eid) const;

    void registerObj(const Link<Distributed>& obj);
    void unregisterObj(const Link<Distributed>& obj);

    bool lookup(const NetID& id, Link<Distributed>& result) const;
    bool lookup(const std::string& eid, std::vector<Link<Distributed> >& result) const;
    bool lookup(std::vector<Link<Distributed> >& result) const;

    bool lookupExcept(const std::string& eid, std::vector<Link<Distributed> >& result) const;

    void removeAllSlots();
    void removeAllSlotsExcept(const std::string& eid);

    void emptySlot(const std::string& eid);
    void emptyAllSlots();
    void emptyAllSlotsExcept(const std::string& eid);

    void dump(std::ostream& os) const;

  private:

    typedef std::tr1::unordered_map<int, Link<Distributed> > IntDstMap;
    typedef std::tr1::unordered_map<std::string, IntDstMap> EIDMapMap;

    EIDMapMap mEIDMap;

  };

} // namespace av

#endif // #if !defined(AVANGO_NETMAP_H)
