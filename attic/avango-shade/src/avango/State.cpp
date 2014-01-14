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

#include "../include/avango/shade/State.h"
#include "../include/avango/shade/Shader.h"
#include "../include/avango/shade/Init.h"
#include "../../core/include/shade/Program.h"
#include "../../core/include/shade/osg/Wrapper.h"
#include <avango/Logger.h>
#include <osg/Node>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::shade::State"));
}

AV_FC_DEFINE(av::shade::State);

AV_FIELD_DEFINE(av::shade::SFDataVariance);
AV_FIELD_DEFINE(av::shade::SFState);
AV_FIELD_DEFINE(av::shade::MFState);

av::shade::State::State() :
  mOsgStateSet(new ::osg::StateSet),
  mState(new ::shade::osg::Wrapper(mOsgStateSet.get()))
{
  AV_FC_ADD_FIELD(Shader, 0);
  AV_FC_ADD_ADAPTOR_FIELD(DataVariance,
                          boost::bind(&State::getDataVarianceCB, this, _1),
                          boost::bind(&State::setDataVarianceCB, this, _1));
}

av::shade::State::~State()
{}

void
av::shade::State::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::shade::State, true);

    SFState::initClass("av::shade::SFState", "av::Field");
    MFState::initClass("av::shade::MFState", "av::Field");
  }
}

void
av::shade::State::applyState(::osg::Node* node)
{
  node->setStateSet(mOsgStateSet.get());
}

/* virtual */ void
av::shade::State::getDataVarianceCB(const SFDataVariance::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgStateSet->getDataVariance();
}

/* virtual */ void
av::shade::State::setDataVarianceCB(const SFDataVariance::SetValueEvent& event)
{
  mOsgStateSet->setDataVariance(event.getValue());
}

/* virtual */ void
av::shade::State::fieldHasChanged(const av::Field& field)
{
  av::FieldContainer::fieldHasChanged(field);

  if (&field == &Shader)
    mProgram.reset();
}

void
av::shade::State::update(void)
{
  av::shade::Shader* shader(dynamic_cast<av::shade::Shader*>(get_pointer(Shader.getValue())));
  if (!shader)
    return;

  if (!mProgram)
  {
    mProgram = boost::shared_ptr< ::shade::Program >(new ::shade::Program(shader->getShader(), mState));
    Init::initProgram(mProgram);
  }

  if (mProgram->requires_compilation())
    mProgram->compile();

  mState->make_current();

  if (mProgram->requires_upload())
    mProgram->upload();
}
