// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
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
    av::FieldContainer::initClass();
    av::tools::TargetHolder::initClass();

    AV_FC_INIT_ABSTRACT(av::FieldContainer, av::tools::Tool, true);

    SFTool::initClass("av::tools::SFTool", "av::Field");
    MFTool::initClass("av::tools::MFTool", "av::Field");
  }
}

/* virtual */ void
av::tools::Tool::evaluate()
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
