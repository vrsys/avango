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

#ifndef shade_shaders_IlluminatedMaterial_H
#define shade_shaders_IlluminatedMaterial_H shade_shaders_IlluminatedMaterial_H

#include "../Shader.h"
#include "../Type.h"
#include "../utility.h"
#include "../shaders/IMaterial.h"
#include "../shaders/ILight.h"
#include "../shaders/ICoordinateSystem.h"
#include "../shaders/Initializeable.h"
#include "../types/local.h"
#include "../types/vec4.h"
#include "../types/const.h"
#include "../types/list.h"
#include "../types/objref.h"
#include <boost/shared_ptr.hpp>
#include <vector>


namespace shade
{
  namespace shaders
  {

    class IlluminatedMaterial : public ShaderBase<IlluminatedMaterial, IMaterial, Initializeable>
    {
    public:

      /*virtual*/ void_<> init_vertex(void);

      /*virtual*/ vec4<> shade(void);

      virtual void_<> illuminance(vec3<> color, vec3<> direction)
      { return void_<>(); }

      virtual void_<> chained_init_vertex(void)
      { return void_<>(); }

      void_<> init(void);
      void_<> accum_color(vec4<> color);
      vec4<> gather(void);

      vec3<> get_position(void);
      vec3<> get_eyepoint(void);
      vec3<> get_normal(void);

      typedef list< std::vector< boost::shared_ptr<ILight> >, const_ > LightList;
      LightList lights;

      objref< boost::shared_ptr<ICoordinateSystem>, const_ > coordinate_system;

    private:
      vec4<local> acc_color;

      SHADE_MULTI_DERIVED_DECL(IlluminatedMaterial, (IMaterial)(Initializeable))
    };

  }
}

#endif /* shade_shaders_IlluminatedMaterial_H */
