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
#include <avango/osg/Depth.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Depth"));
}

AV_FC_DEFINE(av::osg::Depth);

AV_FIELD_DEFINE(av::osg::SFDepth);
AV_FIELD_DEFINE(av::osg::MFDepth);

av::osg::Depth::Depth(::osg::Depth* osgdepth) :
  StateAttribute(osgdepth),
  mOsgDepth(osgdepth)
{
  AV_FC_ADD_ADAPTOR_FIELD(Function,
                            boost::bind(&Depth::getFunctionCB, this, _1),
                            boost::bind(&Depth::setFunctionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(ZNear,
                            boost::bind(&Depth::getZNearCB, this, _1),
                            boost::bind(&Depth::setZNearCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(ZFar,
                            boost::bind(&Depth::getZFarCB, this, _1),
                            boost::bind(&Depth::setZFarCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(WriteMask,
                            boost::bind(&Depth::getWriteMaskCB, this, _1),
                            boost::bind(&Depth::setWriteMaskCB, this, _1));
}

av::osg::Depth::~Depth()
{}

void
av::osg::Depth::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::Depth, true);

    SFDepth::initClass("av::osg::SFDepth", "av::Field");
    MFDepth::initClass("av::osg::MFDepth", "av::Field");
  }
}

::osg::Depth*
av::osg::Depth::getOsgDepth() const
{
  return mOsgDepth;
}

/* virtual */ void
av::osg::Depth::getFunctionCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgDepth->getFunction();
}

/* virtual */ void
av::osg::Depth::setFunctionCB(const av::SFInt::SetValueEvent& event)
{
  mOsgDepth->setFunction((::osg::Depth::Function)(event.getValue()));
}

/* virtual */ void
av::osg::Depth::getZNearCB(const av::SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgDepth->getZNear();
}

/* virtual */ void
av::osg::Depth::setZNearCB(const av::SFDouble::SetValueEvent& event)
{
  mOsgDepth->setZNear(event.getValue());
}

/* virtual */ void
av::osg::Depth::getZFarCB(const av::SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgDepth->getZFar();
}

/* virtual */ void
av::osg::Depth::setZFarCB(const av::SFDouble::SetValueEvent& event)
{
  mOsgDepth->setZFar(event.getValue());
}

/* virtual */ void
av::osg::Depth::getWriteMaskCB(const av::SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgDepth->getWriteMask();
}

/* virtual */ void
av::osg::Depth::setWriteMaskCB(const av::SFBool::SetValueEvent& event)
{
  mOsgDepth->setWriteMask(event.getValue());
}
