// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/osg/LoadImage.h>

#include <osgDB/ReadFile>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LoadImage"));
}

AV_FC_DEFINE(av::osg::LoadImage);

AV_FIELD_DEFINE(av::osg::SFLoadImage);
AV_FIELD_DEFINE(av::osg::MFLoadImage);

av::osg::LoadImage::LoadImage()
{
  AV_FC_ADD_FIELD(Filename, "");
  AV_FC_ADD_FIELD(Image, 0);
}

av::osg::LoadImage::~LoadImage()
{}

void
av::osg::LoadImage::initClass()
{
  if (!isTypeInitialized())
  {
    ::av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::osg::LoadImage, true);

    SFLoadImage::initClass("av::osg::SFLoadImage", "av::Field");
    MFLoadImage::initClass("av::osg::MFLoadImage", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::LoadImage::evaluateLocalSideEffect()
{
  FieldContainer::evaluateLocalSideEffect();
  LOG_TRACE(logger) << "evaluateLocalSideEffect()";

  ::osg::ref_ptr< ::osg::Image> image = osgDB::readImageFile(Filename.getValue());
  if (image.valid())
    Image.setValue(new av::osg::Image(image.get()));
  else
    Image.setValue(0);
}
