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

#include <shade/shaders/TangentSpace.h>

using namespace shade::shaders;

SHADE_CLASS_INIT(TangentSpace, "TangentSpace.glsl", 
    SHADE_NONE,
    SHADE_DEFS((position)(eyepoint)(tangent)(bitangent)(normal)(normal_map))
    )

TangentSpace::TangentSpace(void)
{
  shade::shaders::Value<shade::vec4, shade::const_ >* normal =
    dynamic_cast<shade::shaders::Value<shade::vec4, shade::const_>*>(*normal_map);
  normal->value.set(0., 0., 1., 0.);
}

shade::vec4<> TangentSpace::transform(vec4<>)
{
  return invoke< vec4<> >("TangentSpace_transform_impl");
}

shade::vec3<shade::Type> TangentSpace::get(void)
{
  return invoke< vec3<Type> >("TangentSpace_get_impl");
}

shade::vec3<> TangentSpace::get_transformed_normal(void)
{
  return invoke< vec3<> >("TangentSpace_get_transformed_normal_impl");
}

shade::vec3<> TangentSpace::get_transformed_eyepoint(void)
{
  return invoke< vec3<> >("TangentSpace_get_transformed_eyepoint_impl");
}

shade::void_<> TangentSpace::init_vertex(void)
{
  return invoke< void_<> >("TangentSpace_init_vertex_impl");
}
