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
    ::av::Object::initClass();

    AV_FC_INIT(av::Object, av::osg::LoadImage, true);

    SFLoadImage::initClass("av::osg::SFLoadImage", "av::Field");
    MFLoadImage::initClass("av::osg::MFLoadImage", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::LoadImage::evaluateLocalSideEffect()
{
  Object::evaluateLocalSideEffect();
  LOG_TRACE(logger) << "evaluateLocalSideEffect()";

  ::osg::ref_ptr< ::osg::Image> image = osgDB::readImageFile(Filename.getValue());
  Image.setValue(new av::osg::Image(image.get()));
}
