#include <avango/tools/TypeSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::TypeSelector"));
}

AV_FC_DEFINE(av::tools::TypeSelector);

AV_FIELD_DEFINE(av::tools::SFTypeSelector);
AV_FIELD_DEFINE(av::tools::MFTypeSelector);

av::tools::TypeSelector::TypeSelector():
  mTypesDirty(true)
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableTypes, MFContainer::ContainerType());
  AV_FC_ADD_FIELD(SelectableTargetTypes, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(EqualTypeOnly, false);
}

av::tools::TypeSelector::~TypeSelector()
{}

void
av::tools::TypeSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::TypeSelector, true);

    SFTypeSelector::initClass("av::tools::SFTypeSelector", "av::Field");
    MFTypeSelector::initClass("av::tools::MFTypeSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::TypeSelector::fieldHasChanged(const av::Field& field)
{
  av::tools::Selector::fieldHasChanged(field);

  if (&field == &SelectableTypes || &field == &SelectableTargetTypes)
    mTypesDirty = true;
}

/* virtual */ void
av::tools::TypeSelector::evaluate()
{
  av::tools::Selector::evaluate();

  if (mTypesDirty)
  {
    mTypes.clear();

    const MFContainer::ContainerType &sel_objects = SelectableTypes.getValue();
    for (MFContainer::ContainerType::const_iterator sel_object = sel_objects.begin();
         sel_object != sel_objects.end(); ++sel_object)
    {
      mTypes.insert((*sel_object)->getTypeId());
    }

    const MFTargetHolder::ContainerType &sel_targets = SelectableTargetTypes.getValue();
    for (MFTargetHolder::ContainerType::const_iterator sel_holder = sel_targets.begin();
         sel_holder != sel_targets.end(); ++sel_holder)
    {
      if ((*sel_holder)->Target.getValue().isValid())
        mTypes.insert((*sel_holder)->Target.getValue()->getTypeId());
    }
  }

  MFTargetHolder::ContainerType selected_targets;

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    bool found = false;

    if (isSelectable(*target))
    {
      selected_targets.push_back(*holder);
      found = true;
    }
  }

  if (!selected_targets.empty() || !SelectedTargets.isEmpty())
    SelectedTargets.setValue(selected_targets);
}

bool
av::tools::TypeSelector::isSelectable(const av::FieldContainer& object)
{
  std::set<Type>::const_iterator type = mTypes.end();
  const Type object_type = object.getTypeId();

  if (EqualTypeOnly.getValue())
    type = mTypes.find(object_type);
  else
  {
    type = mTypes.begin();
    while (type != mTypes.end() && !object_type.isOfType(*type))
      ++type;
  }

  return (type != mTypes.end());
}
