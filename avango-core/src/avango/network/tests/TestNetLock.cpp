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

#include <avango/NetLock.h>
#include <avango/UnitTest++/UnitTest++.h>
#include <avango/Application.h>

namespace
{
  class NetLockFixture {
    public:
      NetLockFixture() { av::NetLock::initClass(); }
  };

  TEST_FIXTURE(NetLockFixture, InitializeNetLock)
  {
    CHECK(av::Type::getByName("av::NetLock") != av::Type::badType());
  }

  TEST_FIXTURE(NetLockFixture, CheckProtocol)
  {
    av::Link<av::NetLock> lock(new av::NetLock);

    // Lock is initially not granted, we are the first one
    lock->Request.setValue("Test1");
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(std::string("Test1"), lock->Granted.getValue());

    // Lock is granted to "Test1", cannot be granted to Test2
    lock->Request.setValue("Test2");
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(std::string("Test1"), lock->Granted.getValue());

    // Lock can only be released by "Test1"
    lock->Release.setValue("Test2");
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(std::string("Test1"), lock->Granted.getValue());

    lock->Release.setValue("Test1");
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(std::string(""), lock->Granted.getValue());

    // Now Lock is free and can be requested by "Test2"

    lock->Request.setValue("Test2");
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(std::string("Test2"), lock->Granted.getValue());
  }


}
