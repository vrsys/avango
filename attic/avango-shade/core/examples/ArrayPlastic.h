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

#ifndef shade_shaders_ArrayPlastic_H
#define shade_shaders_ArrayPlastic_H shade_shaders_ArrayPlastic_H

#include <shade/Shader.h>
#include <shade/Type.h>
#include <shade/utility.h>
#include <shade/shaders/IlluminatedMaterial.h>
#include <shade/types/ValueReference.h>
#include <shade/types/sampler2DArray.h>
#include <shade/types/vec2.h>
#include <shade/types/uniform.h>
#include <shade/types/float.h>

class ArrayPlastic : public shade::ShaderBase<ArrayPlastic, shade::shaders::IlluminatedMaterial>
{
  public:
    ArrayPlastic(float diffuse = 1., float specular = 0.);

    /*virtual*/ shade::void_<> illuminance(shade::vec3<> color, shade::vec3<> direction);

    shade::ValueReference<shade::vec2, shade::uniform> uv;
    shade::sampler2DArray texture_unit;

  private:
    shade::float_<shade::uniform> diffuse;
    shade::float_<shade::uniform> specular;

    SHADE_DERIVED_DECL(ArrayPlastic, shade::shaders::IlluminatedMaterial)
};

#endif /* shade_shaders_ArrayPlastic_H */
