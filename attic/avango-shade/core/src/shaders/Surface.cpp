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

#include <shade/shaders/Surface.h>

using namespace shade;
using namespace shade::shaders;

SHADE_CLASS_INIT
(
 Surface, "Surface.glsl",
 SHADE_NONE,
 SHADE_ENV_DEFS(application_stage, (material)(geometry))
 )

shade::shaders::Surface::Surface(void)
{
}

shade::void_<> Surface::enter_vertex(void)
{
  return invoke< shade::void_<> >("Surface_vertex_impl"); 
}

shade::void_<> Surface::enter_geometry(void)
{
  if (!geometry)
    return void_<>();

  return invoke< shade::void_<> >("Surface_geometry_impl");
}

shade::void_<> Surface::enter_fragment(void)
{
  return invoke< shade::void_<> >("Surface_fragment_impl");
}
