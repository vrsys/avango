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

#ifndef shade_shaders_Value_H
#define shade_shaders_Value_H shade_shaders_Value_H

#include "../Shader.h"
#include "../TemplateTQ.h"
#include "../shaders/Gettable.h"
#include "../shaders/Settable.h"
#include "../types/local.h"
#include "../formatter/Generator.h"

namespace shade
{
  namespace shaders
  {

    template<template<class> class T, class Q = local> class Value : public TemplateTQ<T, Q, Value, Gettable<T>, Settable<T> >
    {
    public:

      /*virtual*/ T<Type> get(void);
      /*virtual*/ T<Type> set(T<Type> arg);

      T<Q> value;

    private:

      void get_deferred_getter(formatter::Generator& generator, bool setter) const;
      void get_readonly_getter(formatter::Generator& generator) const;
      void get_default_getter(formatter::Generator& generator, const T<Q>& type) const;
      void get_getter(formatter::Generator& generator, ShaderEnvironment env, bool setter) const;

      void get_inline(formatter::Generator& generator) const;
      void set_inline(formatter::Generator& generator) const;

      SHADE_TEMPLATE_T_Q_MULTI_DERIVED_DECL(T, Q, Value, (Gettable<T>)(Settable<T>))
    };

    SHADE_TEMPLATE_T_Q_INIT(Value, "", SHADE_NONE, SHADE_ENV_DEFS(application_stage, (value)))
  }
}

#include "impl/Value_impl.cpp"

#endif /* shade_shaders_Value_H */
