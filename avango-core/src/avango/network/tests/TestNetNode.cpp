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

#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <avango/Link.h>
#include <avango/NetLock.h>
#include <avango/NetNode.h>
#include <avango/ObjectValue.h>
#include <avango/UUID.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  bool value = 0;

  class MyBoolObject : public av::BoolObject
  {
    AV_FC_DECLARE();

  public:

    virtual void fieldHasChangedLocalSideEffect(const av::Field&);

  };

  AV_FC_DEFINE(MyBoolObject);

  void MyBoolObject::fieldHasChangedLocalSideEffect(const av::Field&)
  {
    value = Value.getValue();
  }

  void MyBoolObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::BoolObject::initClass();
      AV_FC_INIT(av::BoolObject, MyBoolObject, true);
    }
  }

  class MyNetNode : public av::NetNode
  {
  public:
    MyNetNode()
    {
      setEnsOption("-deering_port", "2346");
      setEnsOption("-multiread", "");
      //set_ens_option("-verbose", "");
      //set_ens_option("-socket_verb", "");
      //set_ens_option("-prbool_config", "");
      //set_ens_option("-prbool_defaults", "");
      //set_ens_option("-gc_verb", "");
      //set_ens_option("-trace", "avango");
      //set_ens_option("-traces", "avango");
    }
  };

  TEST(useNetNode)
  {
    MyBoolObject::initClass();
    std::string net_group_name = av::UUID();

    pid_t pid = ::fork();

    if (0 == pid) // child
    {
      MyNetNode net_node;
      net_node.join(net_group_name);

      av::Link<MyBoolObject> bool_obj(new MyBoolObject);
      bool_obj->Value.setValue(true);
      net_node.distributeObject(bool_obj.getPtr());

      while (true)
      {
        ::usleep(100000);
        net_node.handleNetworkReceives();
        net_node.handleNetworkSends();
      }

      ::_exit(0);
    }
    else // parent
    {
      MyNetNode net_node;
      net_node.join(net_group_name);

      for (int i=0; i<50; ++i)
      {
        ::usleep(100000);
        net_node.handleNetworkReceives();
        net_node.handleNetworkSends();
        if (value) break;
      }

      ::kill(pid, SIGTERM);
      ::waitpid(pid, 0, 0);

      CHECK(value);
    }
  }
} // namespace
