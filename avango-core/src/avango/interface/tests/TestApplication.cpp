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

#include <avango/Application.h>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
bool my_object_destroyed = true;

class MyObject : public av::IntObject
{
    AV_FC_DECLARE();

  public:
    MyObject();
    ~MyObject();

    virtual void evaluate();

  private:
    int mEvaluateCount;
};

AV_FC_DEFINE(MyObject);

MyObject::MyObject() : mEvaluateCount(10) { my_object_destroyed = false; }

MyObject::~MyObject() { my_object_destroyed = true; }

void MyObject::evaluate()
{
    Value.setValue(42);

    if(--mEvaluateCount <= 0)
    {
        av::ApplicationInstance::get().exit(false);
    }
}

void MyObject::initClass()
{
    if(!isTypeInitialized())
    {
        av::IntObject::initClass();
        AV_FC_INIT(av::IntObject, MyObject, true);
    }
}

TEST(ApplicationEvaluate)
{
    av::Link<MyObject> my_obj(new MyObject());
    my_obj->touch();
    av::ApplicationInstance::get().evaluate();
    CHECK_EQUAL(42, my_obj->Value.getValue());
    my_obj.clear();
    CHECK(my_object_destroyed);
}

TEST(ApplicationExit)
{
    av::Link<MyObject> my_obj(new MyObject());
    my_obj->touch();
    my_obj.clear();
    av::ApplicationInstance::get().exit(false);
    CHECK(my_object_destroyed);
}

TEST(ApplicationExitFromMainLoop)
{
    av::Link<MyObject> my_obj(AV_NEW(MyObject));
    my_obj->alwaysEvaluate(true);
    av::ApplicationInstance::get().start();
    my_obj.clear();
    CHECK(my_object_destroyed);
}
} // namespace

int main()
{
    av::getLogger("av::Application").addConsoleAppender();
    av::getLogger("av::Application").setLevel(av::logging::DEBUG);

    av::ApplicationInstance::get();
    MyObject::initClass();

    return UnitTest::RunAllTests();
}
