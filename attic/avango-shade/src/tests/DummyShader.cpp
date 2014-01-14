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

#include "DummyShader.h"
#include "../../core/include/shade/ShaderEnvironment.h"
#include "../include/avango/shade/Init.h"

SHADE_CLASS_INIT(DummyShader, "",
                 SHADE_NONE,
                 SHADE_ENV_DEFS(shade::application_stage,
                   (diffuse)(specular)(color)(light)
                   (vec3value)(vec2value)(intvalue)
                   (mat4value)
                   (listvalue)(texturevalue)
                   )
                 )

void initDummyShader(void)
{
  static bool once = true;
  if (once)
  {
    once = false;
    av::shade::Init::registerShader<DummyShader>("DummyShader");
  }
}
