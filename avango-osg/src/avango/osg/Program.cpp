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
#include <avango/osg/Program.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Program"));
}

AV_FC_DEFINE(av::osg::Program);

AV_FIELD_DEFINE(av::osg::SFProgram);
AV_FIELD_DEFINE(av::osg::MFProgram);

av::osg::Program::Program(::osg::Program* osgprogram) :
  StateAttribute(osgprogram),
  mOsgProgram(osgprogram)
{
  AV_FC_ADD_ADAPTOR_FIELD(ShaderList,
                            boost::bind(&Program::getShaderListCB, this, _1),
                            boost::bind(&Program::setShaderListCB, this, _1));
}

av::osg::Program::~Program()
{}

void
av::osg::Program::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::Program, true);

    SFProgram::initClass("av::osg::SFProgram", "av::Field");
    MFProgram::initClass("av::osg::MFProgram", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Program*
av::osg::Program::getOsgProgram() const
{
  return mOsgProgram;
}

/* virtual */ void
av::osg::Program::getShaderListCB(const av::osg::MFShader::GetValueEvent& event)
{
  av::osg::MFShader::ContainerType& container(*event.getValuePtr());
  container.clear();
  for (unsigned int i = 0; i != mOsgProgram->getNumShaders(); ++i)
  {
    container.push_back(av::osg::get_from_osg_object<av::osg::Shader>(mOsgProgram->getShader(i)));
  }
}

/* virtual */ void
av::osg::Program::setShaderListCB(const av::osg::MFShader::SetValueEvent& event)
{
  while (mOsgProgram->getNumShaders() > 0)
  {
    mOsgProgram->removeShader(mOsgProgram->getShader(0));
  }
  const av::osg::MFShader::ContainerType& container(event.getValue());
  for (av::osg::MFShader::ContainerType::const_iterator i = container.begin(); i != container.end(); ++i)
  {
    mOsgProgram->addShader((*i)->getOsgShader());
  }
}

void
av::osg::Program::touchFields()
{
  ShaderList.touch();
}
