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

#include <avango/osg/Drawable.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Drawable"));
}

AV_FC_DEFINE_ABSTRACT(av::osg::Drawable);

AV_FIELD_DEFINE(av::osg::SFDrawable);
AV_FIELD_DEFINE(av::osg::MFDrawable);

av::osg::Drawable::Drawable(::osg::Drawable* osgdrawable) :
  Object(osgdrawable),
  mOsgDrawable(osgdrawable)
{
  AV_FC_ADD_ADAPTOR_FIELD(StateSet,
                            boost::bind(&Drawable::getStateSetCB, this, _1),
                            boost::bind(&Drawable::setStateSetCB, this, _1));
}

av::osg::Drawable::~Drawable()
{}

void
av::osg::Drawable::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT_ABSTRACT(av::osg::Object, av::osg::Drawable, true);

    SFDrawable::initClass("av::osg::SFDrawable", "av::Field");
    MFDrawable::initClass("av::osg::MFDrawable", "av::Field");
  }
}

::osg::Drawable*
av::osg::Drawable::getOsgDrawable() const
{
  return mOsgDrawable;
}

/* virtual */ void
av::osg::Drawable::getStateSetCB(const av::osg::SFStateSet::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::StateSet>(mOsgDrawable->getStateSet());
}

/* virtual */ void
av::osg::Drawable::setStateSetCB(const av::osg::SFStateSet::SetValueEvent& event)
{
  if (!event.getValue().isValid())
  {
    return;
  }

  mOsgDrawable->setStateSet(event.getValue()->getOsgStateSet());
}
