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

#include <avango/osg/StateAttribute.h>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::StateAttribute"));
}

AV_FC_DEFINE_ABSTRACT(av::osg::StateAttribute);

AV_FIELD_DEFINE(av::osg::SFStateAttribute);
AV_FIELD_DEFINE(av::osg::MFStateAttribute);

av::osg::StateAttribute::StateAttribute(::osg::StateAttribute* osgstateattribute) : Object(osgstateattribute), mOsgStateAttribute(osgstateattribute) {}

av::osg::StateAttribute::~StateAttribute() {}

void av::osg::StateAttribute::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Object::initClass();

        AV_FC_INIT_ABSTRACT(av::osg::Object, av::osg::StateAttribute, true);

        SFStateAttribute::initClass("av::osg::SFStateAttribute", "av::Field");
        MFStateAttribute::initClass("av::osg::MFStateAttribute", "av::Field");
    }
}

::osg::StateAttribute* av::osg::StateAttribute::getOsgStateAttribute() const { return mOsgStateAttribute; }
