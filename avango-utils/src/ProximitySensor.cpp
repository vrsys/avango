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

#include "../include/avango/utils/ProximitySensor.h"
#include <gua/math.hpp>
#include <gua/math.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::utils::ProximitySensor"));
}

AV_FC_DEFINE(av::utils::ProximitySensor);

av::utils::ProximitySensor::ProximitySensor()
{
    AV_FC_ADD_FIELD(MinDistance, 0.);
    AV_FC_ADD_FIELD(MaxDistance, 1.);
    AV_FC_ADD_FIELD(ReferencePosition, ::gua::math::mat4());
    AV_FC_ADD_FIELD(CurrentPosition, ::gua::math::mat4());
    AV_FC_ADD_FIELD(Proximity, 0.);
    AV_FC_ADD_FIELD(Switch, 0);

    Name.setValue("ProximitySensor");
}

av::utils::ProximitySensor::~ProximitySensor() {}

void av::utils::ProximitySensor::initClass()
{
    if(isTypeInitialized())
        return;

    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::utils::ProximitySensor, true);
}

/* virtual */ void av::utils::ProximitySensor::evaluate()
{
    av::FieldContainer::evaluate();

    double distance = scm::math::length(::gua::math::get_translation(ReferencePosition.getValue()) - ::gua::math::get_translation(CurrentPosition.getValue()));
    double normalized_distance = (distance - MinDistance.getValue()) / (MaxDistance.getValue() - MinDistance.getValue());
    double proximity = smooth(1. - normalized_distance);

    set_proximity_value(proximity);

    if(proximity == 0.)
        set_switch_value(-1);
    else
        set_switch_value(0);
}

void av::utils::ProximitySensor::set_proximity_value(double proximity)
{
    if(Proximity.getValue() == proximity)
        return;

    Proximity.setValue(proximity);
}

void av::utils::ProximitySensor::set_switch_value(int switch_)
{
    if(Switch.getValue() == switch_)
        return;

    Switch.setValue(switch_);
}

double av::utils::ProximitySensor::smooth(double x)
{
    if(x <= 0.)
        return 0.;
    else if(x >= 1.)
        return 1.;

    return ((-2 * x * x * x) + (3 * x * x));
}
