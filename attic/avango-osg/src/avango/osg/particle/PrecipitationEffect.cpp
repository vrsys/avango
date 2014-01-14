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

#include <osg/Fog>
#include <osg/Version>
#include <avango/osg/particle/PrecipitationEffect.h>
#include <avango/osg/ObjectLink.h>

#include <avango/Logger.h>

#include <boost/bind.hpp>

#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::PrecipitationEffect"));
}

AV_FC_DEFINE(av::osg::particle::PrecipitationEffect);

AV_FIELD_DEFINE(av::osg::particle::SFPrecipitationEffect);
AV_FIELD_DEFINE(av::osg::particle::MFPrecipitationEffect);

av::osg::particle::PrecipitationEffect::PrecipitationEffect
  (::osgParticle::PrecipitationEffect* osgprecipitationeffect) :
  Node(osgprecipitationeffect),
  mOsgPrecipitationEffect(osgprecipitationeffect),
  mRain(0.5f),
  mSnow(0.0f)
{
  AV_FC_ADD_ADAPTOR_FIELD(Rain,
                            boost::bind(&PrecipitationEffect::getRainCB, this, _1),
                            boost::bind(&PrecipitationEffect::setRainCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Snow,
                            boost::bind(&PrecipitationEffect::getSnowCB, this, _1),
                            boost::bind(&PrecipitationEffect::setSnowCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Wind,
                            boost::bind(&PrecipitationEffect::getWindCB, this, _1),
                            boost::bind(&PrecipitationEffect::setWindCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Position,
                            boost::bind(&PrecipitationEffect::getPositionCB, this, _1),
                            boost::bind(&PrecipitationEffect::setPositionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(CellSize,
                            boost::bind(&PrecipitationEffect::getCellSizeCB, this, _1),
                            boost::bind(&PrecipitationEffect::setCellSizeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(ParticleSpeed,
                            boost::bind(&PrecipitationEffect::getParticleSpeedCB, this, _1),
                            boost::bind(&PrecipitationEffect::setParticleSpeedCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(ParticleSize,
                            boost::bind(&PrecipitationEffect::getParticleSizeCB, this, _1),
                            boost::bind(&PrecipitationEffect::setParticleSizeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(ParticleColor,
                            boost::bind(&PrecipitationEffect::getParticleColorCB, this, _1),
                            boost::bind(&PrecipitationEffect::setParticleColorCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(NearTransition,
                            boost::bind(&PrecipitationEffect::getNearTransitionCB, this, _1),
                            boost::bind(&PrecipitationEffect::setNearTransitionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(FarTransition,
                            boost::bind(&PrecipitationEffect::getNearTransitionCB, this, _1),
                            boost::bind(&PrecipitationEffect::setNearTransitionCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(MaximumParticleDensity,
                            boost::bind(&PrecipitationEffect::getMaximumParticleDensityCB, this, _1),
                            boost::bind(&PrecipitationEffect::setMaximumParticleDensityCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(UseFarLineSegments,
                            boost::bind(&PrecipitationEffect::getUseFarLineSegmentsCB, this, _1),
                            boost::bind(&PrecipitationEffect::setUseFarLineSegmentsCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Fog,
                            boost::bind(&PrecipitationEffect::getFogCB, this, _1),
                            boost::bind(&PrecipitationEffect::setFogCB, this, _1));
}

av::osg::particle::PrecipitationEffect::~PrecipitationEffect()
{}

void
av::osg::particle::PrecipitationEffect::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::osg::particle::PrecipitationEffect, true);

    SFPrecipitationEffect::initClass("av::osg::particle::SFPrecipitationEffect", "av::Field");
    MFPrecipitationEffect::initClass("av::osg::particle::MFPrecipitationEffect", "av::Field");
  }
}

::osgParticle::PrecipitationEffect*
av::osg::particle::PrecipitationEffect::getOsgPrecipitationEffect() const
{
  return mOsgPrecipitationEffect;
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getRainCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mRain;
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setRainCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->rain(event.getValue());
  mRain = event.getValue();
  touchFields();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getSnowCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mSnow;
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setSnowCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->snow(event.getValue());
  mSnow = event.getValue();
  touchFields();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getWindCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getWind();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setWindCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setWind(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getPositionCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getPosition();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setPositionCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setPosition(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getCellSizeCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getCellSize();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setCellSizeCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setCellSize(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getParticleSpeedCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getParticleSpeed();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setParticleSpeedCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setParticleSpeed(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getParticleSizeCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getParticleSize();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setParticleSizeCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setParticleSize(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getParticleColorCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getParticleColor();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setParticleColorCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setParticleColor(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getNearTransitionCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getNearTransition();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setNearTransitionCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setNearTransition(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getFarTransitionCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getFarTransition();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setFarTransitionCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setFarTransition(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getMaximumParticleDensityCB(const av::SFFloat::GetValueEvent& event)
{
#if OSG_VERSION_MAJOR == 3
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getMaximumParticleDensity();
#elif OSG_VERSION_MAJOR == 2
  *(event.getValuePtr()) = mOsgPrecipitationEffect->setMaximumParticleDensity();
#endif
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setMaximumParticleDensityCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setMaximumParticleDensity(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getUseFarLineSegmentsCB(const av::SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgPrecipitationEffect->getUseFarLineSegments();
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setUseFarLineSegmentsCB(const av::SFBool::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setUseFarLineSegments(event.getValue());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::getFogCB(const av::osg::SFFog::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Fog>(mOsgPrecipitationEffect->getFog());
}

/* virtual */ void
av::osg::particle::PrecipitationEffect::setFogCB(const av::osg::SFFog::SetValueEvent& event)
{
  mOsgPrecipitationEffect->setFog(event.getValue()->getOsgFog());
}

void
av::osg::particle::PrecipitationEffect::touchFields()
{
  Wind.touch();
  Position.touch();
  CellSize.touch();
  ParticleSpeed.touch();
  ParticleSize.touch();
  NearTransition.touch();
  FarTransition.touch();
  MaximumParticleDensity.touch();
  UseFarLineSegments.touch();
  av::osg::Fog *fog = av::osg::get_from_osg_object<av::osg::Fog>(mOsgPrecipitationEffect->getFog());
  if (fog != 0)
    fog->touch();
}
