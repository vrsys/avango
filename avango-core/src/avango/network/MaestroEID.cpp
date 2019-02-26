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

#include <avango/MaestroEID.h>

#include <cstring>

av::MaestroEID::MaestroEID(Maestro_ErrorHandler* error_handler) : Maestro_EndpID(error_handler) {}

av::MaestroEID::MaestroEID(hot_endpt_t ep, Maestro_ErrorHandler* error_handler) : Maestro_EndpID(ep, error_handler) {}

av::MaestroEID::MaestroEID(const MaestroEID& eid) : Maestro_Base(), Maestro_EndpID(eid.endpID) {}

av::MaestroEID::MaestroEID(const Maestro_EndpID& eid) : Maestro_EndpID(eid.getHotEndpt()) {}

bool av::MaestroEID::operator<(const MaestroEID& eid) const { return (::strncmp(c_str(), eid.c_str(), HOT_ENDP_MAX_NAME_SIZE) < 0); }

bool av::MaestroEID::operator==(const MaestroEID& eid) const { return (::strncmp(c_str(), eid.c_str(), HOT_ENDP_MAX_NAME_SIZE) == 0); }

bool av::MaestroEID::operator!=(const MaestroEID& eid) const { return !MaestroEID::operator==(eid); }

const av::MaestroEID& av::MaestroEID::operator=(const MaestroEID& eid)
{
    Maestro_EndpID::operator=(const_cast<MaestroEID&>(eid));
    return *this;
}

const av::MaestroEID& av::MaestroEID::operator=(const Maestro_EndpID& eid)
{
    Maestro_EndpID::operator=(const_cast<Maestro_EndpID&>(eid));
    return *this;
}

const char* av::MaestroEID::c_str() const { return endpID.name; }

Maestro_Message& av::operator<<(Maestro_Message& msg, const MaestroEID& eid)
{
    const_cast<MaestroEID&>(eid).operator>>(msg);
    return msg;
}

Maestro_Message& av::operator>>(Maestro_Message& msg, MaestroEID& eid)
{
    eid.operator<<(msg);
    return msg;
}

std::ostream& av::operator<<(std::ostream& os, const MaestroEID& eid) { return os << eid.c_str(); }

size_t av::MaestroEID::Hasher::operator()(const MaestroEID& eid) const { return mHasher(eid.c_str()); }

av::MaestroEID_list::MaestroEID_list() {}

av::MaestroEID_list::~MaestroEID_list() {}

av::MaestroEID_list::MaestroEID_list(const MaestroEID_list& l) : MaestroEID_set(l) {}

av::MaestroEID_list::MaestroEID_list(const Maestro_EndpList& l)
{
    for(unsigned int i = 0; i < l.size(); i++)
        insert(l[i]);
}

const av::MaestroEID_list& av::MaestroEID_list::operator=(const Maestro_EndpList& l)
{
    clear();
    for(unsigned int i = 0; i < l.size(); i++)
        insert(l[i]);
    return *this;
}

Maestro_Message& av::operator<<(Maestro_Message& msg, const MaestroEID_list& l)
{
    MaestroEID_list::const_iterator i;

    for(i = l.begin(); i != l.end(); i++)
    {
        MaestroEID eid = (*i);
        msg << eid;
    }

    return msg << static_cast<int>(l.size());
}

Maestro_Message& av::operator>>(Maestro_Message& msg, MaestroEID_list& l)
{
    l.clear();

    int size;

    msg >> size;

    for(int i = 0; i < size; i++)
    {
        MaestroEID eid;
        msg >> eid;
        l.insert(eid);
    }

    return msg;
}

std::ostream& av::operator<<(std::ostream& os, const MaestroEID_list& l)
{
    MaestroEID_list::const_iterator i;

    for(i = l.begin(); i != l.end(); i++)
    {
        os << (*i);

        if(l.end() != i)
            os << " ";
    }

    return os;
}

av::MaestroEID_eidlist_map::MaestroEID_eidlist_map() {}

av::MaestroEID_eidlist_map::~MaestroEID_eidlist_map() {}

av::MaestroEID_eidlist_map::MaestroEID_eidlist_map(const MaestroEID_eidlist_map& m) : MaestroEID_eidlist_map_base(m) {}

Maestro_Message& av::operator<<(Maestro_Message& msg, const MaestroEID_eidlist_map& m)
{
    MaestroEID_eidlist_map::const_iterator i;

    for(i = m.begin(); i != m.end(); ++i)
    {
        msg << (*i).second;
        msg << (*i).first;
    }

    return msg << static_cast<int>(m.size());
}

Maestro_Message& av::operator>>(Maestro_Message& msg, MaestroEID_eidlist_map& m)
{
    m.clear();

    int size;

    msg >> size;

    for(int i = 0; i < size; i++)
    {
        MaestroEID eid;
        MaestroEID_list eid_list;

        msg >> eid;
        msg >> eid_list;

        m[eid] = eid_list;
    }

    return msg;
}

std::ostream& av::operator<<(std::ostream& os, const MaestroEID_eidlist_map& m)
{
    MaestroEID_eidlist_map::const_iterator i;

    for(i = m.begin(); i != m.end(); i++)
    {
        os << "  " << (*i).first << " : " << (*i).second << std::endl;
    }

    return os;
}
