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
#include <avango/osg/Fog.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Fog"));
}

AV_FC_DEFINE(av::osg::Fog);

AV_FIELD_DEFINE(av::osg::SFFog);
AV_FIELD_DEFINE(av::osg::MFFog);

av::osg::Fog::Fog(::osg::Fog* osgfog) :
  StateAttribute(osgfog),
  mOsgFog(osgfog)
{
  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                            boost::bind(&Fog::getModeCB, this, _1),
                            boost::bind(&Fog::setModeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Density,
                            boost::bind(&Fog::getDensityCB, this, _1),
                            boost::bind(&Fog::setDensityCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Start,
                            boost::bind(&Fog::getStartCB, this, _1),
                            boost::bind(&Fog::setStartCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(End,
                            boost::bind(&Fog::getEndCB, this, _1),
                            boost::bind(&Fog::setEndCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Color,
                            boost::bind(&Fog::getColorCB, this, _1),
                            boost::bind(&Fog::setColorCB, this, _1));
}

av::osg::Fog::~Fog()
{}

void
av::osg::Fog::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::Fog, true);

    SFFog::initClass("av::osg::SFFog", "av::Field");
    MFFog::initClass("av::osg::MFFog", "av::Field");

    sSclassTypeId.setDistributable(true);
  }
}

::osg::Fog*
av::osg::Fog::getOsgFog() const
{
  return mOsgFog;
}

/* virtual */ void
av::osg::Fog::getModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgFog->getMode();
}

/* virtual */ void
av::osg::Fog::setModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgFog->setMode(static_cast< ::osg::Fog::Mode>(event.getValue()));
}

/* virtual */ void
av::osg::Fog::getDensityCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgFog->getDensity();
}

/* virtual */ void
av::osg::Fog::setDensityCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgFog->setDensity(event.getValue());
}

/* virtual */ void
av::osg::Fog::getStartCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgFog->getStart();
}

/* virtual */ void
av::osg::Fog::setStartCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgFog->setStart(event.getValue());
}

/* virtual */ void
av::osg::Fog::getEndCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgFog->getEnd();
}

/* virtual */ void
av::osg::Fog::setEndCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgFog->setEnd(event.getValue());
}

/* virtual */ void
av::osg::Fog::getColorCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgFog->getColor();
}

/* virtual */ void
av::osg::Fog::setColorCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgFog->setColor(event.getValue());
}

void
av::osg::Fog::touchFields()
{
  Color.touch();
  Density.touch();
  Start.touch();
  End.touch();
}
