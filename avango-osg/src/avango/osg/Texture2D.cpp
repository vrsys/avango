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

#include <avango/osg/Texture2D.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Texture2D"));
}

AV_FC_DEFINE(av::osg::Texture2D);

AV_FIELD_DEFINE(av::osg::SFTexture2D);
AV_FIELD_DEFINE(av::osg::MFTexture2D);

av::osg::Texture2D::Texture2D(::osg::Texture2D* osgtexture) :
  Texture(osgtexture),
  mOsgTexture2D(osgtexture)
{
  AV_FC_ADD_ADAPTOR_FIELD(Image,
                          boost::bind(&Texture2D::getImageCB, this, _1),
                          boost::bind(&Texture2D::setImageCB, this, _1));
}

/* virtual */
av::osg::Texture2D::~Texture2D()
{}

/* static */ void
av::osg::Texture2D::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Texture, av::osg::Texture2D, true);

    SFTexture2D::initClass("av::osg::SFTexture2D", "av::Field");
    MFTexture2D::initClass("av::osg::MFTexture2D", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Texture2D*
av::osg::Texture2D::getOsgTexture2D() const {
  return mOsgTexture2D;
}

/* virtual */ void
av::osg::Texture2D::getImageCB(const av::osg::SFImage::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Image>(mOsgTexture2D->getImage());
}

/* virtual */ void
av::osg::Texture2D::setImageCB(const av::osg::SFImage::SetValueEvent& event)
{
  if (event.getValue().isValid())
  {
    mOsgTexture2D->setImage(event.getValue()->getOsgImage());
  }
}
