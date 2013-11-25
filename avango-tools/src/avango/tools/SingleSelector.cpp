#include <avango/gua/tools/SingleSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SingleSelector"));
}

AV_FC_DEFINE(av::gua::SingleSelector);

AV_FIELD_DEFINE(av::gua::SFSingleSelector);
AV_FIELD_DEFINE(av::gua::MFSingleSelector);

av::gua::SingleSelector::SingleSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(LastTarget, false);
}

av::gua::SingleSelector::~SingleSelector()
{}

void
av::gua::SingleSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Selector::initClass();

    AV_FC_INIT(av::gua::Selector, av::gua::SingleSelector, true);

    SFSingleSelector::initClass("av::gua::SFSingleSelector", "av::Field");
    MFSingleSelector::initClass("av::gua::MFSingleSelector", "av::Field");
  }
}

/* virtual */ void
av::gua::SingleSelector::evaluate()
{
  av::gua::Selector::evaluate();

  if (!Targets.isEmpty())
  {
    SelectedTargets.setValue(MFTargetHolder::ContainerType(1,
      LastTarget.getValue() ? Targets.getValue().back() : Targets.getValue().front()));
  }
  else if (!SelectedTargets.isEmpty())
    SelectedTargets.clear();
}
