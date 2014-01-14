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

#ifndef shade_shaders_UVPointLight_H
#define shade_shaders_UVPointLight_H shade_shaders_UVPointLight_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/ILight.h"
#include "../types/uniform.h"
#include "../types/Varying.h"

namespace shade
{
  namespace shaders
  {

    class UVPointLight : public ShaderBase<UVPointLight, ILight>
    {
    public:

      /*virtual*/ void_<> illuminate(objref<>, vec3<> position);
      /*virtual*/ void_<> transform(objref<> coordinate_system);

      vec3<uniform> position;
      vec3<uniform> color;

    private:

      Varying<vec3> transformed_position;

      SHADE_DERIVED_DECL(UVPointLight, ILight)
    };

  }
}

#endif /* shade_shaders_UVPointLight_H */
