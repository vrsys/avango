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

#include <shade/shaders/XYCoord.h>

using namespace shade::shaders;

SHADE_CLASS_INIT(XYCoord, "XYCoord.glsl",
    SHADE_NONE,
    SHADE_DEFS((xy_coord))
    )

void XYCoord::get_inline(formatter::Generator& generator)
{
  generator.handle_environment_begin(fragment_shader);
  generator.handle_return_begin();
  generator.handle_function_call_begin();
  generator.handle_function_call_name_begin();
  generator.handle_identifier("XYCoord_get_xy_coord");
  generator.handle_function_call_name_end();
  generator.handle_function_call_parameter_list_begin();
  generator.handle_function_call_parameter_begin();
  generator.handle_identifier("self");
  generator.handle_function_call_parameter_end();
  generator.handle_function_call_parameter_list_end();
  generator.handle_function_call_end();
  generator.handle_return_end();
  generator.handle_environment_end();
  generator.handle_return_begin();
  generator.handle_literal_vec2(0, 0);
  generator.handle_return_end();
}

shade::vec2<> XYCoord::get(void)
{
  return invoke_inline< vec2<> >(boost::bind(&XYCoord::get_inline, boost::ref(*this), _1));
}

shade::void_<> XYCoord::init_vertex(void)
{
  return invoke< void_<> >("XYCoord_init_vertex_impl");
}
