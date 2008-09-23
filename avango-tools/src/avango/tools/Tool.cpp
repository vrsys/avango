// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/tools/Tool.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::Tool"));
}

AV_FC_DEFINE_ABSTRACT(av::tools::Tool);

AV_FIELD_DEFINE(av::tools::SFTool);
AV_FIELD_DEFINE(av::tools::MFTool);

av::tools::Tool::Tool()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
}

av::tools::Tool::~Tool()
{}

void
av::tools::Tool::initClass()
{
  if (!isTypeInitialized())
  {
    av::Object::initClass();
    av::tools::TargetHolder::initClass();

    AV_FC_INIT_ABSTRACT(av::Object, av::tools::Tool, true);

    SFTool::initClass("av::tools::SFTool", "av::Field");
    MFTool::initClass("av::tools::MFTool", "av::Field");
  }
}

/* virtual */ void
av::tools::Tool::evaluate()
{
  av::Object::evaluate();

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
av::tools::Tool::evaluateTarget(TargetHolder&)
{}

/* virtual */ void
av::tools::Tool::evaluateAddedTarget(TargetHolder&)
{}

/* virtual */ void
av::tools::Tool::evaluateKeptTarget(TargetHolder& holder)
{}

/* virtual */ void
av::tools::Tool::evaluateRemovedTarget(TargetHolder& holder)
{}
