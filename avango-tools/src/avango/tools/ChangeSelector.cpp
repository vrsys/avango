#include <avango/gua/tools/ChangeSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ChangeSelector"));
}

AV_FC_DEFINE(av::gua::ChangeSelector);

AV_FIELD_DEFINE(av::gua::SFChangeSelector);
AV_FIELD_DEFINE(av::gua::MFChangeSelector);

av::gua::ChangeSelector::ChangeSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(KeptTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(RemovedTargets, MFTargetHolder::ContainerType());
}

av::gua::ChangeSelector::~ChangeSelector()
{}

void
av::gua::ChangeSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::ChangeSelector, true);

    SFChangeSelector::initClass("av::gua::SFChangeSelector", "av::Field");
    MFChangeSelector::initClass("av::gua::MFChangeSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::ChangeSelector::evaluate()
{
  av::gua::Selector::evaluate();

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
