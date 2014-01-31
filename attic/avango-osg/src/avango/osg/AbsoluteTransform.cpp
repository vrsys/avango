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

#include <avango/osg/AbsoluteTransform.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::AbsoluteTransform"));
}

AV_FC_DEFINE(av::osg::AbsoluteTransform);

AV_FIELD_DEFINE(av::osg::SFAbsoluteTransform);
AV_FIELD_DEFINE(av::osg::MFAbsoluteTransform);

av::osg::AbsoluteTransform::AbsoluteTransform()
{
  AV_FC_ADD_FIELD(AbsoluteMatrix, ::osg::Matrix());

  alwaysEvaluate(true);
}

/* virtual */
av::osg::AbsoluteTransform::~AbsoluteTransform()
{}

/* static */ void
av::osg::AbsoluteTransform::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::osg::AbsoluteTransform, false);

    SFAbsoluteTransform::initClass("av::osg::SFAbsoluteTransform", "av::Field");
    MFAbsoluteTransform::initClass("av::osg::MFAbsoluteTransform", "av::Field");
  }
}

/* virtual */ void
av::osg::AbsoluteTransform::evaluate()
{
  ::osg::Matrix abs_mat = getAbsoluteTransform(this);
  if (AbsoluteMatrix.getValue() != abs_mat)
    AbsoluteMatrix.setValue(abs_mat);
}
