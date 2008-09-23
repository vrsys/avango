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

#include <avango/tools/Init.h>

#include <avango/Logger.h>

#include <avango/tools/ChangeSelector.h>
#include <avango/tools/InjectSelector.h>
#include <avango/tools/ObjectSelector.h>
#include <avango/tools/NameSelector.h>
#include <avango/tools/Selector.h>
#include <avango/tools/SingleSelector.h>
#include <avango/tools/TargetHolder.h>
#include <avango/tools/Tool.h>
#include <avango/tools/TriggerSelector.h>
#include <avango/tools/TypeSelector.h>
#include <avango/tools/UnionSelector.h>

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
#include <avango/tools/DragTool.h>
#include <avango/tools/IntersectionTargetHolder.h>
#include <avango/tools/NodePathTargetHolder.h>
#include <avango/tools/PickSelector.h>
#include <avango/tools/ProximitySelector.h>
#include <avango/tools/ScaledGrabTool.h>
#endif


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
    av::tools::Selector::initClass();
    av::tools::SingleSelector::initClass();
    av::tools::TargetHolder::initClass();
    av::tools::Tool::initClass();
    av::tools::TriggerSelector::initClass();
    av::tools::TypeSelector::initClass();
    av::tools::UnionSelector::initClass();

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
    av::tools::DragTool::initClass();
    av::tools::IntersectionTargetHolder::initClass();
    av::tools::NodePathTargetHolder::initClass();
    av::tools::PickSelector::initClass();
    av::tools::ProximitySelector::initClass();
    av::tools::ScaledGrabTool::initClass();
#endif

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::tools::Init", true);
  }
}
