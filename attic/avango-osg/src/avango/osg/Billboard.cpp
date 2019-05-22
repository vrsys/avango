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
#include <avango/osg/Billboard.h>
#include <avango/Logger.h>
#include <osg/Version>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Billboard"));
}

AV_FC_DEFINE(av::osg::Billboard);

AV_FIELD_DEFINE(av::osg::SFBillboard);
AV_FIELD_DEFINE(av::osg::MFBillboard);

av::osg::Billboard::Billboard(::osg::Billboard* osgbillboard) : Geode(osgbillboard), mOsgBillboard(osgbillboard)
{
    AV_FC_ADD_ADAPTOR_FIELD(Mode, boost::bind(&Billboard::getModeCB, this, _1), boost::bind(&Billboard::setModeCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(Axis, boost::bind(&Billboard::getAxisCB, this, _1), boost::bind(&Billboard::setAxisCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(Normal, boost::bind(&Billboard::getNormalCB, this, _1), boost::bind(&Billboard::setNormalCB, this, _1));
}

av::osg::Billboard::~Billboard() {}

void av::osg::Billboard::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Geode::initClass();

        AV_FC_INIT(av::osg::Geode, av::osg::Billboard, true);

        SFBillboard::initClass("av::osg::SFBillboard", "av::Field");
        MFBillboard::initClass("av::osg::MFBillboard", "av::Field");
    }
}

osg::Billboard* av::osg::Billboard::getOsgBillboard() const { return mOsgBillboard; }

/* virtual */ void av::osg::Billboard::getModeCB(const SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgBillboard->getMode(); }

/* virtual */ void av::osg::Billboard::setModeCB(const SFUInt::SetValueEvent& event) { mOsgBillboard->setMode(static_cast<::osg::Billboard::Mode>(event.getValue())); }

/* virtual */ void av::osg::Billboard::getAxisCB(const av::osg::SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = mOsgBillboard->getAxis(); }

/* virtual */ void av::osg::Billboard::setAxisCB(const av::osg::SFVec3::SetValueEvent& event) { mOsgBillboard->setAxis(event.getValue()); }

/* virtual */ void av::osg::Billboard::getNormalCB(const av::osg::SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = mOsgBillboard->getNormal(); }

/* virtual */ void av::osg::Billboard::setNormalCB(const av::osg::SFVec3::SetValueEvent& event) { mOsgBillboard->setNormal(event.getValue()); }
