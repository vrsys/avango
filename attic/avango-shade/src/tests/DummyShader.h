// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#ifndef DUMMY_SHADER_H
#define DUMMY_SHADER_H

#include "../../core/include/shade/Shader.h"
#include "../../core/include/shade/utility.h"
#include "../../core/include/shade/types/uniform.h"
#include "../../core/include/shade/types/const.h"
#include "../../core/include/shade/types/float.h"
#include "../../core/include/shade/types/vec4.h"
#include "../../core/include/shade/types/vec3.h"
#include "../../core/include/shade/types/vec2.h"
#include "../../core/include/shade/types/matrix4.h"
#include "../../core/include/shade/types/int.h"
#include "../../core/include/shade/types/objref.h"
#include "../../core/include/shade/types/list.h"
#include "../../core/include/shade/types/sampler2D.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class DummyShader : public shade::ShaderBase<DummyShader>
{
  public:
    shade::float_<shade::uniform> diffuse;
    shade::float_<shade::uniform> specular;
    shade::vec4<shade::uniform> color;
    shade::objref<boost::shared_ptr<DummyShader>, shade::uniform> light;

    shade::vec3<shade::uniform> vec3value;
    shade::vec2<shade::uniform> vec2value;
    shade::int_<shade::uniform> intvalue;
    shade::matrix4<shade::uniform> mat4value;
    shade::sampler2D texturevalue;

    typedef shade::list<std::vector<boost::shared_ptr<DummyShader>>, shade::const_> ListType;
    ListType listvalue;

  private:
    SHADE_BASE_DECL(DummyShader)
};

void initDummyShader(void);

#endif
