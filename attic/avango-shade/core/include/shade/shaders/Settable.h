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

#ifndef shade_shaders_Settable_H
#define shade_shaders_Settable_H shade_shaders_Settable_H

#include "../Shader.h"
#include "../TemplateT.h"
#include "../Type.h"

namespace shade
{
namespace shaders
{
template <template <class> class T>
class Settable : public TemplateT<T, Type, Settable>
{
  public:
    virtual T<Type> set(T<Type> arg) { return T<Type>(); };

  private:
    SHADE_TEMPLATE_T_BASE_DECL(T, Settable)
};

SHADE_TEMPLATE_T_INIT(Settable, "", SHADE_DEFS((set)), SHADE_NONE)
} // namespace shaders
} // namespace shade

#endif /* shade_shaders_Settable_H */
