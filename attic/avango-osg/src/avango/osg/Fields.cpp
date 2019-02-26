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

#include <avango/osg/Fields.h>

#include <avango/Logger.h>
#include <avango/MultiField.h>
#include <avango/SingleField.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Fields"));
}

AV_FIELD_DEFINE(av::osg::SFMatrixf);
AV_FIELD_DEFINE(av::osg::MFMatrixf);

AV_FIELD_DEFINE(av::osg::SFMatrixd);
AV_FIELD_DEFINE(av::osg::MFMatrixd);

AV_FIELD_DEFINE(av::osg::SFVec2f);
AV_FIELD_DEFINE(av::osg::MFVec2f);

AV_FIELD_DEFINE(av::osg::SFVec2d);
AV_FIELD_DEFINE(av::osg::MFVec2d);

AV_FIELD_DEFINE(av::osg::SFVec3f);
AV_FIELD_DEFINE(av::osg::MFVec3f);

AV_FIELD_DEFINE(av::osg::SFVec3d);
AV_FIELD_DEFINE(av::osg::MFVec3d);

AV_FIELD_DEFINE(av::osg::SFVec4f);
AV_FIELD_DEFINE(av::osg::MFVec4f);

AV_FIELD_DEFINE(av::osg::SFVec4d);
AV_FIELD_DEFINE(av::osg::MFVec4d);

AV_FIELD_DEFINE(av::osg::SFQuat);
AV_FIELD_DEFINE(av::osg::MFQuat);

void av::osg::initFields()
{
    LOG_TRACE(logger) << "initFields(): initializing osg related fields";

    SFMatrixf::initClass("av::osg::SFMatrixf", "av::Field");
    MFMatrixf::initClass("av::osg::MFMatrixf", "av::Field");

    SFMatrixd::initClass("av::osg::SFMatrixd", "av::Field");
    MFMatrixd::initClass("av::osg::MFMatrixd", "av::Field");

    SFVec2f::initClass("av::osg::SFVec2f", "av::Field");
    MFVec2f::initClass("av::osg::MFVec2f", "av::Field");

    SFVec2d::initClass("av::osg::SFVec2d", "av::Field");
    MFVec2d::initClass("av::osg::MFVec2d", "av::Field");

    SFVec3f::initClass("av::osg::SFVec3f", "av::Field");
    MFVec3f::initClass("av::osg::MFVec3f", "av::Field");

    SFVec3d::initClass("av::osg::SFVec3d", "av::Field");
    MFVec3d::initClass("av::osg::MFVec3d", "av::Field");

    SFVec4f::initClass("av::osg::SFVec4f", "av::Field");
    MFVec4f::initClass("av::osg::MFVec4f", "av::Field");

    SFVec4d::initClass("av::osg::SFVec4d", "av::Field");
    MFVec4d::initClass("av::osg::MFVec4d", "av::Field");

    SFQuat::initClass("av::osg::SFQuat", "av::Field");
    MFQuat::initClass("av::osg::MFQuat", "av::Field");
}
