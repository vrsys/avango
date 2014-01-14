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

#include <shade/shaders/Enterable.h>
#include <exception>

using namespace shade::shaders;

SHADE_CLASS_INIT(Enterable, "", 
    SHADE_ENV_DEFS(shade::vertex_shader, (enter_vertex))
    SHADE_ENV_DEFS(shade::fragment_shader, (enter_fragment))
    SHADE_ENV_DEFS(shade::geometry_shader, (enter_geometry)),
    SHADE_NONE
    )


std::string Enterable::get_entry_name(ShaderEnvironment env)
{
  if (env == shade::vertex_shader)
    return "enter_vertex";
  else if (env == shade::fragment_shader)
    return "enter_fragment";
  else if (env == shade::geometry_shader)
    return "enter_geometry";

  throw std::invalid_argument("Invalid ShaderEnvironment");
}

bool Enterable::has_entry(ShaderEnvironment env)
{
  if (env == shade::vertex_shader)
    return enter_vertex().has_function();
  else if (env == shade::fragment_shader)
    return enter_fragment().has_function();
  else if (env == shade::geometry_shader)
    return enter_geometry().has_function();

  return false;
}
