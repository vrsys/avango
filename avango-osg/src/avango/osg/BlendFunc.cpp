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
#include <avango/osg/BlendFunc.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::BlendFunc"));
}

AV_FC_DEFINE(av::osg::BlendFunc);

AV_FIELD_DEFINE(av::osg::SFBlendFunc);
AV_FIELD_DEFINE(av::osg::MFBlendFunc);

av::osg::BlendFunc::BlendFunc(::osg::BlendFunc* osgblendfunc) :
  StateAttribute(osgblendfunc),
  mOsgBlendFunc(osgblendfunc)
{
  AV_FC_ADD_ADAPTOR_FIELD(SourceRGB,
                            boost::bind(&BlendFunc::getSourceRGBCB, this, _1),
                            boost::bind(&BlendFunc::setSourceRGBCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(SourceAlpha,
                            boost::bind(&BlendFunc::getSourceAlphaCB, this, _1),
                            boost::bind(&BlendFunc::setSourceAlphaCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(DestinationRGB,
                            boost::bind(&BlendFunc::getDestinationRGBCB, this, _1),
                            boost::bind(&BlendFunc::setDestinationRGBCB, this, _1));
  AV_FC_ADD_ADAPTOR_FIELD(DestinationAlpha,
                            boost::bind(&BlendFunc::getDestinationAlphaCB, this, _1),
                            boost::bind(&BlendFunc::setDestinationAlphaCB, this, _1));
}

av::osg::BlendFunc::~BlendFunc()
{}

void
av::osg::BlendFunc::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::BlendFunc, true);

    SFBlendFunc::initClass("av::osg::SFBlendFunc", "av::Field");
    MFBlendFunc::initClass("av::osg::MFBlendFunc", "av::Field");
  }
}

::osg::BlendFunc*
av::osg::BlendFunc::getOsgBlendFunc() const
{
  return mOsgBlendFunc;
}

/* virtual */ void
av::osg::BlendFunc::getSourceRGBCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgBlendFunc->getSourceRGB();
}

/* virtual */ void
av::osg::BlendFunc::setSourceRGBCB(const av::SFInt::SetValueEvent& event)
{
  mOsgBlendFunc->setSourceRGB((GLenum)(event.getValue()));
}

/* virtual */ void
av::osg::BlendFunc::getSourceAlphaCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgBlendFunc->getSourceAlpha();
}

/* virtual */ void
av::osg::BlendFunc::setSourceAlphaCB(const av::SFInt::SetValueEvent& event)
{
  mOsgBlendFunc->setSourceAlpha((GLenum)(event.getValue()));
}

/* virtual */ void
av::osg::BlendFunc::getDestinationRGBCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgBlendFunc->getDestinationRGB();
}

/* virtual */ void
av::osg::BlendFunc::setDestinationRGBCB(const av::SFInt::SetValueEvent& event)
{
  mOsgBlendFunc->setDestinationRGB((GLenum)(event.getValue()));
}

/* virtual */ void
av::osg::BlendFunc::getDestinationAlphaCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgBlendFunc->getDestinationAlpha();
}

/* virtual */ void
av::osg::BlendFunc::setDestinationAlphaCB(const av::SFInt::SetValueEvent& event)
{
  mOsgBlendFunc->setDestinationAlpha((GLenum)(event.getValue()));
}

