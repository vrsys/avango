// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include "Displacement.h"

SHADE_CLASS_INIT(Displacement, "Displacement.glsl",
    SHADE_ENV_DEFS(shade::geometry_shader, (interpolate)),
    SHADE_DEFS((normal)(light_normal)(interpolated_light_normal)(coordinate_system)(texture_unit)(uvcoord)))

shade::void_<> Displacement::enter(void)
{
  return invoke< shade::void_<> >("Displacement_enter_impl");
}

shade::vec3<> Displacement::transform_inverse(shade::vec3<>)
{
  return invoke< shade::vec3<> >("Displacement_transform_inverse_impl");
}

shade::void_<> Displacement::init_vertex(void)
{
  return invoke< shade::void_<> >("Displacement_init_vertex_impl");
}

shade::void_<> Displacement::interpolate(shade::vec3<>)
{
  return invoke< shade::void_<> >("Displacement_interpolate_impl");
}
