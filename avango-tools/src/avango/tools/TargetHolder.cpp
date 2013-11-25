#include <avango/tools/TargetHolder.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::TargetHolder"));
}

AV_FC_DEFINE(av::tools::TargetHolder);

AV_FIELD_DEFINE(av::tools::SFTargetHolder);
AV_FIELD_DEFINE(av::tools::MFTargetHolder);

av::tools::TargetHolder::TargetHolder()
{
  AV_FC_ADD_FIELD(Target, 0);
  AV_FC_ADD_FIELD(Creator, 0);
  AV_FC_ADD_FIELD(ParentTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Keep, false);
}

av::tools::TargetHolder::~TargetHolder()
{}

void
av::tools::TargetHolder::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::tools::TargetHolder, true);

    SFTargetHolder::initClass("av::tools::SFTargetHolder", "av::Field");
    MFTargetHolder::initClass("av::tools::MFTargetHolder", "av::Field");
  }
}

av::tools::TargetHolder::FindList
av::tools::TargetHolder::find(Type type)
{
  FindList found_holders;
  find(type, found_holders);
  return found_holders;
}

/* virtual */ bool
av::tools::TargetHolder::keep()
{
  return (Keep.getValue() || !ParentTargets.isEmpty());
}

void
av::tools::TargetHolder::find(Type type, FindList& foundHolders)
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

