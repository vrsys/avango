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

#ifndef Splitter_H
#define Splitter_H Splitter_H

#include <shade/Shader.h>
#include <shade/Type.h>
#include <shade/shaders/Geometry.h>
#include <shade/types/ValueReference.h>
#include <shade/types/vec3.h>
#include <shade/types/void.h>
#include <shade/types/uniform.h>
#include <shade/utility.h>

class Splitter : public shade::ShaderBase<Splitter, shade::shaders::Geometry>
{
public:

  /*virtual*/ shade::void_<> enter(void);

private:
  SHADE_DERIVED_DECL(Splitter, shade::shaders::Geometry)
};

#endif /* Splitter_H */
