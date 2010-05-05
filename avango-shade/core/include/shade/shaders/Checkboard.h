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

#ifndef shade_shaders_Checkboard_H
#define shade_shaders_Checkboard_H shade_shaders_Checkboard_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/Gettable.h"
#include "../types/vec3.h"
#include "../types/vec4.h"
#include "../types/ValueReference.h"
#include "../types/uniform.h"


namespace shade
{
  namespace shaders
  {

    class Checkboard : public ShaderBase<Checkboard, Gettable<vec4> >
    {
    public:

      /*virtual*/ vec4<Type> get(void);
      vec4<> get_pattern(void);

      ValueReference<vec3, uniform> position;
      vec3<uniform> scale;
      ValueReference<vec4, uniform> color1;
      ValueReference<vec4, uniform> color2;

    private:

      void get_inline(formatter::Generator& generator);

      SHADE_DERIVED_DECL(Checkboard, Gettable<vec4>)
    };

  }
}

#endif /* shade_shaders_Checkboard_H */
