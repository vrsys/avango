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

#ifndef shade_shaders_Plastic_H
#define shade_shaders_Plastic_H shade_shaders_Plastic_H

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

    class Plastic : public ShaderBase<Plastic, IlluminatedMaterial>
    {
    public:
      Plastic(float diffuse = 1., float specular = 0.);

      /*virtual*/ void_<> illuminance(vec3<> color, vec3<> direction);

      ValueReference<float_, uniform> diffuse;
      ValueReference<float_, uniform> specular;
      ValueReference<float_, uniform> roughness;
      ValueReference<vec4, uniform> color;

    private:
      SHADE_DERIVED_DECL(Plastic, IlluminatedMaterial)
    };

  }
}

#endif /* shade_shaders_Plastic_H */
