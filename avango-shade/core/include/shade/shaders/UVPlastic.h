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

#ifndef shade_shaders_UVPlastic_H
#define shade_shaders_UVPlastic_H shade_shaders_UVPlastic_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/IlluminatedMaterial.h"
#include "../types/ValueReference.h"
#include "../types/uniform.h"
#include "../types/float.h"


namespace shade
{
  namespace shaders
  {

    class UVPlastic : public ShaderBase<UVPlastic, IlluminatedMaterial>
    {
    public:
      UVPlastic(float diffuse = 1., float specular = 0.);

      /*virtual*/ void_<> illuminance(vec3<> color, vec3<> direction);
      void_<> illuminance_uv(vec3<> color, vec3<> direction);

      ValueReference<vec4, uniform> color;
      ValueReference<vec4, uniform> uv_color;

    private:
      float_<uniform> diffuse;
      float_<uniform> specular;

      SHADE_DERIVED_DECL(UVPlastic, IlluminatedMaterial)
    };

  }
}

#endif /* shade_shaders_UVPlastic_H */
