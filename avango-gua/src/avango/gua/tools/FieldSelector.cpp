#include <avango/gua/tools/FieldSelector.hpp>

#include <avango/Logger.h>
#include <boost/regex.hpp>
#include <iostream>



namespace
{
  av::Logger& logger(av::getLogger("av::gua::FieldSelector"));
}

AV_FC_DEFINE(av::gua::FieldSelector);

AV_FIELD_DEFINE(av::gua::SFFieldSelector);
AV_FIELD_DEFINE(av::gua::MFFieldSelector);

av::gua::FieldSelector::FieldSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableFieldName, "");
  AV_FC_ADD_FIELD(EqualNameOnly, true);
  AV_FC_ADD_FIELD(TreatAsRegularExpression, false);
}

av::gua::FieldSelector::~FieldSelector()
{}

void
av::gua::FieldSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::FieldSelector, true);

    SFFieldSelector::initClass("av::gua::SFFieldSelector", "av::Field");
    MFFieldSelector::initClass("av::gua::MFFieldSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::FieldSelector::evaluate()
{
  av::gua::Selector::evaluate();

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
av::gua::FieldSelector::isSelectable(av::FieldContainer& object)
{
  const std::string field_name = SelectableFieldName.getValue();
  const std::vector<Field*> &fields = object.getFields();
  std::vector<Field*>::const_iterator field = fields.end();

  if (!TreatAsRegularExpression.getValue() )
  {
    if (EqualNameOnly.getValue())
      for (field = fields.begin();field!=fields.end();++field)
      {
        if ((*field)->getName() == field_name)
          break;
      }
    else
    {
      field = fields.begin();
      while (field != fields.end() && field_name.find((*field)->getName()) == std::string::npos)
        ++field;
    }
  }
  else
  {
    for (field = fields.begin();field!=fields.end();++field)
    {
      const std::string expr = EqualNameOnly.getValue() ? (".*" + (*field)->getName() + ".*") : (*field)->getName();
      boost::regex e(expr);
      if(regex_match(field_name, e))
        break;
    }
  }

  return (field != fields.end());
}
