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

#ifndef shade_shaders_Geometry_H
#define shade_shaders_Geometry_H shade_shaders_Geometry_H

#include "../Shader.h"
#include "../utility.h"
#include "../types/void.h"
#include "../types/vec3.h"
#include "../types/ValueReference.h"
#include "../types/local.h"

namespace shade
{
namespace shaders
{
class Geometry : public ShaderBase<Geometry>
{
  public:
    virtual void_<> enter(void) { return void_<>(); };

    virtual vec3<> transform_inverse(vec3<>);

    ValueReference<shade::vec3, shade::local> uvw;

  private:
    SHADE_BASE_DECL(Geometry)
};

} // namespace shaders
} // namespace shade

#endif /* shade_shaders_Geometry_H */
