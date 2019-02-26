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

#include <avango/tools/TriggerSelector.hpp>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::tools::TriggerSelector"));
}

AV_FC_DEFINE(av::tools::TriggerSelector);

AV_FIELD_DEFINE(av::tools::SFTriggerSelector);
AV_FIELD_DEFINE(av::tools::MFTriggerSelector);

av::tools::TriggerSelector::TriggerSelector() : mLastTrigger(false)
{
    AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
    AV_FC_ADD_FIELD(Trigger, false);
    AV_FC_ADD_FIELD(TransitionOnly, false);
}

av::tools::TriggerSelector::~TriggerSelector() {}

void av::tools::TriggerSelector::initClass()
{
    if(!isTypeInitialized())
    {
        av::tools::Selector::initClass();

        AV_FC_INIT(av::tools::Selector, av::tools::TriggerSelector, true);

        SFTriggerSelector::initClass("av::tools::SFTriggerSelector", "av::Field");
        MFTriggerSelector::initClass("av::tools::MFTriggerSelector", "av::Field");
    }
}

/* virtual */ void av::tools::TriggerSelector::evaluate()
{
    av::tools::Selector::evaluate();

    if(Trigger.getValue() && (!mLastTrigger || !TransitionOnly.getValue()))
    {
        if(!SelectedTargets.isEmpty() || !Targets.isEmpty())
            SelectedTargets.setValue(Targets.getValue());
        mLastTrigger = true;
    }
    else
    {
        if(!SelectedTargets.isEmpty())
            SelectedTargets.clear();
        mLastTrigger = false;
    }
}
