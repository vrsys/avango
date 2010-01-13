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

#include <avango/osg/CameraAttachment.h>
#include <osg/Camera>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::CameraAttachment"));
}

AV_FC_DEFINE(av::osg::CameraAttachment);

AV_FIELD_DEFINE(av::osg::SFCameraAttachment);
AV_FIELD_DEFINE(av::osg::MFCameraAttachment);

av::osg::CameraAttachment::CameraAttachment()
{
  AV_FC_ADD_FIELD(BufferComponent, ::osg::Camera::COLOR_BUFFER);
  AV_FC_ADD_FIELD(InternalFormat, GL_RGBA);
  AV_FC_ADD_FIELD(Texture, Link< ::av::osg::Texture >());
  AV_FC_ADD_FIELD(Image, Link< ::av::osg::Image >());
  AV_FC_ADD_FIELD(Level, 0);
  AV_FC_ADD_FIELD(Face, 0);
  AV_FC_ADD_FIELD(MIPMapGeneration, false);
  AV_FC_ADD_FIELD(MultisampleSamples, 0);
  AV_FC_ADD_FIELD(MultisampleColorSamples, 0);
}

av::osg::CameraAttachment::~CameraAttachment()
{}

void
av::osg::CameraAttachment::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::osg::CameraAttachment, true);

    SFCameraAttachment::initClass("av::osg::SFCameraAttachment", "av::Field");
    MFCameraAttachment::initClass("av::osg::MFCameraAttachment", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}
