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

#ifndef RaytracedSphereFrame_H
#define RaytracedSphereFrame_H RaytracedSphereFrame_H

#include <shade/Shader.h>
#include <shade/Type.h>
#include <shade/utility.h>
#include <shade/shaders/ICoordinateSystem.h>
#include <shade/shaders/Initializeable.h>
#include <shade/types/objref.h>
#include <shade/types/Varying.h>
#include <shade/types/local.h>
#include <shade/types/vec3.h>
#include <shade/types/vec4.h>
#include <shade/types/void.h>

class RaytracedSphereFrame : public shade::ShaderBase<RaytracedSphereFrame, shade::shaders::ICoordinateSystem, shade::shaders::Initializeable, shade::shaders::Gettable<shade::vec3>>
{
  public:
    /*virtual*/ shade::vec4<> transform(shade::vec4<>);

    /*virtual*/ shade::void_<> init_vertex(void);
    /*virtual*/ shade::void_<> init_fragment(void);

    /*virtual*/ shade::vec3<shade::Type> get(void);

    /*virtual*/ shade::vec3<> get_transformed_normal(void);

    /*virtual*/ shade::vec3<> get_transformed_eyepoint(void);

  private:
    void get_inline(shade::formatter::Generator& generator);

    shade::Varying<shade::vec3> position;
    shade::Varying<shade::vec3> eyepoint;
    shade::Varying<shade::vec3> normal;

    shade::vec3<shade::local> sphere_position;

    SHADE_MULTI_DERIVED_DECL(RaytracedSphereFrame, (shade::shaders::ICoordinateSystem)(shade::shaders::Initializeable)(shade::shaders::Gettable<shade::vec3>))
};

#endif /* RaytracedSphereFrame_H */
