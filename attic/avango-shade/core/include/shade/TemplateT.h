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

#ifndef shade_TemplateT_H
#define shade_TemplateT_H shade_TemplateT_H

#include "Shader.h"
#include "Formatter.h"
#include "utility.h"

namespace shade
{
  template
  <
    template<class> class T,
    class Q,
    template<template<class> class> class Derived,
    class Base1 = detail::EmptyShader,
    class Base2 = detail::EmptyShader,
    class Base3 = detail::EmptyShader,
    class Base4 = detail::EmptyShader
  >
  class TemplateT : 
    public ShaderBase<Derived<T>, Base1, Base2, Base3, Base4 >
  {
  public:

    /*virtual*/ std::string get_class_name(void) const
    {
      T<Q> type;
      static std::string name(ShaderBase<Derived<T>, Base1, Base2, Base3, Base4>::get_class_name());
      return name+"_"+type.get_uniq_id();
    }

  private:

    friend class shade::ShaderBase<Derived<T>, Base1, Base2, Base3, Base4>;
  };

}

#endif /* shade_TemplateT_H */
