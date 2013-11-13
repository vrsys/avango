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

#if !defined(AVANGO_MAESTROEID_H)
#define AVANGO_MAESTROEID_H

#include <iosfwd>
#include <set>
#include <string>
#include <vector>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/functional>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#else
#include <functional>
#include <unordered_map>
#include <unordered_set>
#endif

#include <avango/ensemble/maestro/Maestro_Types.h>

namespace av
{

  /**
   * We need to add a few things to Maestro_EndpID for the STL.
   */
  class MaestroEID : public Maestro_EndpID
  {

  public:

    // for the STL hash containers
    class Hasher
    {
    public:
      size_t operator() (const MaestroEID& eid) const;
    private:
      std::tr1::hash<std::string> mHasher;
    };

    MaestroEID(Maestro_ErrorHandler* error_handler = 0);
    MaestroEID(hot_endpt_t ep, Maestro_ErrorHandler* error_handler = 0);
    MaestroEID(const MaestroEID& eid);
    MaestroEID(const Maestro_EndpID& eid);

    // support sorted STL container
    bool operator<(const MaestroEID &eid) const;
    bool operator==(const MaestroEID &eid) const;
    bool operator!=(const MaestroEID &eid) const;

    // assignment
    const MaestroEID& operator=(const MaestroEID& eid);
    const MaestroEID& operator=(const Maestro_EndpID& eid);

    // get the C string
    const char* c_str() const;

    friend Maestro_Message& operator<<(Maestro_Message &msg, const MaestroEID& eid);
    friend Maestro_Message& operator>>(Maestro_Message &msg, MaestroEID& eid);

  };

  std::ostream& operator<<(std::ostream& os, const MaestroEID& eid);

  // typedef for common containers
  typedef std::tr1::unordered_map<MaestroEID, int, MaestroEID::Hasher> MaestroEID_int_map;
  typedef std::tr1::unordered_set<MaestroEID, MaestroEID::Hasher>      MaestroEID_hset;
  typedef std::set<MaestroEID>                                         MaestroEID_set;
  typedef std::vector<MaestroEID>                                      MaestroEID_vec;

  class MaestroEID_list : public MaestroEID_set
  {

  public:

    MaestroEID_list();
    MaestroEID_list(const MaestroEID_list& l);
    MaestroEID_list(const Maestro_EndpList& l);
    virtual ~MaestroEID_list();

    const MaestroEID_list& operator=(const Maestro_EndpList& l);

  };

  Maestro_Message& operator<< (Maestro_Message &msg, const MaestroEID_list& l);
  Maestro_Message& operator>> (Maestro_Message &msg, MaestroEID_list& l);
  std::ostream& operator<<(std::ostream& os, const MaestroEID_list& l);

  typedef std::tr1::unordered_map<MaestroEID, MaestroEID_list, MaestroEID::Hasher>
  MaestroEID_eidlist_map_base;

  // a MaestroEID to MaestroEID_list map
  class MaestroEID_eidlist_map : public MaestroEID_eidlist_map_base
  {

  public:

    MaestroEID_eidlist_map();
    MaestroEID_eidlist_map(const MaestroEID_eidlist_map& m);
    virtual ~MaestroEID_eidlist_map();

  };

  Maestro_Message& operator<< (Maestro_Message &msg, const MaestroEID_eidlist_map& m);
  Maestro_Message& operator>> (Maestro_Message &msg, MaestroEID_eidlist_map& m);
  std::ostream& operator<<(std::ostream& os, const MaestroEID_eidlist_map& m);

} // namespace av

#endif // #if !defined(AVANGO_MAESTROEID_H)
