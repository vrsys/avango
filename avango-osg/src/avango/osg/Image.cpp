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

#include <avango/osg/Image.h>

AV_FC_DEFINE(av::osg::Image);

AV_FIELD_DEFINE(av::osg::SFImage);
AV_FIELD_DEFINE(av::osg::MFImage);

av::osg::Image::Image(::osg::Image* osgimage) :
  Object(osgimage),
  mOsgImage(osgimage)
{
}

av::osg::Image::~Image()
{}

void
av::osg::Image::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::Image, true);

    SFImage::initClass("av::osg::SFImage", "av::Field");
    MFImage::initClass("av::osg::MFImage", "av::Field");
  }
}

::osg::Image*
av::osg::Image::getOsgImage() const
{
  return mOsgImage;
}
