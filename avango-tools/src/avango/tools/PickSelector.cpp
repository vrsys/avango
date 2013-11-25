#include <avango/tools/PickSelector.hpp>

#include <avango/gua/scenegraph/PickResult.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::PickSelector"));
}

AV_FC_DEFINE(av::tools::PickSelector);

AV_FIELD_DEFINE(av::tools::SFPickSelector);
AV_FIELD_DEFINE(av::tools::MFPickSelector);

av::tools::PickSelector::PickSelector():
  mLastPickTrigger(false)
{
  AV_FC_ADD_FIELD(PickTrigger, false);
  AV_FC_ADD_FIELD(TransitionOnly, true);
  AV_FC_ADD_FIELD(EveryFrame, false);
  AV_FC_ADD_FIELD(PickRay, nullptr);
  AV_FC_ADD_FIELD(SceneGraph, nullptr);
  AV_FC_ADD_FIELD(Options, 0);
  AV_FC_ADD_FIELD(SetCreator, true);
  AV_FC_ADD_FIELD(PickMask, "");
}

av::tools::PickSelector::~PickSelector()
{}

void
av::tools::PickSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::PickSelector, true);

    SFPickSelector::initClass("av::tools::SFPickSelector", "av::Field");
    MFPickSelector::initClass("av::tools::MFPickSelector", "av::Field");
  }
}

const av::tools::MFTargetHolder::ContainerType&
av::tools::PickSelector::pick() {
  mSelectedTargets.clear();

  if (SceneGraph.getValue().isValid()) {

    auto results(SceneGraph.getValue()->getGuaSceneGraph()->ray_test(*(
                 PickRay.getValue()->getGuaNode()),
                 static_cast< ::gua::PickResult::Options>(Options.getValue()),
                 PickMask.getValue()));

    for (auto result : results) {
      Link<TargetHolder> holder (new TargetHolder());
      holder->Target.setValue(new av::gua::PickResult(result));

      if (SetCreator.getValue())
        holder->Creator.setValue(this);

      mSelectedTargets.push_back(holder);

    }

  }

  return mSelectedTargets;
}

/* virtual */ void
av::tools::PickSelector::evaluate() {
  av::tools::Selector::evaluate();

  // try to pick if trigger changes to or stays true
  if (PickTrigger.getValue() && (!mLastPickTrigger || !TransitionOnly.getValue()))
  {
    pick();
    if (!mSelectedTargets.empty() || !SelectedTargets.isEmpty()) {
      SelectedTargets.setValue(mSelectedTargets);
    }

    if (!mLastPickTrigger)
    {
      mLastPickTrigger = true;
      if (!TransitionOnly.getValue() && EveryFrame.getValue()) {
        alwaysEvaluate(true);
      }
    }
  }
  else if (!PickTrigger.getValue() && mLastPickTrigger)
  {
    if (!SelectedTargets.isEmpty())
      SelectedTargets.clear();
    mLastPickTrigger = false;
    alwaysEvaluate(false);
  }
}
