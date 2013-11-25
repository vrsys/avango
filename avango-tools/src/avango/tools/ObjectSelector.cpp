#include <avango/gua/tools/ObjectSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ObjectSelector"));
}

AV_FC_DEFINE(av::gua::ObjectSelector);

AV_FIELD_DEFINE(av::gua::SFObjectSelector);
AV_FIELD_DEFINE(av::gua::MFObjectSelector);

av::gua::ObjectSelector::ObjectSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableObjects, MFContainer::ContainerType());
  AV_FC_ADD_FIELD(SelectableTargets, MFTargetHolder::ContainerType());
}

av::gua::ObjectSelector::~ObjectSelector()
{}

void
av::gua::ObjectSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::ObjectSelector, true);

    SFObjectSelector::initClass("av::gua::SFObjectSelector", "av::Field");
    MFObjectSelector::initClass("av::gua::MFObjectSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::ObjectSelector::evaluate()
{
  av::gua::Selector::evaluate();

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  const MFContainer::ContainerType &sel_objects = SelectableObjects.getValue();
  const MFTargetHolder::ContainerType &sel_targets = SelectableTargets.getValue();
  MFTargetHolder::ContainerType selected_targets;

  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    bool found = false;

    if (hasObject(sel_objects, target))
    {
      selected_targets.push_back(*holder);
      found = true;
    }
    else
    {
      MFTargetHolder::ContainerType::const_iterator sel_holder = find(sel_targets, target);
      if (sel_holder != sel_targets.end())
      {
        selected_targets.push_back(combine(*holder, *sel_holder));
        found = true;
      }
    }
  }

  if (!selected_targets.empty() || !SelectedTargets.isEmpty())
    SelectedTargets.setValue(selected_targets);
}
