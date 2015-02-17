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

#include <avango/tools/Init.hpp>

#include <avango/Logger.h>

#include <avango/tools/ChangeSelector.hpp>
#include <avango/tools/InjectSelector.hpp>
#include <avango/tools/ObjectSelector.hpp>
#include <avango/tools/NameSelector.hpp>
#include <avango/tools/FieldSelector.hpp>
#include <avango/tools/Selector.hpp>
#include <avango/tools/SingleSelector.hpp>
#include <avango/tools/TargetHolder.hpp>
#include <avango/tools/Tool.hpp>
#include <avango/tools/TriggerSelector.hpp>
#include <avango/tools/TypeSelector.hpp>
#include <avango/tools/UnionSelector.hpp>

//#if defined(AVANGO_TOOLS_GUA_SUPPORT)
#include <avango/tools/DragTool.hpp>
//#include <avango/tools/IntersectionTargetHolder.hpp>
//#include <avango/tools/NodePathTargetHolder.hpp>
#include <avango/tools/ProximitySelector.hpp>
#include <avango/tools/ScaledGrabTool.hpp>
//#endif

namespace
{
  av::Logger& logger(av::getLogger("av::tools::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::tools::Init);

/* static */ void
av::tools::Init::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::ChangeSelector::initClass();
    av::tools::InjectSelector::initClass();
    av::tools::ObjectSelector::initClass();
    av::tools::NameSelector::initClass();
    av::tools::FieldSelector::initClass();
    av::tools::Selector::initClass();
    av::tools::SingleSelector::initClass();
    av::tools::TargetHolder::initClass();
    av::tools::Tool::initClass();
    av::tools::TriggerSelector::initClass();
    av::tools::TypeSelector::initClass();
    av::tools::UnionSelector::initClass();

//#if defined(AVANGO_TOOLS_GUA_SUPPORT)
    av::tools::DragTool::initClass();
    //av::tools::IntersectionTargetHolder::initClass();
    //av::tools::NodePathTargetHolder::initClass();
    av::tools::ProximitySelector::initClass();
    av::tools::ScaledGrabTool::initClass();
//#endif

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::tools::Init", true);
  }
}
