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
#include <avango/osg/Uniform.h>
#include <avango/Logger.h>
#include <typeinfo>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Uniform"));


  // returns 0 for int and 1 for float
  int isFloat(::osg::Uniform::Type t)
  {
    switch(::osg::Uniform::getGlApiType(t))
    {
      case ::osg::Uniform::INT:
      case ::osg::Uniform::INT_VEC2:
      case ::osg::Uniform::INT_VEC3:
      case ::osg::Uniform::INT_VEC4:
        return 0;
      default:
        return 1;
    }
  }

}

AV_FC_DEFINE(av::osg::Uniform);

AV_FIELD_DEFINE(av::osg::SFUniform);
AV_FIELD_DEFINE(av::osg::MFUniform);

av::osg::Uniform::Uniform(::osg::Uniform* osguniform) :
  Object(osguniform),
  mOsgUniform(osguniform)
{
  AV_FC_ADD_ADAPTOR_FIELD(Type,
                            boost::bind(&Uniform::getTypeCB, this, _1),
                            boost::bind(&Uniform::setTypeCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(Values,
                            boost::bind(&Uniform::getValuesCB, this, _1),
                            boost::bind(&Uniform::setValuesCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(UniformName,
                            boost::bind(&Uniform::getUniformNameCB, this, _1),
                            boost::bind(&Uniform::setUniformNameCB, this, _1));
}

av::osg::Uniform::~Uniform()
{}

void
av::osg::Uniform::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::Uniform, true);

    SFUniform::initClass("av::osg::SFUniform", "av::Field");
    MFUniform::initClass("av::osg::MFUniform", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Uniform*
av::osg::Uniform::getOsgUniform() const
{
  return mOsgUniform;
}

/* virtual */ void
av::osg::Uniform::getTypeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgUniform->getType();
}

/* virtual */ void
av::osg::Uniform::setTypeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgUniform->setType(static_cast< ::osg::Uniform::Type>(event.getValue()));
}

/* virtual */ void
av::osg::Uniform::getUniformNameCB(const av::SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgUniform->getName();
}

/* virtual */ void
av::osg::Uniform::setUniformNameCB(const av::SFString::SetValueEvent& event)
{
  mOsgUniform->setName(event.getValue());
}



/* virtual */ void
av::osg::Uniform::getValuesCB(const av::MFFloat::GetValueEvent& event)
{
  av::MFFloat::ContainerType& container(*event.getValuePtr());
  container.clear();
  unsigned int size = mOsgUniform->getTypeNumComponents(mOsgUniform->getType());
  if (isFloat(mOsgUniform->getType())) {
    ::osg::FloatArray* fa = mOsgUniform->getFloatArray();
    for (unsigned int i = 0; i < size; ++i) {
      container.push_back((*fa)[i]);
    }
  } else {
    ::osg::IntArray* ia = mOsgUniform->getIntArray();
    for (unsigned int i = 0; i < size; ++i) {
      container.push_back((float)(*ia)[i]);
    }
  }
}

/* virtual */ void
av::osg::Uniform::setValuesCB(const av::MFFloat::SetValueEvent& event)
{
  const av::MFFloat::ContainerType& container(event.getValue());
  mOsgUniform->setNumElements(1);

  unsigned int k=0;
  if (isFloat(mOsgUniform->getType())) {
    ::osg::FloatArray* fa = mOsgUniform->getFloatArray();
    for (av::MFFloat::ContainerType::const_iterator i = container.begin(); i != container.end(); ++i,++k) {
      (*fa)[k] = (*i);
    }
  } else {
    ::osg::IntArray* ia = mOsgUniform->getIntArray();
    for (av::MFFloat::ContainerType::const_iterator i = container.begin(); i != container.end(); ++i,++k) {
      (*ia)[k] = (int)(*i);
    }
  }
  mOsgUniform->dirty();
}

void
av::osg::Uniform::touchFields()
{
  UniformName.touch();
  Type.touch();
  Values.touch();
}



