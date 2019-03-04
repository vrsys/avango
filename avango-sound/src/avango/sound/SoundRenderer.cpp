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

#include "avango/sound/SoundRenderer.h"

#include <functional>
#include <avango/gua/Fields.hpp>

AV_FC_DEFINE_ABSTRACT(av::sound::SoundRenderer)

AV_FIELD_DEFINE(av::sound::SFSoundRenderer);
AV_FIELD_DEFINE(av::sound::MFSoundRenderer);

av::sound::SoundRenderer::SoundRenderer()
{
    AV_FC_ADD_ADAPTOR_FIELD(ListenerPosition, std::bind(&SoundRenderer::getListenerPosCB, this, std::placeholders::_1), std::bind(&SoundRenderer::setListenerPosCB, this, std::placeholders::_1));
    AV_FC_ADD_ADAPTOR_FIELD(ListenerVelocity, std::bind(&SoundRenderer::getListenerVeloCB, this, std::placeholders::_1), std::bind(&SoundRenderer::setListenerVeloCB, this, std::placeholders::_1));
}

av::sound::SoundRenderer::~SoundRenderer() {}

/*static*/ void av::sound::SoundRenderer::initClass()
{
    if(!isTypeInitialized())
    {
        FieldContainer::initClass();
        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::sound::SoundRenderer, false);

        SFSoundRenderer::initClass("av::sound::SFSoundRenderer", "av::Field");
        MFSoundRenderer::initClass("av::sound::MFSoundRenderer", "av::Field");
    }
}

boost::shared_ptr<av::sound::SoundSource::LocalSource> av::sound::SoundRenderer::createLocalSource()
{
    AV_ASSERT(false);
    return boost::shared_ptr<av::sound::SoundSource::LocalSource>();
}

void av::sound::SoundRenderer::getListenerPosCB(const av::gua::SFMatrix::GetValueEvent& event)
{
    av::gua::SFMatrix::ValueType& matrix(*event.getValuePtr());
    matrix = getListenerPosition();
}

void av::sound::SoundRenderer::setListenerPosCB(const av::gua::SFMatrix::SetValueEvent& event)
{
    const av::gua::SFMatrix::ValueType& matrix(event.getValue());
    setListenerPosition(matrix);
}

void av::sound::SoundRenderer::getListenerVeloCB(const av::gua::SFVec3::GetValueEvent& event)
{
    av::gua::SFVec3::ValueType& vector(*event.getValuePtr());
    vector = getListenerVelocity();
}

void av::sound::SoundRenderer::setListenerVeloCB(const av::gua::SFVec3::SetValueEvent& event)
{
    const av::gua::SFVec3::ValueType& vector(event.getValue());
    setListenerVelocity(vector);
}

void av::sound::SoundRenderer::updateSoundSourcePosition(av::Link<av::sound::SoundSource> source, const ::gua::math::mat4& transform)
{
    const unsigned int num_local_sources = source->numLocalSources();

    unsigned int local_source_index;
    for(local_source_index = 0; local_source_index < num_local_sources; ++local_source_index)
    {
        boost::shared_ptr<SoundSource::LocalSource> local_source = source->getLocalSource(local_source_index);
        if(this == local_source->getRenderer())
        {
            local_source->setWorldTransform(transform);
            break; // every renderer has only one local source per sound source
        }
    }
    if(local_source_index == num_local_sources) // no local source found, create one
    {
        boost::shared_ptr<SoundSource::LocalSource> local_source = createLocalSource();
        source->addLocalSource(local_source);
        local_source->setWorldTransform(transform);
    }
}
