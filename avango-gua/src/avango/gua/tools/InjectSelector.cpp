#include <avango/gua/tools/InjectSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::InjectSelector"));
}

AV_FC_DEFINE(av::gua::InjectSelector);

AV_FIELD_DEFINE(av::gua::SFInjectSelector);
AV_FIELD_DEFINE(av::gua::MFInjectSelector);

av::gua::InjectSelector::InjectSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(InjectTargets, MFTargetHolder::ContainerType());
}

av::gua::InjectSelector::~InjectSelector()
{}

void
av::gua::InjectSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::InjectSelector, true);

    SFInjectSelector::initClass("av::gua::SFInjectSelector", "av::Field");
    MFInjectSelector::initClass("av::gua::MFInjectSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::InjectSelector::evaluate()
{
  av::gua::Selector::evaluate();

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
