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

#include "ArrayPlastic.h"

SHADE_CLASS_INIT(ArrayPlastic, "ArrayPlastic.glsl",
    SHADE_ENV_DEFS(shade::fragment_shader, (illuminance)),
    SHADE_DEFS((diffuse)(specular)(uv)(texture_unit))
    )

ArrayPlastic::ArrayPlastic(float d, float s) :
  diffuse(d),
  specular(s)
{
}

shade::void_<> ArrayPlastic::illuminance(shade::vec3<> color, shade::vec3<> direction)
{
  return invoke< shade::void_<> >("ArrayPlastic_illuminance_impl");
}
