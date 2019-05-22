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

#include <avango/tools/NodePathTargetHolder.h>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::tools::NodePathTargetHolder"));
}

AV_FC_DEFINE(av::tools::NodePathTargetHolder);

AV_FIELD_DEFINE(av::tools::SFNodePathTargetHolder);
AV_FIELD_DEFINE(av::tools::MFNodePathTargetHolder);

av::tools::NodePathTargetHolder::NodePathTargetHolder()
{
    AV_FC_ADD_FIELD(NodePath, av::osg::MFNode::ContainerType());

    Keep.setValue(true);
}

av::tools::NodePathTargetHolder::~NodePathTargetHolder() {}

void av::tools::NodePathTargetHolder::initClass()
{
    if(!isTypeInitialized())
    {
        av::tools::TargetHolder::initClass();

        AV_FC_INIT(av::tools::TargetHolder, av::tools::NodePathTargetHolder, true);

        SFNodePathTargetHolder::initClass("av::tools::SFNodePathTargetHolder", "av::Field");
        MFNodePathTargetHolder::initClass("av::tools::MFNodePathTargetHolder", "av::Field");
    }
}
