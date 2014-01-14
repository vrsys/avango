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

#ifndef shade_shaders_PerStageGettable_H
#define shade_shaders_PerStageGettable_H shade_shaders_PerStageGettable_H

#include "../TemplateT.h"
#include "../Type.h"
#include "../shaders/Gettable.h"
#include "../utility.h"
#include "../formatter/Generator.h"

namespace shade
{
  namespace shaders
  {
    template<template<class> class T> class PerStageGettable :
      public TemplateT<T, Type, PerStageGettable, Gettable<T> >
    {
    protected:

      /*virtual*/ T<Type> get(void);

      // TODO make methods const (requires new 'def' methods in ShaderBase)
      virtual T<Type> get_vertex(void);
      virtual T<Type> get_fragment(void);
      virtual T<Type> get_geometry(void);

    private:

      void get_inline(formatter::Generator&);
      void get_default_inline(formatter::Generator&);

      SHADE_TEMPLATE_T_DERIVED_DECL(T, PerStageGettable, Gettable<T>)
    };
  }
}

#include "impl/PerStageGettable_impl.cpp"

#endif /* shade_shaders_PerStageGettable_H */
