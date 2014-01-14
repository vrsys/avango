// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/osg/Object.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Object"));
}

AV_FC_DEFINE_ABSTRACT(av::osg::Object);

AV_FIELD_DEFINE(av::osg::SFObject);
AV_FIELD_DEFINE(av::osg::MFObject);

av::osg::Object::Object(::osg::Object *osgobject) :
  mOsgObject(osgobject)
{
  AV_ASSERT(mOsgObject != 0);
  mOsgObject->setUserData(new ObjectLink(this));
  mOsgObject->addObserver(this);
  av::Base::refImpl();
}

av::osg::Object::~Object()
{}

void
av::osg::Object::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT_ABSTRACT(av::FieldContainer, av::osg::Object, true);

    SFObject::initClass("av::osg::SFObject", "av::Field");
    MFObject::initClass("av::osg::MFObject", "av::Field");
  }
}

::osg::Object*
av::osg::Object::getOsgObject() const
{
  return mOsgObject;
}

/* virtual */ void
av::osg::Object::refImpl()
{
  mOsgObject->ref();
}

/* virtual */ void
av::osg::Object::unrefImpl()
{
  mOsgObject->unref();
}

/* virtual */ void
av::osg::Object::unrefWithoutDeletionImpl()
{
  mOsgObject->unref_nodelete();
}

/* virtual */ int
av::osg::Object::refCountImpl()
{
  return mOsgObject->referenceCount();
}

/* virtual */ void
av::osg::Object::objectDeleted(void*)
{
  av::Base::unrefImpl();
}
