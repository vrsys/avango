// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/tools/Selector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::Selector"));
}

AV_FC_DEFINE_ABSTRACT(av::tools::Selector);

AV_FIELD_DEFINE(av::tools::SFSelector);
AV_FIELD_DEFINE(av::tools::MFSelector);

av::tools::Selector::Selector()
{
  AV_FC_ADD_FIELD(SelectedTargets, MFTargetHolder::ContainerType());
}

av::tools::Selector::~Selector()
{}

void
av::tools::Selector::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    av::tools::TargetHolder::initClass();

    AV_FC_INIT_ABSTRACT(av::FieldContainer, av::tools::Selector, true);

    SFSelector::initClass("av::tools::SFSelector", "av::Field");
    MFSelector::initClass("av::tools::MFSelector", "av::Field");
  }
}

av::Link<av::tools::TargetHolder>
av::tools::Selector::combine(const Link<TargetHolder> holder1, const Link<TargetHolder> holder2)
{
  Link<TargetHolder> holder;

  if (!holder2->keep())
    holder = holder1;
  else if (!holder1->keep())
    holder = holder2;
  else
  {
    holder = new TargetHolder;
    holder->Target.setValue(holder1->Target.getValue());
    holder->Creator.setValue(this);
    holder->ParentTargets.add1Value(holder1);
    holder->ParentTargets.add1Value(holder2);
  }

  return holder;
}
