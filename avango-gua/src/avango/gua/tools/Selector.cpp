#include <avango/gua/tools/Selector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Selector"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::Selector);

AV_FIELD_DEFINE(av::gua::SFSelector);
AV_FIELD_DEFINE(av::gua::MFSelector);

av::gua::Selector::Selector()
{
  AV_FC_ADD_FIELD(SelectedTargets, MFTargetHolder::ContainerType());
}

av::gua::Selector::~Selector()
{}

void
av::gua::Selector::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    av::gua::TargetHolder::initClass();

    AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::Selector, true);

    SFSelector::initClass("av::gua::SFSelector", "av::Field");
    MFSelector::initClass("av::gua::MFSelector", "av::Field");
  }
}

av::Link<av::gua::TargetHolder>
av::gua::Selector::combine(const Link<TargetHolder> holder1, const Link<TargetHolder> holder2)
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
