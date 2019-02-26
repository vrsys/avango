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

#include <avango/osg/TextureCubeMap.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::osg::TextureCubeMap"));
}

AV_FC_DEFINE(av::osg::TextureCubeMap);

AV_FIELD_DEFINE(av::osg::SFTextureCubeMap);
AV_FIELD_DEFINE(av::osg::MFTextureCubeMap);

av::osg::TextureCubeMap::TextureCubeMap(::osg::TextureCubeMap* osgtexture) : Texture(osgtexture), mOsgTextureCubeMap(osgtexture)
{
    AV_FC_ADD_ADAPTOR_FIELD(Images, boost::bind(&TextureCubeMap::getImagesCB, this, _1), boost::bind(&TextureCubeMap::setImagesCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(TextureWidth, boost::bind(&TextureCubeMap::getTextureWidthCB, this, _1), boost::bind(&TextureCubeMap::setTextureWidthCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(TextureHeight, boost::bind(&TextureCubeMap::getTextureHeightCB, this, _1), boost::bind(&TextureCubeMap::setTextureHeightCB, this, _1));
}

/* virtual */
av::osg::TextureCubeMap::~TextureCubeMap() {}

/* static */ void av::osg::TextureCubeMap::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Object::initClass();

        AV_FC_INIT(av::osg::Texture, av::osg::TextureCubeMap, true);

        SFTextureCubeMap::initClass("av::osg::SFTextureCubeMap", "av::Field");
        MFTextureCubeMap::initClass("av::osg::MFTextureCubeMap", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

::osg::TextureCubeMap* av::osg::TextureCubeMap::getOsgTextureCubeMap() const { return mOsgTextureCubeMap; }

/* virtual */ void av::osg::TextureCubeMap::getImagesCB(const av::osg::MFImage::GetValueEvent& event)
{
    MFImage::ContainerType result;
    for(unsigned int i = 0; i != mOsgTextureCubeMap->getNumImages(); ++i)
    {
        ::osg::Image* osg_image = mOsgTextureCubeMap->getImage(i);
        if(!osg_image)
        {
            result.push_back(new Image);
            continue;
        }
        Image* image = dynamic_cast<Image*>(osg_image->getUserData());
        if(!image)
            result.push_back(image);
    }
    *(event.getValuePtr()) = result;
}

/* virtual */ void av::osg::TextureCubeMap::setImagesCB(const av::osg::MFImage::SetValueEvent& event)
{
    MFImage::ContainerType images = event.getValue();
    unsigned int face = 0;
    for(MFImage::ContainerType::const_iterator i = images.begin(); i != images.end(); ++i)
    {
        mOsgTextureCubeMap->setImage(face, (*i)->getOsgImage());
        ++face;
    }
}

/* virtual */ void av::osg::TextureCubeMap::getTextureWidthCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTextureCubeMap->getTextureWidth(); }

/* virtual */ void av::osg::TextureCubeMap::setTextureWidthCB(const av::SFInt::SetValueEvent& event) { mOsgTextureCubeMap->setTextureWidth(event.getValue()); }

/* virtual */ void av::osg::TextureCubeMap::getTextureHeightCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTextureCubeMap->getTextureWidth(); }

/* virtual */ void av::osg::TextureCubeMap::setTextureHeightCB(const av::SFInt::SetValueEvent& event) { mOsgTextureCubeMap->setTextureHeight(event.getValue()); }
