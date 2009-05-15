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
#include <avango/Logger.h>
#include <avango/ObjectValue.h>

#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  av::Logger& logger(av::getLogger("TestFieldReferencing"));

  class MyObject : public av::Object
  {
    AV_FC_DECLARE();

  public:

    MyObject(bool* deleted = 0);
    ~MyObject();

  private:

    bool *mDeleted;

  };

  AV_FC_DEFINE(MyObject);

  MyObject::MyObject(bool* deleted) :
    Object(),
    mDeleted(deleted)
  {
    if (mDeleted) *mDeleted = false;
  }

  MyObject::~MyObject()
  {
    if (mDeleted) *mDeleted = true;
  }

  void MyObject::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();
      AV_FC_INIT(av::Object, MyObject, true);
    }
  }

  TEST(fieldReferencing)
  {
    MyObject::initClass();

    bool src_deleted = false;
    MyObject *src(new MyObject(&src_deleted));
    src->reference();

    bool dst_deleted = false;
    MyObject *dst(new MyObject(&dst_deleted));
    dst->reference();

    AVANGO_LOG(logger, av::logging::INFO, "before connect");
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("src: %1%") % src->referenceCount()));
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("dst: %1%") % dst->referenceCount()));

    dst->Name.connectFrom(&src->Name);
    AVANGO_LOG(logger, av::logging::INFO, "after connect");
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("src: %1%") % src->referenceCount()));
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("dst: %1%") % dst->referenceCount()));

    // evaluate once because containers have been touched
    av::ApplicationInstance::get().evaluate();

    AVANGO_LOG(logger, av::logging::INFO, "after evaluate");
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("src: %1%") % src->referenceCount()));
    AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("dst: %1%") % dst->referenceCount()));

    dst->unreference();
    AVANGO_LOG(logger, av::logging::INFO, "after dst unref");
    if (!src_deleted) AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("src: %1%") % src->referenceCount()));
    if (!dst_deleted) AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("dst: %1%") % dst->referenceCount()));

    src->unreference();
    AVANGO_LOG(logger, av::logging::INFO, "after src unref");
    if (!src_deleted) AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("src: %1%") % src->referenceCount()));
    if (!dst_deleted) AVANGO_LOG(logger, av::logging::INFO, boost::str(boost::format("dst: %1%") % dst->referenceCount()));

    // simulate application exit
    av::ApplicationInstance::get().exit(false);

    CHECK(src_deleted);
    CHECK(dst_deleted);
  }

} // namespace

int main()
{
  logger.addConsoleAppender();
  av::ApplicationInstance::get();
  return UnitTest::RunAllTests();
}

