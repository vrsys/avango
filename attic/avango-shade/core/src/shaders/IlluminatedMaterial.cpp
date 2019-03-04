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

#include <shade/shaders/IlluminatedMaterial.h>

using namespace shade;
using namespace shade::shaders;

SHADE_CLASS_INIT(IlluminatedMaterial,
                 "IlluminatedMaterial.glsl",
                 SHADE_ENV_DEFS(shade::vertex_shader, (chained_init_vertex)) SHADE_ENV_DEFS(shade::fragment_shader, (illuminance)(init)(accum_color)(gather)(get_normal)(get_position)(get_eyepoint)),
                 SHADE_ENV_DEFS(shade::application_stage, (lights)(coordinate_system)) SHADE_DEFS((acc_color)))

void_<> IlluminatedMaterial::init_vertex(void) { return invoke<shade::void_<>>("IlluminatedMaterial_init_vertex_impl"); }

vec4<> IlluminatedMaterial::shade(void) { return invoke<shade::vec4<>>("IlluminatedMaterial_shade_impl"); }

void_<> IlluminatedMaterial::init(void) { return invoke<shade::void_<>>("IlluminatedMaterial_init_impl"); }

void_<> IlluminatedMaterial::accum_color(vec4<> color) { return invoke<shade::void_<>>("IlluminatedMaterial_accum_color_impl"); }

vec4<> IlluminatedMaterial::gather(void) { return invoke<shade::vec4<>>("IlluminatedMaterial_gather_impl"); }

vec3<> IlluminatedMaterial::get_normal(void) { return invoke<shade::vec3<>>("IlluminatedMaterial_get_normal_impl"); }

vec3<> IlluminatedMaterial::get_position(void) { return invoke<shade::vec3<>>("IlluminatedMaterial_get_position_impl"); }

vec3<> IlluminatedMaterial::get_eyepoint(void) { return invoke<shade::vec3<>>("IlluminatedMaterial_get_eyepoint_impl"); }
