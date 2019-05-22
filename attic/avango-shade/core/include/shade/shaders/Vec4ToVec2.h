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

#ifndef shade_shaders_Vec4ToVec2_H
#define shade_shaders_Vec4ToVec2_H shade_shaders_Vec4ToVec2_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/Gettable.h"
#include "../types/objref.h"
#include "../types/vec2.h"
#include "../types/vec4.h"
#include "../types/void.h"
#include "../types/const.h"

namespace shade
{
namespace shaders
{
class Vec4ToVec2 : public ShaderBase<Vec4ToVec2, Gettable<vec2>>
{
  public:
    /*virtual*/ vec2<> get(void);

    objref<boost::shared_ptr<Gettable<vec4>>, const_> value;

  private:
    SHADE_DERIVED_DECL(Vec4ToVec2, Gettable<vec2>)
};

} // namespace shaders
} // namespace shade

#endif /* shade_shaders_Vec4ToVec2_H */
