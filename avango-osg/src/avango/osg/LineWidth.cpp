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
#include <avango/osg/LineWidth.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LineWidth"));
}

AV_FC_DEFINE(av::osg::LineWidth);

AV_FIELD_DEFINE(av::osg::SFLineWidth);
AV_FIELD_DEFINE(av::osg::MFLineWidth);

av::osg::LineWidth::LineWidth(::osg::LineWidth* osglinewidth) :
  StateAttribute(osglinewidth),
  mOsgLineWidth(osglinewidth)
{
  AV_FC_ADD_ADAPTOR_FIELD(Width,
                            boost::bind(&LineWidth::getWidthCB, this, _1),
                            boost::bind(&LineWidth::setWidthCB, this, _1));

}

av::osg::LineWidth::~LineWidth()
{}

void
av::osg::LineWidth::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::LineWidth, true);

    SFLineWidth::initClass("av::osg::SFLineWidth", "av::Field");
    MFLineWidth::initClass("av::osg::MFLineWidth", "av::Field");
  }
}

::osg::LineWidth*
av::osg::LineWidth::getOsgLineWidth() const
{
  return mOsgLineWidth;
}

/* virtual */ void
av::osg::LineWidth::getWidthCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLineWidth->getWidth();
}

/* virtual */ void
av::osg::LineWidth::setWidthCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLineWidth->setWidth((GLenum)(event.getValue()));
}


