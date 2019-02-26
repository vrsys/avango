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

#include <shade/shaders/Initializeable.h>
#include <exception>

using namespace shade::shaders;

SHADE_CLASS_INIT(Initializeable,
                 "",
                 SHADE_ENV_DEFS(shade::vertex_shader, (init_vertex)) SHADE_ENV_DEFS(shade::fragment_shader, (init_fragment))
                     SHADE_ENV_DEFS(shade::geometry_shader, (init_geometry)(init_post_geometry)),
                 SHADE_NONE)

std::string Initializeable::get_method_name(ShaderEnvironment env)
{
    if(env == shade::vertex_shader)
        return "init_vertex";
    else if(env == shade::fragment_shader)
        return "init_fragment";
    else if(env == shade::geometry_shader)
        return "init_geometry";
    else if(env == shade::post_geometry_shader)
        return "init_post_geometry";

    throw std::invalid_argument("Given ShaderEnvironment does not support initializers");
}
