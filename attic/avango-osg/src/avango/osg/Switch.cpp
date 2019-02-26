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

#include <avango/osg/Switch.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Switch"));
}

AV_FC_DEFINE(av::osg::Switch);

AV_FIELD_DEFINE(av::osg::SFSwitch);
AV_FIELD_DEFINE(av::osg::MFSwitch);

av::osg::Switch::Switch(::osg::Switch* osgswitch) : Group(osgswitch), mOsgSwitch(osgswitch), mVisibleChildChanged(false), mVisibleChild(0)
{
    mOsgSwitch->setNewChildDefaultValue(true);
    AV_FC_ADD_ADAPTOR_FIELD(VisibleChild, boost::bind(&Switch::getVisibleChildCB, this, _1), boost::bind(&Switch::setVisibleChildCB, this, _1));
}

av::osg::Switch::~Switch() {}

void av::osg::Switch::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Group::initClass();

        AV_FC_INIT(av::osg::Group, av::osg::Switch, true);

        SFSwitch::initClass("av::osg::SFSwitch", "av::Field");
        MFSwitch::initClass("av::osg::MFSwitch", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

::osg::Switch* av::osg::Switch::getOsgSwitch() const { return mOsgSwitch; }

/* virtual */ void av::osg::Switch::evaluateLocalSideEffect()
{
    Group::evaluateLocalSideEffect();

    if(mVisibleChildChanged)
    {
        updateVisibleChild();
        mVisibleChildChanged = false;
    }
}

/* virtual */ void av::osg::Switch::getVisibleChildCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mVisibleChild; }

/* virtual */ void av::osg::Switch::setVisibleChildCB(const av::SFInt::SetValueEvent& event)
{
    mVisibleChild = event.getValue();
    mVisibleChildChanged = true;
}

/* virtual */ void av::osg::Switch::setChildrenCB(const av::osg::MFNode::SetValueEvent& event)
{
    Group::setChildrenCB(event);
    mVisibleChildChanged = true;
}

void av::osg::Switch::updateVisibleChild()
{
    if(mVisibleChild > 0) // single child on (starting with 1 for first child)
    {
        mOsgSwitch->setNewChildDefaultValue(false);
        mOsgSwitch->setSingleChildOn(mVisibleChild - 1);
    }
    if(mVisibleChild == 0) // all children on
    {
        mOsgSwitch->setNewChildDefaultValue(true);
        mOsgSwitch->setAllChildrenOn();
    }
    if(mVisibleChild < 0) // all children off
    {
        mOsgSwitch->setNewChildDefaultValue(false);
        mOsgSwitch->setAllChildrenOff();
    }
}
