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

// include i/f header

// includes, system

#include <string>

// includes, project

#include <avango/ensemble/maestro/Maestro.h>

// variables, exported

// internal unnamed namespace

namespace
{
const char line[] = "---------------------------------------";

class group_test : public Maestro_GroupMember
{
  public:
    group_test(Maestro_GrpMemb_Options& ops) : Maestro_GroupMember(ops) { std::cout << "group_test::group_test(): constructor called." << std::endl; }

    virtual ~group_test() { std::cout << "group_test::group_test(): destructor called." << std::endl; }

  private:
    void grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData& viewData, Maestro_Message& /*OUT*/ /*viewMsg*/)
    {
        std::cout << "group_test::grpMemb_ViewMsg_Callback(): " << line << std::endl;
        std::cout << " version:     " << viewData.version << std::endl;
        std::cout << " groupname:   " << viewData.groupName << std::endl;
        std::cout << " endpID:      " << viewData.myEndpID << std::endl;
        std::cout << " members:     " << viewData.members << std::endl;
        std::cout << " new members: " << viewData.newMembers << std::endl;
        std::cout << " dep members: " << viewData.departedMembers << std::endl;
    }

    void grpMemb_MergeState_Callback(Maestro_MessageList& /*in*/ /*stateMsgs*/, Maestro_Message& /*out*/ /*mergedState*/)
    {
        std::cout << "group_test::grpMemb_MergeState_Callback(): " << line << std::endl;
    }

    void grpMemb_ViewStateInfo_Callback(
        Maestro_EndpID /*in*/& origin, Maestro_GrpMemb_ViewData /*in*/& viewData, Maestro_Message /*in*/& /*msg*/, Maestro_Message /*out*/& /*next_view_msg*/, int /*out*/& /*final*/)
    {
        std::cout << "group_test::grpMemb_ViewStateInfo_Callback(): " << line << std::endl;
        std::cout << " origin:      " << origin;
        std::cout << " version:     " << viewData.version << std::endl;
        std::cout << " groupname:   " << viewData.groupName << std::endl;
        std::cout << " endpID:      " << viewData.myEndpID << std::endl;
        std::cout << " members:     " << viewData.members << std::endl;
        std::cout << " new members: " << viewData.newMembers << std::endl;
        std::cout << " dep members: " << viewData.departedMembers << std::endl;
    }

    void grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData& /*viewData*/, Maestro_Message& /*msg*/) { std::cout << "group_test::grpMemb_AcceptedView_Callback(): " << line << std::endl; }

    // The group is blocked for a view change:

    void grpMemb_Block_Callback() { std::cout << "group_test::grpMemb_Block_Callback(): " << line << std::endl; }

    // The member has left the group:
    virtual void grpMemb_Exit_Callback() { std::cout << "group_test::grpMemb_Exit_Callback(): " << line << std::endl; }
};

bool linkTest()
{
    std::string s("Maestro_String initialized");
    Maestro_String ms(const_cast<char*>(s.c_str()));
    return !s.compare(ms.s);
}

bool groupTest()
{
    char* argv[] = {"groupTest", "-deering_port", "4000", 0};
    Maestro_GrpMemb_Options ops;

    ops.groupName = "group_test";
    // ops.viewMsgFlag = 1;
    ops.transports = "DEERING";
    ops.properties = "Gmp:Sync:Heal:Switch:Frag:Suspect:Flow:Total";
    ops.params = "suspect_max_idle=3:int;suspect_sweep=1.000:time";
    ops.argv = argv;

    group_test* member = new group_test(ops);
    member->join();
    std::cout << "groupTest(): member joined!" << std::endl;

    // Maestro_Semaphore sema;
    // sema.dec();

    return true;
}

} // namespace

// functions, exported

int main() { return !(linkTest() && groupTest()); }
