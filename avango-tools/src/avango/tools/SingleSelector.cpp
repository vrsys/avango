#include <avango/tools/SingleSelector.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::SingleSelector"));
}

AV_FC_DEFINE(av::tools::SingleSelector);

AV_FIELD_DEFINE(av::tools::SFSingleSelector);
AV_FIELD_DEFINE(av::tools::MFSingleSelector);

av::tools::SingleSelector::SingleSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(LastTarget, false);
}

av::tools::SingleSelector::~SingleSelector()
{}

void
av::tools::SingleSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::SingleSelector, true);

    SFSingleSelector::initClass("av::tools::SFSingleSelector", "av::Field");
    MFSingleSelector::initClass("av::tools::MFSingleSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::SingleSelector::evaluate()
{
  av::tools::Selector::evaluate();

  if (!Targets.isEmpty())
  {
    SelectedTargets.setValue(MFTargetHolder::ContainerType(1,
      LastTarget.getValue() ? Targets.getValue().back() : Targets.getValue().front()));
  }
  else if (!SelectedTargets.isEmpty())
    SelectedTargets.clear();
}
