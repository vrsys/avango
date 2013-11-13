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

#include <vector>

#include <boost/bind.hpp>

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  class MyConstObject : public av::BoolObject
  {
    AV_FC_DECLARE();

  public:

    av::MFBool BoolValues;
    av::SFBool BoolValue;

    MyConstObject()
    {
      AV_FC_ADD_FIELD(BoolValues, av::MFBool::ContainerType());
      AV_FC_ADD_FIELD(BoolValue, false);

    }

    void testConstness() const
    {
      BoolValues.getValue();
      BoolValue.getValue();
    }
  };

  AV_FC_DEFINE(MyConstObject);

  void MyConstObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::BoolObject::initClass();
      AV_FC_INIT(av::BoolObject, MyConstObject, true);
    }
  }

  TEST(fieldGetConst)
  {
    MyConstObject::initClass();
    av::Link<MyConstObject> obj(new MyConstObject);
    av::MFBool::ContainerType testArray(10, true);

    obj->BoolValues.setValue(testArray);

    CHECK_ARRAY_EQUAL(testArray, obj->BoolValues.getValue(), testArray.size());
  }

} // namespace
