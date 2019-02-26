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

#ifndef shade_shaders_Initializeable_H
#define shade_shaders_Initializeable_H shade_shaders_Initializeable_H

#include "../Shader.h"
#include "../types/void.h"
#include "../utility.h"

namespace shade
{
namespace shaders
{
class Initializeable : public ShaderBase<Initializeable>
{
  public:
    std::string get_method_name(ShaderEnvironment env);

    virtual void_<> init_vertex(void) { return void_<>(); };

    virtual void_<> init_fragment(void) { return void_<>(); };

    virtual void_<> init_geometry(void) { return void_<>(); };

    virtual void_<> init_post_geometry(void) { return void_<>(); };

  private:
    SHADE_BASE_DECL(Initializeable)
};

} // namespace shaders
} // namespace shade

#endif /* shade_shaders_Initializeable_H */
