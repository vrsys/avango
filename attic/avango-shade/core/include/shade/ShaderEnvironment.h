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

#ifndef shade_ShaderEnvironment_H
#define shade_ShaderEnvironment_H shade_ShaderEnvironment_H

#include <string>

namespace shade
{
enum ShaderEnvironment
{
    vertex_shader,
    fragment_shader,
    geometry_shader,
    num_shader_environment,
    invariant_shader = num_shader_environment,
    post_geometry_shader,

    vertex_stage = vertex_shader,
    fragment_stage = fragment_shader,
    geometry_stage = geometry_shader,
    invariant_stage = invariant_shader,
    post_geometry_stage = post_geometry_shader,
    application_stage
};

std::string get_shader_env_define(ShaderEnvironment env);

} // namespace shade

#endif /* shade_ShaderEnvironment_H */
