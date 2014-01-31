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

#include <shade/shaders/ShadowPointLight.h>

using namespace shade;
using namespace shade::shaders;

SHADE_CLASS_INIT(ShadowPointLight, "ShadowPointLight.glsl",
    SHADE_NONE,
    SHADE_DEFS((position)(color)(shadow_map)(modelview)(transformed_position)(transformed_surface_position)))

void_<> ShadowPointLight::illuminate(objref<> surface, vec3<> position)
{
  return invoke< shade::void_<> >("ShadowPointLight_illuminate_impl");
}

void_<> ShadowPointLight::transform(objref<> material)
{
  return invoke< shade::void_<> >("ShadowPointLight_transform_impl");
}
