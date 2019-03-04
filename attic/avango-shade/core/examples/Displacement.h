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

#ifndef Displacement_H
#define Displacement_H Displacement_H

#include <shade/Shader.h>
#include <shade/Type.h>
#include <shade/shaders/Geometry.h>
#include <shade/shaders/Initializeable.h>
#include <shade/shaders/TangentSpace.h>
#include <shade/types/ValueReference.h>
#include <shade/types/vec2.h>
#include <shade/types/vec3.h>
#include <shade/types/vec4.h>
#include <shade/types/inout.h>
#include <shade/types/void.h>
#include <shade/types/uniform.h>
#include <shade/types/const.h>
#include <shade/types/local.h>
#include <shade/types/objref.h>
#include <shade/types/sampler2D.h>
#include <shade/utility.h>

class Displacement : public shade::ShaderBase<Displacement, shade::shaders::Geometry, shade::shaders::Initializeable>
{
  public:
    /*virtual*/ shade::void_<> enter(void);
    /*virtual*/ shade::vec3<> transform_inverse(shade::vec3<>);

    /*virtual*/ shade::void_<> init_vertex(void);

    shade::void_<> interpolate(shade::vec3<>);

    shade::objref<shade::shaders::TangentSpace*, shade::const_> coordinate_system;
    shade::sampler2D texture_unit;

  private:
    shade::vec4<shade::inout<shade::vertex_shader, shade::geometry_shader>> normal;
    shade::vec4<shade::inout<shade::vertex_shader, shade::geometry_shader>> light_normal;
    shade::vec2<shade::inout<shade::vertex_shader, shade::geometry_shader>> uvcoord;
    shade::vec4<shade::local> interpolated_light_normal;

    SHADE_MULTI_DERIVED_DECL(Displacement, (shade::shaders::Geometry)(shade::shaders::Initializeable))
};

#endif /* Displacement_H */
