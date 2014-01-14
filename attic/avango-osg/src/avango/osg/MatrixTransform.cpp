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

#include <avango/osg/MatrixTransform.h>

#include <boost/bind.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::MatrixTransform"));
}

AV_FC_DEFINE(av::osg::MatrixTransform);

AV_FIELD_DEFINE(av::osg::SFMatrixTransform);
AV_FIELD_DEFINE(av::osg::MFMatrixTransform);

av::osg::MatrixTransform::MatrixTransform(::osg::MatrixTransform* osgmatrixtransform) :
  Transform(osgmatrixtransform),
  mOsgMatrixTransform(osgmatrixtransform)
{
  AV_FC_ADD_ADAPTOR_FIELD(Matrix,
                          boost::bind(&MatrixTransform::getMatrixCB, this, _1),
                          boost::bind(&MatrixTransform::setMatrixCB, this, _1));
}

/* virtual */
av::osg::MatrixTransform::~MatrixTransform()
{}

/* static */ void
av::osg::MatrixTransform::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Transform::initClass();

    AV_FC_INIT(av::osg::Transform, av::osg::MatrixTransform, true);

    SFMatrixTransform::initClass("av::osg::SFMatrixTransform", "av::Field");
    MFMatrixTransform::initClass("av::osg::MFMatrixTransform", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::MatrixTransform*
av::osg::MatrixTransform::getOsgMatrixTransform() const
{
  return mOsgMatrixTransform;
}

/* virtual */ void
av::osg::MatrixTransform::getMatrixCB(const av::osg::SFMatrix::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgMatrixTransform->getMatrix();
}

/* virtual */ void
av::osg::MatrixTransform::setMatrixCB(const av::osg::SFMatrix::SetValueEvent& event)
{
  mOsgMatrixTransform->setMatrix(event.getValue());
}
