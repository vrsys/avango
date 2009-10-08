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

#include <avango/osg/ImageStream.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::ImageStream"));
}

AV_FC_DEFINE(av::osg::ImageStream);

AV_FIELD_DEFINE(av::osg::SFImageStream);
AV_FIELD_DEFINE(av::osg::MFImageStream);

av::osg::ImageStream::ImageStream(::osg::ImageStream* osgtexture) :
  Image(osgtexture),
  mOsgImageStream(osgtexture)
{
  AV_FC_ADD_ADAPTOR_FIELD(Loop,
                          boost::bind(&ImageStream::getLoopCB, this, _1),
                          boost::bind(&ImageStream::setLoopCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Status,
                          boost::bind(&ImageStream::getStatusCB, this, _1),
                          boost::bind(&ImageStream::setStatusCB, this, _1));
}

/* virtual */
av::osg::ImageStream::~ImageStream()
{}

/* static */ void
av::osg::ImageStream::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Image, av::osg::ImageStream, true);

    SFImageStream::initClass("av::osg::SFImageStream", "av::Field");
    MFImageStream::initClass("av::osg::MFImageStream", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::ImageStream*
av::osg::ImageStream::getOsgImageStream() const {
  return mOsgImageStream;
}

/* virtual */ void
av::osg::ImageStream::getLoopCB(const av::SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = (mOsgImageStream->getLoopingMode() == ::osg::ImageStream::LOOPING);
}

/* virtual */ void
av::osg::ImageStream::setLoopCB(const av::SFBool::SetValueEvent& event)
{
  if (event.getValue())
    mOsgImageStream->setLoopingMode(::osg::ImageStream::LOOPING);
  else
    mOsgImageStream->setLoopingMode(::osg::ImageStream::NO_LOOPING);
}

/* virtual */ void
av::osg::ImageStream::getStatusCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgImageStream->getStatus();
}

/* virtual */ void
av::osg::ImageStream::setStatusCB(const av::SFInt::SetValueEvent& event)
{
  //if (event.getValue())
  switch (static_cast< ::osg::ImageStream::LoopingMode>(event.getValue()))
  {
    case ::osg::ImageStream::PLAYING :
      mOsgImageStream->play();
      break;
    case ::osg::ImageStream::PAUSED :
      mOsgImageStream->pause();
      break;
    case ::osg::ImageStream::REWINDING :
      mOsgImageStream->rewind();
      break;
    // does not handle ::osg::ImageStream::INVALID :
    default:
      break;
  }
}

