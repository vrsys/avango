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

#ifndef shade_shaders_Constant_H
#define shade_shaders_Constant_H shade_shaders_Constant_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/IMaterial.h"
#include "../types/vec4.h"
#include "../types/ValueReference.h"
#include "../types/uniform.h"
#include "../types/const.h"

namespace shade
{
namespace shaders
{
class Constant : public ShaderBase<Constant, IMaterial>
{
  public:
    /*virtual*/ vec4<> shade(void);

    ValueReference<vec4, uniform> color;

  private:
    SHADE_DERIVED_DECL(Constant, IMaterial)
};

} // namespace shaders
} // namespace shade

#endif /* shade_shaders_Constant_H */
