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

#include <avango/tools/InjectSelector.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::InjectSelector"));
}

AV_FC_DEFINE(av::tools::InjectSelector);

AV_FIELD_DEFINE(av::tools::SFInjectSelector);
AV_FIELD_DEFINE(av::tools::MFInjectSelector);

av::tools::InjectSelector::InjectSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(InjectTargets, MFTargetHolder::ContainerType());
}

av::tools::InjectSelector::~InjectSelector()
{}

void
av::tools::InjectSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::InjectSelector, true);

    SFInjectSelector::initClass("av::tools::SFInjectSelector", "av::Field");
    MFInjectSelector::initClass("av::tools::MFInjectSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::InjectSelector::evaluate()
{
  av::tools::Selector::evaluate();

  MFTargetHolder::ContainerType selected_targets = Targets.getValue();
  const MFTargetHolder::ContainerType &inject_targets = InjectTargets.getValue();

  for (MFTargetHolder::ContainerType::iterator holder = selected_targets.begin();
       holder != selected_targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    MFTargetHolder::ContainerType::const_iterator inject_holder = find(inject_targets, target);
    if (inject_holder != inject_targets.end())
    {
      if ((*holder)->keep())
      {
        Link<TargetHolder> combined_holder = new TargetHolder;
        combined_holder->Target.setValue(target);
        combined_holder->Creator.setValue(this);
        combined_holder->ParentTargets.add1Value(*holder);
        combined_holder->ParentTargets.add1Value(*inject_holder);
        *holder = combined_holder;
      }
      else
        *holder = *inject_holder;
    }
  }

  if (!selected_targets.empty() || !SelectedTargets.isEmpty())
    SelectedTargets.setValue(selected_targets);
}
