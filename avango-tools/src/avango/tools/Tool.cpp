#include <avango/gua/tools/Tool.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Tool"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::Tool);

AV_FIELD_DEFINE(av::gua::SFTool);
AV_FIELD_DEFINE(av::gua::MFTool);

av::gua::Tool::Tool()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
}

av::gua::Tool::~Tool()
{}

void
av::gua::Tool::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    av::gua::TargetHolder::initClass();

    AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::Tool, true);

    SFTool::initClass("av::gua::SFTool", "av::Field");
    MFTool::initClass("av::gua::MFTool", "av::Field");
  }
}

/* virtual */ void
av::gua::Tool::evaluate()
{
  av::FieldContainer::evaluate();

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  for (MFTargetHolder::ContainerType::const_iterator last_holder = mLastTargets.begin();
       last_holder != mLastTargets.end(); ++last_holder)
  {
    if (!hasTarget(targets, (*last_holder)->Target.getValue()))
      evaluateRemovedTarget(**last_holder);
  }

  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    evaluateTarget(**holder);
    if (hasTarget(mLastTargets, (*holder)->Target.getValue()))
      evaluateKeptTarget(**holder);
    else
      evaluateAddedTarget(**holder);
  }

  mLastTargets = targets;
}

/* virtual */ void
av::gua::Tool::evaluateTarget(TargetHolder&)
{}

/* virtual */ void
av::gua::Tool::evaluateAddedTarget(TargetHolder&)
{}

/* virtual */ void
av::gua::Tool::evaluateKeptTarget(TargetHolder& holder)
{}

/* virtual */ void
av::gua::Tool::evaluateRemovedTarget(TargetHolder& holder)
{}
