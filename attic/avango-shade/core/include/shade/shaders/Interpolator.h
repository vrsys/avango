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

#ifndef shade_shaders_Interpolator_H
#define shade_shaders_Interpolator_H shade_shaders_Interpolator_H

#include "../Shader.h"
#include "../TemplateT.h"
#include "../ObjectMap.h"
#include "../formatter/Generator.h"
#include "../shaders/Initializeable.h"
#include "../shaders/Geometry.h"
#include "../types/ValueReference.h"
#include "../types/vec3.h"
#include "../types/objref.h"
#include "../types/local.h"
#include "../types/const.h"

namespace shade
{
namespace shaders
{
template <template <class> class T>
class Interpolator : public TemplateT<T, Type, Interpolator, Initializeable>
{
  public:
    /*virtual*/ void_<> init_post_geometry(void);

    ValueReference<T, local> input;
    ValueReference<T, local> output;
    objref<boost::shared_ptr<shaders::Geometry>, shade::const_> geometry;

  private:
    void init_post_geometry_inline(formatter::Generator& generator) const;
    void generate_weighting(formatter::Generator& generator, const std::string& component, int index) const;

    SHADE_TEMPLATE_T_DERIVED_DECL(T, Interpolator, Initializeable)
};

SHADE_TEMPLATE_T_INIT(Interpolator, "", SHADE_NONE, SHADE_DEFS((input)(output)(geometry)))
} // namespace shaders
} // namespace shade

#include "impl/Interpolator_impl.cpp"

#endif /* shade_shaders_Interpolator_H */
