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

#include <avango/osg/Texture2DArray.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Texture2DArray"));
}

AV_FC_DEFINE(av::osg::Texture2DArray);

AV_FIELD_DEFINE(av::osg::SFTexture2DArray);
AV_FIELD_DEFINE(av::osg::MFTexture2DArray);

av::osg::Texture2DArray::Texture2DArray(::osg::Texture2DArray* osgtexture) :
  Texture(osgtexture),
  mOsgTexture2DArray(osgtexture)
{
  AV_FC_ADD_ADAPTOR_FIELD(Image,
                          boost::bind(&Texture2DArray::getImageCB, this, _1),
                          boost::bind(&Texture2DArray::setImageCB, this, _1));
}

/* virtual */
av::osg::Texture2DArray::~Texture2DArray()
{}

/* static */ void
av::osg::Texture2DArray::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Texture, av::osg::Texture2DArray, true);

    SFTexture2DArray::initClass("av::osg::SFTexture2DArray", "av::Field");
    MFTexture2DArray::initClass("av::osg::MFTexture2DArray", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Texture2DArray*
av::osg::Texture2DArray::getOsgTexture2DArray() const {
  return mOsgTexture2DArray;
}

/* virtual */ void
av::osg::Texture2DArray::getImageCB(const av::osg::MFImage::GetValueEvent& event)
{
  av::osg::MFImage::ContainerType& container(*event.getValuePtr());
  container.clear();
  for (int i = 0; i != mOsgTexture2DArray->getTextureDepth(); ++i)
  {
    container.push_back(av::osg::get_from_osg_object<av::osg::Image>(mOsgTexture2DArray->getImage(i)));
  }
}

/* virtual */ void
av::osg::Texture2DArray::setImageCB(const av::osg::MFImage::SetValueEvent& event)
{
  const av::osg::MFImage::ContainerType& container(event.getValue());
  mOsgTexture2DArray->setTextureDepth(container.size());
  int index = 0;
  bool is_mipmap = true;
  for (av::osg::MFImage::ContainerType::const_iterator i = container.begin(); i != container.end(); ++i, ++index)
  {
    is_mipmap &= (*i)->getOsgImage()->isMipmap();
    mOsgTexture2DArray->setImage(index, (*i)->getOsgImage());
  }

  // FIXME This should somehow happen automatically, but it does not.
  if (is_mipmap)
    mOsgTexture2DArray->allocateMipmapLevels();
}
