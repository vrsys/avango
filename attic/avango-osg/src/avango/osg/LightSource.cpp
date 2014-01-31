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
#include <avango/osg/LightSource.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LightSource"));
}

AV_FC_DEFINE(av::osg::LightSource);

AV_FIELD_DEFINE(av::osg::SFLightSource);
AV_FIELD_DEFINE(av::osg::MFLightSource);

av::osg::LightSource::LightSource(::osg::LightSource* osglightsource) :
  Group(osglightsource),
  mOsgLightSource(osglightsource)
{
  AV_FC_ADD_ADAPTOR_FIELD(Light,
			  boost::bind(&LightSource::getLightCB, this, _1),
			  boost::bind(&LightSource::setLightCB, this, _1));
}

av::osg::LightSource::~LightSource()
{}

void
av::osg::LightSource::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::LightSource, true);

    SFLightSource::initClass("av::osg::SFLightSource", "av::Field");
    MFLightSource::initClass("av::osg::MFLightSource", "av::Field");
  }
}



::osg::LightSource*
av::osg::LightSource::getOsgLightSource() const
{
  return mOsgLightSource;
}

/* virtual */ void
av::osg::LightSource::getLightCB(const av::osg::SFLight::GetValueEvent& event)
{
  *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::Light>(mOsgLightSource->getLight());
}

/* virtual */ void
av::osg::LightSource::setLightCB(const av::osg::SFLight::SetValueEvent& event)
{
  mOsgLightSource->setLight(event.getValue()->getOsgLight());
  mOsgLightSource->setLocalStateSetModes(::osg::StateAttribute::ON); 

  //mOsgLightSource->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);


}






void
av::osg::LightSource::touchFields()
{
  Light.touch();
}
