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

#include <avango/osg/Camera.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <vector>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Camera"));
}

AV_FC_DEFINE(av::osg::Camera);

AV_FIELD_DEFINE(av::osg::SFCamera);
AV_FIELD_DEFINE(av::osg::MFCamera);

av::osg::Camera::Camera(::osg::Camera* osgcamera) : Transform(osgcamera), mOsgCamera(osgcamera)
{
    AV_FC_ADD_ADAPTOR_FIELD(ClearMask, boost::bind(&Camera::getClearMaskCB, this, _1), boost::bind(&Camera::setClearMaskCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(CullMask, boost::bind(&Camera::getCullMaskCB, this, _1), boost::bind(&Camera::setCullMaskCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(InheritanceMask, boost::bind(&Camera::getInheritanceMaskCB, this, _1), boost::bind(&Camera::setInheritanceMaskCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ClearColor, boost::bind(&Camera::getClearColorCB, this, _1), boost::bind(&Camera::setClearColorCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ViewMatrix, boost::bind(&Camera::getViewMatrixCB, this, _1), boost::bind(&Camera::setViewMatrixCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ProjectionMatrix, boost::bind(&Camera::getProjectionMatrixCB, this, _1), boost::bind(&Camera::setProjectionMatrixCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(Viewport, boost::bind(&Camera::getViewportCB, this, _1), boost::bind(&Camera::setViewportCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(RenderOrder, boost::bind(&Camera::getRenderOrderCB, this, _1), boost::bind(&Camera::setRenderOrderCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(RenderTargetImplementation, boost::bind(&Camera::getRenderTargetImplementationCB, this, _1), boost::bind(&Camera::setRenderTargetImplementationCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(Attachment, boost::bind(&Camera::getAttachmentCB, this, _1), boost::bind(&Camera::setAttachmentCB, this, _1));
}

av::osg::Camera::~Camera() {}

void av::osg::Camera::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Transform::initClass();

        AV_FC_INIT(av::osg::Transform, av::osg::Camera, true);

        SFCamera::initClass("av::osg::SFCamera", "av::Field");
        MFCamera::initClass("av::osg::MFCamera", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

::osg::Camera* av::osg::Camera::getOsgCamera() const { return mOsgCamera; }

void av::osg::Camera::getClearMaskCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getClearMask(); }

void av::osg::Camera::setClearMaskCB(const av::SFInt::SetValueEvent& event) { mOsgCamera->setClearMask(static_cast<GLbitfield>(event.getValue())); }

void av::osg::Camera::getCullMaskCB(const av::SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getCullMask(); }

void av::osg::Camera::setCullMaskCB(const av::SFUInt::SetValueEvent& event) { mOsgCamera->setCullMask(event.getValue()); }

void av::osg::Camera::getInheritanceMaskCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getInheritanceMask(); }

void av::osg::Camera::setInheritanceMaskCB(const av::SFInt::SetValueEvent& event) { mOsgCamera->setInheritanceMask(event.getValue()); }

void av::osg::Camera::getClearColorCB(const av::osg::SFVec4::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getClearColor(); }

void av::osg::Camera::setClearColorCB(const av::osg::SFVec4::SetValueEvent& event) { mOsgCamera->setClearColor(event.getValue()); }

void av::osg::Camera::getViewMatrixCB(const av::osg::SFMatrix::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getViewMatrix(); }

void av::osg::Camera::setViewMatrixCB(const av::osg::SFMatrix::SetValueEvent& event) { mOsgCamera->setViewMatrix(event.getValue()); }

void av::osg::Camera::getProjectionMatrixCB(const av::osg::SFMatrix::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getProjectionMatrix(); }

void av::osg::Camera::setProjectionMatrixCB(const av::osg::SFMatrix::SetValueEvent& event) { mOsgCamera->setProjectionMatrix(event.getValue()); }

void av::osg::Camera::getViewportCB(const av::osg::SFVec4::GetValueEvent& event)
{
    ::osg::Vec4 result;
    ::osg::Viewport* viewport(mOsgCamera->getViewport());
    if(viewport)
    {
        result.x() = viewport->x();
        result.y() = viewport->y();
        result.z() = viewport->width();
        result.w() = viewport->height();
    }
    *(event.getValuePtr()) = result;
}

void av::osg::Camera::setViewportCB(const av::osg::SFVec4::SetValueEvent& event)
{
    ::osg::Vec4 v(event.getValue());
    mOsgCamera->setViewport(v.x(), v.y(), v.z(), v.w());
}

void av::osg::Camera::getRenderOrderCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getRenderOrder(); }

void av::osg::Camera::setRenderOrderCB(const av::SFInt::SetValueEvent& event) { mOsgCamera->setRenderOrder(static_cast<::osg::Camera::RenderOrder>(event.getValue())); }

void av::osg::Camera::getRenderTargetImplementationCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgCamera->getRenderTargetImplementation(); }

void av::osg::Camera::setRenderTargetImplementationCB(const av::SFInt::SetValueEvent& event)
{
    mOsgCamera->setRenderTargetImplementation(static_cast<::osg::Camera::RenderTargetImplementation>(event.getValue()));
}

/* virtual */ void av::osg::Camera::getAttachmentCB(const av::osg::MFCameraAttachment::GetValueEvent& event)
{
    ::osg::Camera::BufferAttachmentMap& buffer_map(mOsgCamera->getBufferAttachmentMap());
    MFCameraAttachment::ContainerType attachments;
    for(::osg::Camera::BufferAttachmentMap::const_iterator i = buffer_map.begin(); i != buffer_map.end(); ++i)
    {
        Link<CameraAttachment> a(new CameraAttachment);
        a->BufferComponent.setValue(i->first);
        a->InternalFormat.setValue(i->second._internalFormat);
        if(i->second._texture.valid())
        {
            Texture* texture = dynamic_cast<Texture*>(i->second._texture->getUserData());
            if(texture)
                a->Texture.setValue(texture);
        }
        if(i->second._image.valid())
        {
            Image* image = dynamic_cast<Image*>(i->second._image->getUserData());
            if(image)
                a->Image.setValue(image);
        }
        a->Level.setValue(i->second._level);
        a->Face.setValue(i->second._face);
        a->MIPMapGeneration.setValue(i->second._mipMapGeneration);
        a->MultisampleSamples.setValue(i->second._multisampleSamples);
        a->MultisampleColorSamples.setValue(i->second._multisampleColorSamples);
        attachments.push_back(a);
    }
    *(event.getValuePtr()) = attachments;
}

/* virtual */ void av::osg::Camera::setAttachmentCB(const av::osg::MFCameraAttachment::SetValueEvent& event)
{
    ::osg::Camera::BufferAttachmentMap& buffer_map(mOsgCamera->getBufferAttachmentMap());

    // Remove attachments
    typedef std::vector<::osg::Camera::BufferComponent> ComponentContainer;
    ComponentContainer components;
    for(::osg::Camera::BufferAttachmentMap::const_iterator i = buffer_map.begin(); i != buffer_map.end(); ++i)
    {
        components.push_back(i->first);
    }
    for(ComponentContainer::const_iterator i = components.begin(); i != components.end(); ++i)
    {
        mOsgCamera->detach(*i);
    }

    // Add given ones
    for(MFCameraAttachment::ContainerType::const_iterator i = event.getValue().begin(); i != event.getValue().end(); ++i)
    {
        ::osg::Camera::Attachment attachment;
        attachment._internalFormat = static_cast<GLenum>((*i)->InternalFormat.getValue());
        attachment._level = (*i)->Level.getValue();
        attachment._face = (*i)->Face.getValue();
        if((*i)->Texture.getValue().isValid())
        {
            attachment._texture = (*i)->Texture.getValue()->getOsgTexture();
        }
        if((*i)->Image.getValue().isValid())
        {
            attachment._image = (*i)->Image.getValue()->getOsgImage();
        }
        attachment._mipMapGeneration = (*i)->MIPMapGeneration.getValue();
        attachment._multisampleSamples = (*i)->MultisampleSamples.getValue();
        attachment._multisampleColorSamples = (*i)->MultisampleColorSamples.getValue();
        buffer_map[static_cast<::osg::Camera::BufferComponent>((*i)->BufferComponent.getValue())] = attachment;
    }
}
