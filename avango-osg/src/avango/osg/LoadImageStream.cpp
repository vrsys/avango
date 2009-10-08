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

#include <avango/osg/LoadImageStream.h>

#include <osgDB/ReadFile>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LoadImageStream"));
}

AV_FC_DEFINE(av::osg::LoadImageStream);

AV_FIELD_DEFINE(av::osg::SFLoadImageStream);
AV_FIELD_DEFINE(av::osg::MFLoadImageStream);

av::osg::LoadImageStream::LoadImageStream() 
{
  AV_FC_ADD_FIELD(Filename, "");
  AV_FC_ADD_FIELD(ImageStream, 0);
}

av::osg::LoadImageStream::~LoadImageStream()
{}

void
av::osg::LoadImageStream::initClass()
{
  if (!isTypeInitialized())
  {
    ::av::Object::initClass();

    AV_FC_INIT(av::Object, av::osg::LoadImageStream, true);

    SFLoadImageStream::initClass("av::osg::SFLoadImageStream", "av::Field");
    MFLoadImageStream::initClass("av::osg::MFLoadImageStream", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::LoadImageStream::evaluateLocalSideEffect()
{
  Object::evaluateLocalSideEffect();
  LOG_TRACE(logger) << "evaluateLocalSideEffect()";

  ::osg::ref_ptr< ::osg::Image> image = osgDB::readImageFile(Filename.getValue());
  //if (imagestream.valid())
  if (dynamic_cast< ::osg::ImageStream*>(image.get()))
    ImageStream.setValue(new av::osg::ImageStream(dynamic_cast< ::osg::ImageStream*>(image.get())));
  else
    ImageStream.setValue(0);
}
