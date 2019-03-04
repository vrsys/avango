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

#include <avango/tools/UnionSelector.hpp>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::tools::UnionSelector"));
}

AV_FC_DEFINE(av::tools::UnionSelector);

AV_FIELD_DEFINE(av::tools::SFUnionSelector);
AV_FIELD_DEFINE(av::tools::MFUnionSelector);

av::tools::UnionSelector::UnionSelector()
{
    AV_FC_ADD_FIELD(TargetSet1, MFTargetHolder::ContainerType());
    AV_FC_ADD_FIELD(TargetSet2, MFTargetHolder::ContainerType());
}

av::tools::UnionSelector::~UnionSelector() {}

void av::tools::UnionSelector::initClass()
{
    if(!isTypeInitialized())
    {
        av::tools::Selector::initClass();

        AV_FC_INIT(av::tools::Selector, av::tools::UnionSelector, true);

        SFUnionSelector::initClass("av::tools::SFUnionSelector", "av::Field");
        MFUnionSelector::initClass("av::tools::MFUnionSelector", "av::Field");
    }
}

/* virtual */ void av::tools::UnionSelector::evaluate()
{
    av::tools::Selector::evaluate();

    if(TargetSet1.isEmpty())
    {
        if(!TargetSet2.isEmpty())
            SelectedTargets.setValue(TargetSet2.getValue());
        else if(!SelectedTargets.isEmpty())
            SelectedTargets.clear();
    }
    else if(TargetSet2.isEmpty())
        SelectedTargets.setValue(TargetSet1.getValue());
    else
    {
        const MFTargetHolder::ContainerType& set1 = TargetSet1.getValue();
        const MFTargetHolder::ContainerType& set2 = TargetSet2.getValue();
        MFTargetHolder::ContainerType selected_targets;

        for(MFTargetHolder::ContainerType::const_iterator holder1 = set1.begin(); holder1 != set1.end(); ++holder1)
        {
            const SFContainer::ValueType& target = (*holder1)->Target.getValue();
            MFTargetHolder::ContainerType::const_iterator holder2 = find(set2, target);
            if(holder2 == set2.end())
                selected_targets.push_back(*holder1);
            else
                selected_targets.push_back(combine(*holder1, *holder2));
        }

        for(MFTargetHolder::ContainerType::const_iterator holder2 = set2.begin(); holder2 != set2.end(); ++holder2)
        {
            if(!hasTarget(set1, (*holder2)->Target.getValue()))
                selected_targets.push_back(*holder2);
        }

        SelectedTargets.setValue(selected_targets);
    }
}
