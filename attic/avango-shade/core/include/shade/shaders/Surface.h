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

#ifndef shade_shaders_Surface_H
#define shade_shaders_Surface_H shade_shaders_Surface_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/Enterable.h"
#include "../shaders/IMaterial.h"
#include "../shaders/Geometry.h"
#include "../types/objref.h"
#include <list>
#include <boost/shared_ptr.hpp>

namespace shade
{
  namespace shaders
  {

    class Surface : public ShaderBase<Surface, Enterable>
    {
    public:
      Surface(void);

      /*virtual*/ shade::void_<> enter_vertex(void);
      /*virtual*/ shade::void_<> enter_geometry(void);
      /*virtual*/ shade::void_<> enter_fragment(void);

      typedef boost::shared_ptr<shaders::IMaterial> MaterialPtr;
      typedef shade::objref<MaterialPtr, shade::const_> MaterialRef;

      typedef boost::shared_ptr<shaders::Geometry> GeometryPtr;
      typedef shade::objref<GeometryPtr, shade::const_> GeometryRef;

      MaterialRef material;
      GeometryRef geometry;

    private:

      SHADE_DERIVED_DECL(Surface, Enterable)
    };

  }
}

#endif /* shade_shaders_Surface_H */
