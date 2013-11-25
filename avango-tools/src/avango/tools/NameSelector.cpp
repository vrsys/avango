#include <avango/tools/NameSelector.hpp>

#include <avango/Logger.h>
#include <boost/regex.hpp>
#include <iostream>



namespace
{
  av::Logger& logger(av::getLogger("av::tools::NameSelector"));
}

AV_FC_DEFINE(av::tools::NameSelector);

AV_FIELD_DEFINE(av::tools::SFNameSelector);
AV_FIELD_DEFINE(av::tools::MFNameSelector);

av::tools::NameSelector::NameSelector():
  mNamesDirty(true)
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableNames, MFString::ContainerType());
  AV_FC_ADD_FIELD(SelectableTargetNames, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(EqualNameOnly, true);
  AV_FC_ADD_FIELD(TreatAsRegularExpression, false);
}

av::tools::NameSelector::~NameSelector()
{}

void
av::tools::NameSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::NameSelector, true);

    SFNameSelector::initClass("av::tools::SFNameSelector", "av::Field");
    MFNameSelector::initClass("av::tools::MFNameSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::NameSelector::fieldHasChanged(const av::Field& field)
{
  av::tools::Selector::fieldHasChanged(field);

  if (&field == &SelectableNames || &field == &SelectableTargetNames)
    mNamesDirty = true;
}

/* virtual */ void
av::tools::NameSelector::evaluate()
{
  av::tools::Selector::evaluate();

  if (mNamesDirty)
  {
    mNames.clear();
    const MFString::ContainerType &sel_names = SelectableNames.getValue();
    mNames.insert(sel_names.begin(), sel_names.end());

    const MFTargetHolder::ContainerType &sel_targets = SelectableTargetNames.getValue();
    for (MFTargetHolder::ContainerType::const_iterator sel_holder = sel_targets.begin();
         sel_holder != sel_targets.end(); ++sel_holder)
    {
      SFContainer::ValueType target = (*sel_holder)->Target.getValue();
      if (target.isValid())
      {
        SFString::ValueType name = target->Name.getValue();
        if (!name.empty())
          mNames.insert(name);
      }
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
av::tools::NameSelector::isSelectable(av::FieldContainer& object)
{
  std::set<std::string>::const_iterator name = mNames.end();
  const std::string object_name = object.Name.getValue();

  if (!object_name.empty())
  {
    if (!TreatAsRegularExpression.getValue() )
    {
      if (EqualNameOnly.getValue())
        name = mNames.find(object_name);
      else
      {
        name = mNames.begin();
        while (name != mNames.end() && object_name.find(*name) == std::string::npos)
          ++name;
      }
    }
    else
    {
      for(name = mNames.begin();name!=mNames.end();++name)
      {
        const std::string expr = EqualNameOnly.getValue() ? (".*" + *name + ".*") : *name;
        boost::regex e(expr);
        if(regex_match(object_name, e))
          break;
      }


    }
  }

  return (name != mNames.end());
}
