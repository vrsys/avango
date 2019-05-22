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

#include <shade/ObjectMap.h>

void shade::ObjectMap::add_object(Shader* shader)
{
    m_indices[shader] = get_next_index();
    m_objects.push_back(shader);
}

shade::ObjectMap::Index shade::ObjectMap::get_next_index(void) const { return m_objects.size(); }

shade::Shader* shade::ObjectMap::get_object(Index index) const { return m_objects[index]; }

shade::Shader* shade::ObjectMap::operator[](Index index) const { return get_object(index); }

shade::ObjectMap::Index shade::ObjectMap::get_index(Shader* shader) const { return m_indices.find(shader)->second; }
