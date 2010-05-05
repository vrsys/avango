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

#include <avango/tools/ChangeSelector.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ChangeSelector"));
}

AV_FC_DEFINE(av::tools::ChangeSelector);

AV_FIELD_DEFINE(av::tools::SFChangeSelector);
AV_FIELD_DEFINE(av::tools::MFChangeSelector);

av::tools::ChangeSelector::ChangeSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(KeptTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(RemovedTargets, MFTargetHolder::ContainerType());
}

av::tools::ChangeSelector::~ChangeSelector()
{}

void
av::tools::ChangeSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::ChangeSelector, true);

    SFChangeSelector::initClass("av::tools::SFChangeSelector", "av::Field");
    MFChangeSelector::initClass("av::tools::MFChangeSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::ChangeSelector::evaluate()
{
  av::tools::Selector::evaluate();

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  MFTargetHolder::ContainerType added_targets, kept_targets, removed_targets;

  for (MFTargetHolder::ContainerType::const_iterator last_holder = mLastTargets.begin();
       last_holder != mLastTargets.end(); ++last_holder)
  {
    if (!hasTarget(targets, (*last_holder)->Target.getValue()))
      removed_targets.push_back(*last_holder);
  }

  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    (hasTarget(mLastTargets, target) ? kept_targets : added_targets).push_back(*holder);
  }

  if (!removed_targets.empty() || !RemovedTargets.isEmpty())
    RemovedTargets.setValue(removed_targets);
  if (!kept_targets.empty() || !KeptTargets.isEmpty())
    KeptTargets.setValue(kept_targets);
  if (!added_targets.empty() || !SelectedTargets.isEmpty())
    SelectedTargets.setValue(added_targets);

  mLastTargets = targets;
  alwaysEvaluate(!added_targets.empty() || !removed_targets.empty());
}
