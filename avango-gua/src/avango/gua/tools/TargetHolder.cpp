#include <avango/gua/tools/TargetHolder.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TargetHolder"));
}

AV_FC_DEFINE(av::gua::TargetHolder);

AV_FIELD_DEFINE(av::gua::SFTargetHolder);
AV_FIELD_DEFINE(av::gua::MFTargetHolder);

av::gua::TargetHolder::TargetHolder()
{
  AV_FC_ADD_FIELD(Target, 0);
  AV_FC_ADD_FIELD(Creator, 0);
  AV_FC_ADD_FIELD(ParentTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Keep, false);
}

av::gua::TargetHolder::~TargetHolder()
{}

void
av::gua::TargetHolder::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::gua::TargetHolder, true);

    SFTargetHolder::initClass("av::gua::SFTargetHolder", "av::Field");
    MFTargetHolder::initClass("av::gua::MFTargetHolder", "av::Field");
  }
}

av::gua::TargetHolder::FindList
av::gua::TargetHolder::find(Type type)
{
  FindList found_holders;
  find(type, found_holders);
  return found_holders;
}

/* virtual */ bool
av::gua::TargetHolder::keep()
{
  return (Keep.getValue() || !ParentTargets.isEmpty());
}

void
av::gua::TargetHolder::find(Type type, FindList& foundHolders)
{
  if (getTypeId().isOfType(type))
    foundHolders.push_back(this);

  const MFTargetHolder::ContainerType &parent_targets = ParentTargets.getValue();
  for (MFTargetHolder::ContainerType::const_iterator parent_holder = parent_targets.begin();
       parent_holder != parent_targets.end(); ++parent_holder)
  {
    (*parent_holder)->find(type, foundHolders);
  }
}

