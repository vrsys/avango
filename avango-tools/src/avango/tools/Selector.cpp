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
