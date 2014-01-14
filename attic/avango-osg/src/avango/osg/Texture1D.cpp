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

#include <avango/osg/Texture1D.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Texture1D"));
}

AV_FC_DEFINE(av::osg::Texture1D);

AV_FIELD_DEFINE(av::osg::SFTexture1D);
AV_FIELD_DEFINE(av::osg::MFTexture1D);

av::osg::Texture1D::Texture1D(::osg::Texture1D* osgtexture) :
  Texture(osgtexture),
  mOsgTexture1D(osgtexture)
{
  AV_FC_ADD_ADAPTOR_FIELD(Image,
                          boost::bind(&Texture1D::getImageCB, this, _1),
                          boost::bind(&Texture1D::setImageCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(TextureWidth,
                          boost::bind(&Texture1D::getTextureWidthCB, this, _1),
                          boost::bind(&Texture1D::setTextureWidthCB, this, _1));
}

/* virtual */
av::osg::Texture1D::~Texture1D()
{}

/* static */ void
av::osg::Texture1D::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Texture, av::osg::Texture1D, true);

    SFTexture1D::initClass("av::osg::SFTexture1D", "av::Field");
    MFTexture1D::initClass("av::osg::MFTexture1D", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Texture1D*
av::osg::Texture1D::getOsgTexture1D() const {
  return mOsgTexture1D;
}

/* virtual */ void
av::osg::Texture1D::getImageCB(const av::osg::SFImage::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Image>(mOsgTexture1D->getImage());
}

/* virtual */ void
av::osg::Texture1D::setImageCB(const av::osg::SFImage::SetValueEvent& event)
{
  if (event.getValue().isValid())
  {
    mOsgTexture1D->setImage(event.getValue()->getOsgImage());
  }
}

/* virtual */ void
av::osg::Texture1D::getTextureWidthCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgTexture1D->getTextureWidth();
}

/* virtual */ void
av::osg::Texture1D::setTextureWidthCB(const av::SFInt::SetValueEvent& event)
{
  mOsgTexture1D->setTextureWidth(event.getValue());
}
