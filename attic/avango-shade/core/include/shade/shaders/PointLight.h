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

#ifndef shade_shaders_PointLight_H
#define shade_shaders_PointLight_H shade_shaders_PointLight_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/ILight.h"
#include "../shaders/InverseTransformInterpolator.h"
#include "../types/uniform.h"
#include "../types/Varying.h"
#include "../types/ValueReference.h"

namespace shade
{
  namespace shaders
  {

    class PointLight : public ShaderBase<PointLight, ILight>
    {
    public:

      /*virtual*/ void_<> illuminate(objref<>, vec3<> position);
      /*virtual*/ void_<> transform(objref<> coordinate_system);

      ValueReference<vec3, uniform> position;
      ValueReference<vec3, uniform> color;

    private:

      Varying<vec3, InverseTransformInterpolator> transformed_position;

      SHADE_DERIVED_DECL(PointLight, ILight)
    };

  }
}

#endif /* shade_shaders_PointLight_H */
