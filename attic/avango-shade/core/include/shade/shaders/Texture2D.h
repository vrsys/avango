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

#ifndef shade_shaders_Texture2D_H
#define shade_shaders_Texture2D_H shade_shaders_Texture2D_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/Gettable.h"
#include "../types/vec2.h"
#include "../types/vec4.h"
#include "../types/sampler2D.h"
#include "../types/ValueReference.h"
#include "../types/uniform.h"


namespace shade
{
  namespace shaders
  {

    class Texture2D : public ShaderBase<Texture2D, Gettable<vec4> >
    {
    public:

      /*virtual*/ vec4<> get(void);
      vec4<> get_texture(void);

      ValueReference<vec2, uniform> uv;
      sampler2D texture_unit;

    private:

      void get_inline(formatter::Generator& generator);

      SHADE_DERIVED_DECL(Texture2D, Gettable<vec4>)
    };

  }
}

#endif /* shade_shaders_Texture2D_H */
