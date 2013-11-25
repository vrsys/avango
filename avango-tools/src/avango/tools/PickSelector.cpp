#include <avango/gua/tools/PickSelector.hpp>

#include <avango/gua/scenegraph/PickResult.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::PickSelector"));
}

AV_FC_DEFINE(av::gua::PickSelector);

AV_FIELD_DEFINE(av::gua::SFPickSelector);
AV_FIELD_DEFINE(av::gua::MFPickSelector);

av::gua::PickSelector::PickSelector():
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

av::gua::PickSelector::~PickSelector()
{}

void
av::gua::PickSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::PickSelector, true);

    SFPickSelector::initClass("av::gua::SFPickSelector", "av::Field");
    MFPickSelector::initClass("av::gua::MFPickSelector", "av::Field");
  }
}

const av::gua::MFTargetHolder::ContainerType&
av::gua::PickSelector::pick() {
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
av::gua::PickSelector::evaluate() {
  av::gua::Selector::evaluate();

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
