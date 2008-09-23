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
#include <avango/osg/CullFace.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::CullFace"));
}

AV_FC_DEFINE(av::osg::CullFace);

AV_FIELD_DEFINE(av::osg::SFCullFace);
AV_FIELD_DEFINE(av::osg::MFCullFace);

av::osg::CullFace::CullFace(::osg::CullFace* osgcullface) :
  StateAttribute(osgcullface),
  mOsgCullFace(osgcullface)
{
  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                            boost::bind(&CullFace::getModeCB, this, _1),
                            boost::bind(&CullFace::setModeCB, this, _1));
}

av::osg::CullFace::~CullFace()
{}

void
av::osg::CullFace::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::StateAttribute::initClass();

    AV_FC_INIT(av::osg::StateAttribute, av::osg::CullFace, true);

    SFCullFace::initClass("av::osg::SFCullFace", "av::Field");
    MFCullFace::initClass("av::osg::MFCullFace", "av::Field");
  }
}

::osg::CullFace*
av::osg::CullFace::getOsgCullFace() const
{
  return mOsgCullFace;
}

/* virtual */ void
av::osg::CullFace::getModeCB(const av::SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgCullFace->getMode();
}

/* virtual */ void
av::osg::CullFace::setModeCB(const av::SFInt::SetValueEvent& event)
{
  mOsgCullFace->setMode((::osg::CullFace::Mode)(event.getValue()));
}
