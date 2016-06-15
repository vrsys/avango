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

#include <functional>

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  av::Logger& logger(av::getLogger("TestFieldValueCallbacks"));

  class MyObject : public av::BoolObject
  {
    AV_FC_DECLARE();

  public:

    av::MFBool Values;

    MyObject() :
      mValue(false)
    {
      AV_FC_ADD_ADAPTOR_FIELD(Value,
                              std::bind(&MyObject::getValueCallback, this, std::placeholders::_1),
                              std::bind(&MyObject::setValueCallback, this, std::placeholders::_1));
      AV_FC_ADD_ADAPTOR_FIELD(Values,
                              std::bind(&MyObject::getValuesCallback, this, std::placeholders::_1),
                              std::bind(&MyObject::setValuesCallback, this, std::placeholders::_1));
    }

    bool mValue;
    std::vector<bool> mValues;

  private:

    void getValueCallback(av::SFBool::GetValueEvent event)
    {
      *(event.getValuePtr()) = mValue;
    }

    void setValueCallback(av::SFBool::SetValueEvent event)
    {
      mValue = event.getValue();
    }

    void getValuesCallback(av::MFBool::GetValueEvent event)
    {
      *(event.getValuePtr()) = mValues;
    }

    void setValuesCallback(av::MFBool::SetValueEvent event)
    {
      mValues = event.getValue();
    }

  };

  AV_FC_DEFINE(MyObject);

  void MyObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::BoolObject::initClass();
      AV_FC_INIT(av::BoolObject, MyObject, true);
    }
  }

  TEST(fieldValueValue)
  {
    MyObject::initClass();
    av::Link<MyObject> obj(new MyObject);

    CHECK_EQUAL(false, obj->mValue);
    CHECK_EQUAL(false, obj->Value.getValue());

    obj->Value.setValue(true);
    CHECK_EQUAL(true, obj->mValue);
    CHECK_EQUAL(true, obj->Value.getValue());

    obj->mValue = false;
    CHECK_EQUAL(false, obj->mValue);
    CHECK_EQUAL(false, obj->Value.getValue());

    CHECK(obj->mValues.empty());
    CHECK_EQUAL(0, obj->Values.getSize());
    obj->mValues.push_back(true);
    CHECK_EQUAL(1, obj->Values.getSize());
    obj->Values.add1Value(true);
    CHECK_EQUAL(2, obj->Values.getSize());
    CHECK_EQUAL(2u, obj->mValues.size());
    obj->Values.clear();
    CHECK(obj->Values.isEmpty());
    CHECK(obj->mValues.empty());
  }

} // namespace

int main()
{
  logger.addConsoleAppender();
  av::ApplicationInstance::get();
  return UnitTest::RunAllTests();
}
