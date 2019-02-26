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

#include <avango/osg/Texture.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Texture"));
}

AV_FC_DEFINE_ABSTRACT(av::osg::Texture);

AV_FIELD_DEFINE(av::osg::SFTexture);
AV_FIELD_DEFINE(av::osg::MFTexture);

av::osg::Texture::Texture(::osg::Texture* osgtexture) : Object(osgtexture), mOsgTexture(osgtexture)
{
    AV_FC_ADD_ADAPTOR_FIELD(WrapS, boost::bind(&Texture::getWrapCB, this, ::osg::Texture::WRAP_S, _1), boost::bind(&Texture::setWrapCB, this, ::osg::Texture::WRAP_S, _1));
    AV_FC_ADD_ADAPTOR_FIELD(WrapT, boost::bind(&Texture::getWrapCB, this, ::osg::Texture::WRAP_T, _1), boost::bind(&Texture::setWrapCB, this, ::osg::Texture::WRAP_T, _1));
    AV_FC_ADD_ADAPTOR_FIELD(WrapR, boost::bind(&Texture::getWrapCB, this, ::osg::Texture::WRAP_R, _1), boost::bind(&Texture::setWrapCB, this, ::osg::Texture::WRAP_R, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MinFilter, boost::bind(&Texture::getMinFilterCB, this, _1), boost::bind(&Texture::setMinFilterCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MagFilter, boost::bind(&Texture::getMagFilterCB, this, _1), boost::bind(&Texture::setMagFilterCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(InternalFormatMode, boost::bind(&Texture::getInternalFormatModeCB, this, _1), boost::bind(&Texture::setInternalFormatModeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(InternalFormat, boost::bind(&Texture::getInternalFormat, this, _1), boost::bind(&Texture::setInternalFormat, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MaxAnisotropy, boost::bind(&Texture::getMaxAnisotropyCB, this, _1), boost::bind(&Texture::setMaxAnisotropyCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(ResizeNonPowerOfTwoHint, boost::bind(&Texture::getResizeNonPowerOfTwoHint, this, _1), boost::bind(&Texture::setResizeNonPowerOfTwoHint, this, _1));
}

/* virtual */
av::osg::Texture::~Texture() {}

/* static */ void av::osg::Texture::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Object::initClass();

        AV_FC_INIT_ABSTRACT(av::osg::Object, av::osg::Texture, true);

        SFTexture::initClass("av::osg::SFTexture", "av::Field");
        MFTexture::initClass("av::osg::MFTexture", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

::osg::Texture* av::osg::Texture::getOsgTexture() const { return mOsgTexture; }

/* virtual */ void av::osg::Texture::getWrapCB(::osg::Texture::WrapParameter param, const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getWrap(param); }

/* virtual */ void av::osg::Texture::setWrapCB(::osg::Texture::WrapParameter param, const av::SFInt::SetValueEvent& event)
{
    mOsgTexture->setWrap(param, (::osg::Texture::WrapMode)(event.getValue()));
}

/* virtual */ void av::osg::Texture::getMinFilterCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getFilter(::osg::Texture::MIN_FILTER); }

/* virtual */ void av::osg::Texture::setMinFilterCB(const av::SFInt::SetValueEvent& event) { mOsgTexture->setFilter(::osg::Texture::MIN_FILTER, (::osg::Texture::FilterMode)(event.getValue())); }

/* virtual */ void av::osg::Texture::getMagFilterCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getFilter(::osg::Texture::MAG_FILTER); }

/* virtual */ void av::osg::Texture::setMagFilterCB(const av::SFInt::SetValueEvent& event) { mOsgTexture->setFilter(::osg::Texture::MAG_FILTER, (::osg::Texture::FilterMode)(event.getValue())); }

/* virtual */ void av::osg::Texture::getInternalFormatModeCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getInternalFormatMode(); }

/* virtual */ void av::osg::Texture::setInternalFormatModeCB(const av::SFInt::SetValueEvent& event) { mOsgTexture->setInternalFormatMode((::osg::Texture::InternalFormatMode)(event.getValue())); }

/* virtual */ void av::osg::Texture::getInternalFormat(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getInternalFormat(); }

/* virtual */ void av::osg::Texture::setInternalFormat(const av::SFInt::SetValueEvent& event) { mOsgTexture->setInternalFormat(event.getValue()); }

/* virtual */ void av::osg::Texture::getMaxAnisotropyCB(const av::SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getMaxAnisotropy(); }

/* virtual */ void av::osg::Texture::setMaxAnisotropyCB(const av::SFFloat::SetValueEvent& event) { mOsgTexture->setMaxAnisotropy(event.getValue()); }

/* virtual */ void av::osg::Texture::getResizeNonPowerOfTwoHint(const av::SFBool::GetValueEvent& event) { *(event.getValuePtr()) = mOsgTexture->getResizeNonPowerOfTwoHint(); }

/* virtual */ void av::osg::Texture::setResizeNonPowerOfTwoHint(const av::SFBool::SetValueEvent& event) { mOsgTexture->setResizeNonPowerOfTwoHint(event.getValue()); }
