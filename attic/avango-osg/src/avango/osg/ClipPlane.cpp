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
#include <avango/osg/ClipPlane.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::ClipPlane"));
}

AV_FC_DEFINE(av::osg::ClipPlane);

AV_FIELD_DEFINE(av::osg::SFClipPlane);
AV_FIELD_DEFINE(av::osg::MFClipPlane);

av::osg::ClipPlane::ClipPlane(::osg::ClipPlane* osgClipPlane) : StateAttribute(osgClipPlane), mOsgClipPlane(osgClipPlane)
{
    AV_FC_ADD_ADAPTOR_FIELD(PlaneNumber, boost::bind(&ClipPlane::getPlaneNumberCB, this, _1), boost::bind(&ClipPlane::setPlaneNumberCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Plane, boost::bind(&ClipPlane::getPlaneCB, this, _1), boost::bind(&ClipPlane::setPlaneCB, this, _1));
}

av::osg::ClipPlane::~ClipPlane() {}

void av::osg::ClipPlane::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::StateAttribute::initClass();

        AV_FC_INIT(av::osg::StateAttribute, av::osg::ClipPlane, true);

        SFClipPlane::initClass("av::osg::SFClipPlane", "av::Field");
        MFClipPlane::initClass("av::osg::MFClipPlane", "av::Field");
    }
}

::osg::ClipPlane* av::osg::ClipPlane::getOsgClipPlane() const { return mOsgClipPlane; }

/* virtual */ void av::osg::ClipPlane::getPlaneNumberCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgClipPlane->getClipPlaneNum(); }

/* virtual */ void av::osg::ClipPlane::setPlaneNumberCB(const av::SFInt::SetValueEvent& event) { mOsgClipPlane->setClipPlaneNum(event.getValue()); }

/* virtual */ void av::osg::ClipPlane::getPlaneCB(const av::osg::SFVec4::GetValueEvent& event) { *(event.getValuePtr()) = mOsgClipPlane->getClipPlane(); }

/* virtual */ void av::osg::ClipPlane::setPlaneCB(const av::osg::SFVec4::SetValueEvent& event)
{
    ::osg::Vec4d plane = event.getValue();
    mOsgClipPlane->setClipPlane(plane);
}
