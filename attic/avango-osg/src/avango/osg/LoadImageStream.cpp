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

av::osg::LoadImageStream::~LoadImageStream() {}

void av::osg::LoadImageStream::initClass()
{
    if(!isTypeInitialized())
    {
        ::av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::osg::LoadImageStream, true);

        SFLoadImageStream::initClass("av::osg::SFLoadImageStream", "av::Field");
        MFLoadImageStream::initClass("av::osg::MFLoadImageStream", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

/* virtual */ void av::osg::LoadImageStream::evaluateLocalSideEffect()
{
    FieldContainer::evaluateLocalSideEffect();
    LOG_TRACE(logger) << "evaluateLocalSideEffect()";

    ::osg::ref_ptr<::osg::Image> image = osgDB::readImageFile(Filename.getValue());
    // if (imagestream.valid())
    if(dynamic_cast<::osg::ImageStream*>(image.get()))
        ImageStream.setValue(new av::osg::ImageStream(dynamic_cast<::osg::ImageStream*>(image.get())));
    else
        ImageStream.setValue(0);
}
