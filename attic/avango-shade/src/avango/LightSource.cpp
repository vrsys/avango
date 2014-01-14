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

#include <avango/shade/LightSource.h>
#include <avango/shade/Shader.h>

#include <boost/bind.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::shade::LightSource"));
}

AV_FC_DEFINE(av::shade::LightSource);

AV_FIELD_DEFINE(av::shade::SFLightSource);
AV_FIELD_DEFINE(av::shade::MFLightSource);

av::shade::LightSource::LightSource(::osg::LightSource* osglightsource) :
  Node(osglightsource),
  mOsgLightSource(osglightsource),
  mGLLightPosition(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("av::shade::GLLightPosition")))
{
  if (!mOsgLightSource->getLight())
  {
    mOsgLightSource->setLight(new ::osg::Light);
  }

  AV_FC_ADD_ADAPTOR_FIELD(LightNum,
                          boost::bind(&LightSource::getLightNumCB, this, _1),
                          boost::bind(&LightSource::setLightNumCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Position,
                          boost::bind(&LightSource::getPositionCB, this, _1),
                          boost::bind(&LightSource::setPositionCB, this, _1));

  av::Link<av::shade::Shader> converter(dynamic_cast<av::shade::Shader*>(av::Type::createInstanceOfType("av::shade::Vec4ToVec3")));
  SFContainer* value_field(dynamic_cast<SFContainer*>(converter->getField("value")));
  assert(value_field);
  value_field->setValue(mGLLightPosition);
  AV_FC_ADD_FIELD(Shader, converter);

  // Initialize light index for SHADE gettable
  setLightNum(getLightNum());
}

/* virtual */
av::shade::LightSource::~LightSource()
{}

/* static */ void
av::shade::LightSource::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::shade::LightSource, true);

    SFLightSource::initClass("av::shade::SFLightSource", "av::Field");
    MFLightSource::initClass("av::shade::MFLightSource", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::LightSource*
av::shade::LightSource::getOsgLightSource() const
{
  return mOsgLightSource;
}

int av::shade::LightSource::getLightNum(void)
{
  ::osg::Light* light = mOsgLightSource->getLight();
  assert(light);

  return light->getLightNum();
}

void av::shade::LightSource::setLightNum(int num)
{
  ::osg::Light* light = mOsgLightSource->getLight();
  assert(light);

  SFInt* index_field(dynamic_cast<SFInt*>(mGLLightPosition->getField("index")));
  assert(index_field);

  light->setLightNum(num);
  index_field->setValue(num);
}

/* virtual */ void
av::shade::LightSource::getLightNumCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = getLightNum();
}

/* virtual */ void
av::shade::LightSource::setLightNumCB(const av::SFInt::SetValueEvent& event)
{
  setLightNum(event.getValue());
}

/* virtual */ void
av::shade::LightSource::getPositionCB(const av::osg::SFVec4::GetValueEvent& event)
{
  ::osg::Light* light = mOsgLightSource->getLight();
  if (!light)
    return;

  *(event.getValuePtr()) = light->getPosition();
}

/* virtual */ void
av::shade::LightSource::setPositionCB(const av::osg::SFVec4::SetValueEvent& event)
{
  ::osg::Light* light = mOsgLightSource->getLight();
  if (!light)
    return;

  light->setPosition(event.getValue());
}
