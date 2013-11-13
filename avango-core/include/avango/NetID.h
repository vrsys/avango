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

#if !defined(AVANGO_NETID_H)
#define AVANGO_NETID_H

#include <string>
#include <iosfwd>

#ifdef __GNUC__ // GNU C++ stores TR1 headers differently
#include <tr1/unordered_set>
#else
#include <unordered_map>
#endif


namespace av
{
  /**
   * Unique ID for every node in a net group.
   * It consists of the endpoint id and a number.
   */
  class NetID
  {

  public:

    class Hasher
    {
    public:

      // this is a very poor hash function. improve it!
      size_t operator()(const NetID& id) const
      {
        return std::tr1::hash<std::string>()(id.getEID()) + id.getNum();
      }
    };

    NetID();
    NetID(const NetID& ni);
    NetID(const std::string& eid, int number);
    ~NetID();

    NetID& operator=(const NetID&);
    bool operator==(const NetID& ni) const;

    void set(const std::string& eid, int number);

    int getNum() const;
    const std::string& getEID() const;
    bool isWellKnown() const;

    bool isNull() const;
    void setNull();
    static NetID nullID();

    friend std::ostream& operator<< (std::ostream& os, const NetID&);
    static const int sNetGroupRootNode;
    static const int sNetGroupContainerHolder;

  private:
    static const int sWellKnownBase;

    std::string mCreator;
    int         mNumber;
  };

  std::ostream& operator<< (std::ostream& os, const NetID&);

  typedef std::tr1::unordered_set<NetID, NetID::Hasher, std::equal_to<NetID> > NetID_set;

} // namespace av

#endif // #if !defined(AVANGO_NETID_H)
