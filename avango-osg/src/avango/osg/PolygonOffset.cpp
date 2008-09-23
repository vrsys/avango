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
#include <avango/osg/PolygonOffset.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::PolygonOffset"));
}

AV_FC_DEFINE(av::osg::PolygonOffset);

AV_FIELD_DEFINE(av::osg::SFPolygonOffset);
AV_FIELD_DEFINE(av::osg::MFPolygonOffset);

av::osg::PolygonOffset::PolygonOffset(::osg::PolygonOffset* osgpolygonoffset) :
  StateAttribute(osgpolygonoffset),
  mOsgPolygonOffset(osgpolygonoffset)
{
  AV_FC_ADD_ADAPTOR_FIELD(Factor,
                            boost::bind(&PolygonOffset::getFactorCB, this, _1),
                            boost::bind(&PolygonOffset::setFactorCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Units,
                            boost::bind(&PolygonOffset::getUnitsCB, this, _1),
                            boost::bind(&PolygonOffset::setUnitsCB, this, _1));
}

av::osg::PolygonOffset::~PolygonOffset()
{}

void
av::osg::PolygonOffset::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::PolygonOffset, true);

    SFPolygonOffset::initClass("av::osg::SFPolygonOffset", "av::Field");
    MFPolygonOffset::initClass("av::osg::MFPolygonOffset", "av::Field");
  }
}

::osg::PolygonOffset*
av::osg::PolygonOffset::getOsgPolygonOffset() const
{
  return mOsgPolygonOffset;
}

/* virtual */ void
av::osg::PolygonOffset::getFactorCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPolygonOffset->getFactor();
}

/* virtual */ void
av::osg::PolygonOffset::setFactorCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPolygonOffset->setFactor(event.getValue());
}

/* virtual */ void
av::osg::PolygonOffset::getUnitsCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPolygonOffset->getUnits();
}

/* virtual */ void
av::osg::PolygonOffset::setUnitsCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPolygonOffset->setUnits(event.getValue());
}
