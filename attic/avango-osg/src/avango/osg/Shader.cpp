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
#include <avango/osg/Shader.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Shader"));
}

AV_FC_DEFINE(av::osg::Shader);

AV_FIELD_DEFINE(av::osg::SFShader);
AV_FIELD_DEFINE(av::osg::MFShader);

av::osg::Shader::Shader(::osg::Shader* osgshader) : Object(osgshader), mOsgShader(osgshader)
{
    AV_FC_ADD_ADAPTOR_FIELD(Type, boost::bind(&Shader::getTypeCB, this, _1), boost::bind(&Shader::setTypeCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(ShaderSource, boost::bind(&Shader::getShaderSourceCB, this, _1), boost::bind(&Shader::setShaderSourceCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(FileName, boost::bind(&Shader::getFileNameCB, this, _1), boost::bind(&Shader::setFileNameCB, this, _1));
}

av::osg::Shader::~Shader() {}

void av::osg::Shader::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Object::initClass();

        AV_FC_INIT(av::osg::Object, av::osg::Shader, true);

        SFShader::initClass("av::osg::SFShader", "av::Field");
        MFShader::initClass("av::osg::MFShader", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

::osg::Shader* av::osg::Shader::getOsgShader() const { return mOsgShader; }

/* virtual */ void av::osg::Shader::getTypeCB(const av::SFInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgShader->getType(); }

/* virtual */ void av::osg::Shader::setTypeCB(const av::SFInt::SetValueEvent& event) { mOsgShader->setType(static_cast<::osg::Shader::Type>(event.getValue())); }

/* virtual */ void av::osg::Shader::getShaderSourceCB(const av::SFString::GetValueEvent& event) { *(event.getValuePtr()) = mOsgShader->getShaderSource(); }

/* virtual */ void av::osg::Shader::setShaderSourceCB(const av::SFString::SetValueEvent& event) { mOsgShader->setShaderSource(event.getValue()); }

/* virtual */ void av::osg::Shader::getFileNameCB(const av::SFString::GetValueEvent& event) { *(event.getValuePtr()) = mOsgShader->getFileName(); }

/* virtual */ void av::osg::Shader::setFileNameCB(const av::SFString::SetValueEvent& event) { mOsgShader->loadShaderSourceFromFile(event.getValue()); }

void av::osg::Shader::touchFields()
{
    Type.touch();
    ShaderSource.touch();
    FileName.touch();
}
