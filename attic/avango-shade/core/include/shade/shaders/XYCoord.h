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

#ifndef shade_shaders_XYCoord_H
#define shade_shaders_XYCoord_H shade_shaders_XYCoord_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/Gettable.h"
#include "../shaders/Initializeable.h"
#include "../types/objref.h"
#include "../types/Varying.h"
#include "../types/vec2.h"
#include "../types/void.h"


namespace shade
{
  namespace shaders
  {

    class XYCoord : public ShaderBase<XYCoord, Initializeable, Gettable<vec2> >
    {
    public:

      /*virtual*/ void_<> init_vertex(void);

      /*virtual*/ vec2<> get(void);

    private:

      void get_inline(formatter::Generator& generator);

      Varying<vec2> xy_coord;

      SHADE_MULTI_DERIVED_DECL(XYCoord, (Initializeable)(Gettable<vec2>))
    };

  }
}

#endif /* shade_shaders_XYCoord_H */
