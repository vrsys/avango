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

#include <shade/shaders/UVPlastic.h>

using namespace shade;
using namespace shade::shaders;

SHADE_CLASS_INIT(UVPlastic, "UVPlastic.glsl", SHADE_ENV_DEFS(shade::fragment_shader, (illuminance_uv)), SHADE_DEFS((diffuse)(specular)(color)(uv_color)))

UVPlastic::UVPlastic(float d, float s) : diffuse(d), specular(s) {}

void_<> UVPlastic::illuminance(vec3<> color, vec3<> direction) { return invoke<shade::void_<>>("UVPlastic_illuminance_impl"); }

void_<> UVPlastic::illuminance_uv(vec3<> color, vec3<> direction) { return invoke<shade::void_<>>("UVPlastic_illuminance_uv_impl"); }
