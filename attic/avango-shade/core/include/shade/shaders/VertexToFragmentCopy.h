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

#ifndef shade_shaders_VertexToFragmentCopy_H
#define shade_shaders_VertexToFragmentCopy_H shade_shaders_VertexToFragmentCopy_H

#include "../Shader.h"
#include "../Type.h"
#include "../TemplateT.h"
#include "../shaders/Gettable.h"
#include "../shaders/PerStageGettable.h"
#include "../shaders/Initializeable.h"
#include "../formatter/Generator.h"
#include "../types/objref.h"
#include "../types/void.h"
#include "../types/const.h"

namespace shade
{
namespace shaders
{
template <template <class> class T>
class VertexToFragmentCopy : public TemplateT<T, Type, VertexToFragmentCopy, Initializeable, PerStageGettable<T>>
{
  public:
    /*virtual*/ void_<> init_vertex(void);
    /*virtual*/ T<Type> get_fragment(void);

    objref<boost::shared_ptr<Gettable<T>>, const_> value;

  private:
    Varying<T> copy;

    void get_fragment_inline(formatter::Generator& generator) const;
    void init_vertex_inline(formatter::Generator& generator) const;

    SHADE_TEMPLATE_T_MULTI_DERIVED_DECL(T, VertexToFragmentCopy, (Initializeable)(PerStageGettable<T>))
};
} // namespace shaders
} // namespace shade

#include "impl/VertexToFragmentCopy_impl.cpp"

#endif /* shade_shaders_VertexToFragmentCopy_H */
