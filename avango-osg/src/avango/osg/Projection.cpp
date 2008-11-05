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

#include <avango/osg/Projection.h>

#include <boost/bind.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Projection"));
}

AV_FC_DEFINE(av::osg::Projection);

AV_FIELD_DEFINE(av::osg::SFProjection);
AV_FIELD_DEFINE(av::osg::MFProjection);

av::osg::Projection::Projection(::osg::Projection* osgprojection) :
  Group(osgprojection),
  mOsgProjection(osgprojection)
{
  AV_FC_ADD_ADAPTOR_FIELD(Matrix,
                          boost::bind(&Projection::getMatrixCB, this, _1),
                          boost::bind(&Projection::setMatrixCB, this, _1));
}

/* virtual */
av::osg::Projection::~Projection()
{}

/* static */ void
av::osg::Projection::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::Projection, true);

    SFProjection::initClass("av::osg::SFProjection", "av::Field");
    MFProjection::initClass("av::osg::MFProjection", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}


::osg::Projection*
av::osg::Projection::getOsgProjection() const
{
  return mOsgProjection;
}

/* virtual */ void
av::osg::Projection::getMatrixCB(const av::osg::SFMatrix::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgProjection->getMatrix();
}

/* virtual */ void
av::osg::Projection::setMatrixCB(const av::osg::SFMatrix::SetValueEvent& event)
{
  mOsgProjection->setMatrix(event.getValue());
}
