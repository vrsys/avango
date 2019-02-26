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

#ifndef shade_ObjectMap_H
#define shade_ObjectMap_H shade_ObjectMap_H

#include <vector>
#include <map>

namespace shade
{
class Shader;

class ObjectMap
{
  public:
    typedef std::vector<Shader*>::size_type Index;

    void add_object(Shader* shader);

    Index get_next_index(void) const;

    Shader* get_object(Index index) const;
    Shader* operator[](Index index) const;

    Index get_index(Shader* shader) const;

  private:
    std::vector<Shader*> m_objects;
    std::map<Shader*, Index> m_indices;
};

} // namespace shade

#endif /* shade_ObjectMap_H */
