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

#include <boost/bind.hpp>
#include <avango/osg/Light.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Light"));
}

AV_FC_DEFINE(av::osg::Light);

AV_FIELD_DEFINE(av::osg::SFLight);
AV_FIELD_DEFINE(av::osg::MFLight);

av::osg::Light::Light(::osg::Light* osglight) :
  StateAttribute(osglight),
  mOsgLight(osglight)
{
  AV_FC_ADD_ADAPTOR_FIELD(LightNum,
                            boost::bind(&Light::getLightNumCB, this, _1),
                            boost::bind(&Light::setLightNumCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Ambient,
                            boost::bind(&Light::getAmbientCB, this, _1),
                            boost::bind(&Light::setAmbientCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Diffuse,
                            boost::bind(&Light::getDiffuseCB, this, _1),
                            boost::bind(&Light::setDiffuseCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Specular,
                            boost::bind(&Light::getSpecularCB, this, _1),
                            boost::bind(&Light::setSpecularCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Position,
                            boost::bind(&Light::getPositionCB, this, _1),
                            boost::bind(&Light::setPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Direction,
                            boost::bind(&Light::getDirectionCB, this, _1),
                            boost::bind(&Light::setDirectionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ConstantAttenuation,
                            boost::bind(&Light::getConstantAttenuationCB, this, _1),
                            boost::bind(&Light::setConstantAttenuationCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LinearAttenuation,
                            boost::bind(&Light::getLinearAttenuationCB, this, _1),
                            boost::bind(&Light::setLinearAttenuationCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(QuadraticAttenuation,
                            boost::bind(&Light::getQuadraticAttenuationCB, this, _1),
                            boost::bind(&Light::setQuadraticAttenuationCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SpotExponent,
                            boost::bind(&Light::getSpotExponentCB, this, _1),
                            boost::bind(&Light::setSpotExponentCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SpotCutoff,
                            boost::bind(&Light::getSpotCutoffCB, this, _1),
                            boost::bind(&Light::setSpotCutoffCB, this, _1));



}

av::osg::Light::~Light()
{}

void
av::osg::Light::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::Light, true);

    SFLight::initClass("av::osg::SFLight", "av::Field");
    MFLight::initClass("av::osg::MFLight", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Light*
av::osg::Light::getOsgLight() const
{
  return mOsgLight;
}

/* virtual */ void
av::osg::Light::getLightNumCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getLightNum();
}

/* virtual */ void
av::osg::Light::setLightNumCB(const av::SFInt::SetValueEvent& event)
{
  mOsgLight->setLightNum(event.getValue());
}




/* virtual */ void
av::osg::Light::getAmbientCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getAmbient();
}

/* virtual */ void
av::osg::Light::setAmbientCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgLight->setAmbient(event.getValue());
}


/* virtual */ void
av::osg::Light::getDiffuseCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getDiffuse();
}

/* virtual */ void
av::osg::Light::setDiffuseCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgLight->setDiffuse(event.getValue());
}


/* virtual */ void
av::osg::Light::getSpecularCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getSpecular();
}

/* virtual */ void
av::osg::Light::setSpecularCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgLight->setSpecular(event.getValue());
}


/* virtual */ void
av::osg::Light::getPositionCB(const av::osg::SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getPosition();
}

/* virtual */ void
av::osg::Light::setPositionCB(const av::osg::SFVec4::SetValueEvent& event)
{
  mOsgLight->setPosition(event.getValue());
}

/* virtual */ void
av::osg::Light::getDirectionCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getDirection();
}

/* virtual */ void
av::osg::Light::setDirectionCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgLight->setDirection(event.getValue());
}


/* virtual */ void
av::osg::Light::getConstantAttenuationCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getConstantAttenuation();
}

/* virtual */ void
av::osg::Light::setConstantAttenuationCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLight->setConstantAttenuation(event.getValue());
}


/* virtual */ void
av::osg::Light::getLinearAttenuationCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getLinearAttenuation();
}

/* virtual */ void
av::osg::Light::setLinearAttenuationCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLight->setLinearAttenuation(event.getValue());
}

/* virtual */ void
av::osg::Light::getQuadraticAttenuationCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getQuadraticAttenuation();
}

/* virtual */ void
av::osg::Light::setQuadraticAttenuationCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLight->setQuadraticAttenuation(event.getValue());
}

/* virtual */ void
av::osg::Light::getSpotExponentCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getSpotExponent();
}

/* virtual */ void
av::osg::Light::setSpotExponentCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLight->setSpotExponent(event.getValue());
}

/* virtual */ void
av::osg::Light::getSpotCutoffCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgLight->getSpotCutoff();
}

/* virtual */ void
av::osg::Light::setSpotCutoffCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgLight->setSpotCutoff(event.getValue());
}



void
av::osg::Light::touchFields()
{
  LightNum.touch();
  Ambient.touch();
  Diffuse.touch();
  Specular.touch();
  Position.touch();
  Direction.touch();
  ConstantAttenuation.touch();
  LinearAttenuation.touch();
  QuadraticAttenuation.touch();
  SpotExponent.touch();
  SpotCutoff.touch();
}
