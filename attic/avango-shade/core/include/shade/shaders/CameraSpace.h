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

#ifndef shade_shaders_CameraSpace_H
#define shade_shaders_CameraSpace_H shade_shaders_CameraSpace_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/ICoordinateSystem.h"
#include "../shaders/Initializeable.h"
#include "../types/objref.h"
#include "../types/Varying.h"
#include "../types/vec3.h"
#include "../types/vec4.h"
#include "../types/void.h"

namespace shade
{
namespace formatter
{
class Generator;
}

namespace shaders
{
class CameraSpace : public ShaderBase<CameraSpace, ICoordinateSystem, Initializeable, Gettable<vec3>>
{
  public:
    /*virtual*/ vec4<> transform(vec4<>);

    /*virtual*/ void_<> init_vertex(void);

    /*virtual*/ vec3<Type> get(void);

    /*virtual*/ vec3<> get_transformed_normal(void);

    /*virtual*/ vec3<> get_transformed_eyepoint(void);

  private:
    void get_inline(formatter::Generator& generator);

    Varying<vec3> position;
    Varying<vec3> normal;

    SHADE_MULTI_DERIVED_DECL(CameraSpace, (ICoordinateSystem)(Initializeable)(Gettable<vec3>))
};

} // namespace shaders
} // namespace shade

#endif /* shade_shaders_CameraSpace_H */
