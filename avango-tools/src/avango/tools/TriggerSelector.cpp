#include <avango/gua/tools/TriggerSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TriggerSelector"));
}

AV_FC_DEFINE(av::gua::TriggerSelector);

AV_FIELD_DEFINE(av::gua::SFTriggerSelector);
AV_FIELD_DEFINE(av::gua::MFTriggerSelector);

av::gua::TriggerSelector::TriggerSelector():
  mLastTrigger(false)
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Trigger, false);
  AV_FC_ADD_FIELD(TransitionOnly, false);
}

av::gua::TriggerSelector::~TriggerSelector()
{}

void
av::gua::TriggerSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::TriggerSelector, true);

    SFTriggerSelector::initClass("av::gua::SFTriggerSelector", "av::Field");
    MFTriggerSelector::initClass("av::gua::MFTriggerSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::TriggerSelector::evaluate()
{
  av::gua::Selector::evaluate();

  if (Trigger.getValue() && (!mLastTrigger || !TransitionOnly.getValue()))
  {
    if (!SelectedTargets.isEmpty() || !Targets.isEmpty())
      SelectedTargets.setValue(Targets.getValue());
    mLastTrigger = true;
  }
  else
  {
    if (!SelectedTargets.isEmpty())
      SelectedTargets.clear();
    mLastTrigger = false;
  }
}
