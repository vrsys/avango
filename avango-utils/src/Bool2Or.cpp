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

#include "../include/avango/utils/Bool2Or.h"

namespace
{
av::Logger& logger(av::getLogger("av::utils::Bool2Or"));
}

AV_FC_DEFINE(av::utils::Bool2Or);

av::utils::Bool2Or::Bool2Or()
{
    AV_FC_ADD_FIELD(Input1, false);
    AV_FC_ADD_FIELD(Input2, false);
    AV_FC_ADD_FIELD(Output, false);

    Name.setValue("Bool2Or");
}

av::utils::Bool2Or::~Bool2Or() {}

void av::utils::Bool2Or::initClass()
{
    if(isTypeInitialized())
        return;

    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::utils::Bool2Or, true);
}

/* virtual */ void av::utils::Bool2Or::evaluate()
{
    if(Input1.getValue() || Input2.getValue())
    {
        Output.setValue(true);
    }
    else
    {
        Output.setValue(false);
    }
}
