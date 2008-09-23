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

#include <boost/bind.hpp>

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/Object.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  av::Logger& logger(av::getLogger("TestFieldCallbacks"));

  int callback_counter(0);

  class MyObject : public av::Object
  {
    AV_FC_DECLARE();

  public:

    MyObject();

  private:

    void fieldEventCallback(av::Field::Event event);
  };

  AV_FC_DEFINE(MyObject);

  MyObject::MyObject()
  {
    Name.addChangedCallback(boost::bind(&MyObject::fieldEventCallback, this, _1));
    Name.addConnectedCallback(boost::bind(&MyObject::fieldEventCallback, this, _1));
    Name.addDisconnectedCallback(boost::bind(&MyObject::fieldEventCallback, this, _1));
  }

  void MyObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();
      AV_FC_INIT(av::Object, MyObject, true);
    }
  }

  void MyObject::fieldEventCallback(av::Field::Event)
  {
    logger.info() << "MyObject::fieldEventCallback";
    ++callback_counter;
  }

  TEST(fieldCallbacks)
  {
    MyObject::initClass();
    av::BoolObject::initClass();

    av::Link<MyObject> obj(new MyObject);
    av::Link<av::BoolObject> bool_obj(new av::BoolObject);

    callback_counter = 0;
    obj->Name.touch();
    CHECK_EQUAL(1, callback_counter);

    callback_counter = 0;
    obj->Name.connectFrom(&bool_obj->Name);
    CHECK_EQUAL(2, callback_counter);

    callback_counter = 0;
    obj->Name.disconnectFrom(&bool_obj->Name);
    CHECK_EQUAL(2, callback_counter);
  }

} // namespace

int main()
{
  logger.addConsoleAppender();
  av::ApplicationInstance::get();
  return UnitTest::RunAllTests();
}

