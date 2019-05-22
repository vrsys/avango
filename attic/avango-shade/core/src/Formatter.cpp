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

#include <shade/Formatter.h>

shade::ShaderEnvironment shade::Formatter::get_shader_env(void) { return m_env; }

void shade::Formatter::setup(shade::ObjectMap::Index num_classes, formatter::Constants::Primitive primitive_type)
{
    m_num_classes = num_classes;
    m_primitive_type = primitive_type;
}

shade::ObjectMap::Index shade::Formatter::get_num_classes(void) const { return m_num_classes; }

unsigned int shade::Formatter::get_num_vertices_in(void) const
{
    switch(m_primitive_type)
    {
    case formatter::Constants::points:
        return 1;
    case formatter::Constants::lines:
        return 2;
    case formatter::Constants::lines_adjacency:
        return 4;
    case formatter::Constants::triangles:
        return 3;
    case formatter::Constants::triangles_adjacency:
        return 6;
    };

    return 0;
}

std::string shade::Formatter::get_initializer_name(void) { return "initializer"; }
