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

#ifndef shade_utility_H
#define shade_utility_H shade_utility_H

#include "ShaderEnvironment.h"

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/tuple/elem.hpp>


/*
 * Utility macros
 */

#define SHADE_NONE
#define SHADE__EMPTY2(a, b)


/*
 * Internal macro for declaration in class
 */

#define SHADE__CLASS_VARS(class_) \
   static const std::string class_name; \
   static const std::string source_name; \
   static shade::MethodAccessor methods[]; \
   static shade::AttributeAccessor attributes[];


/*
 * Internal macros to define method and attribute reference lists
 */

#define SHADE__DEF_FUNC(r, class_, value) \
    ( class_::def(&class_::BOOST_PP_TUPLE_ELEM(2, 0, value), BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, value)), BOOST_PP_TUPLE_ELEM(2, 1, value)) )

#define SHADE__ENUM_DEF(class_, seq) \
    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(SHADE__DEF_FUNC, class_, seq)),

#define SHADE__DEF(type_, var, class_, seq) \
  type_ class_::var[] = \
   { \
    BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq), SHADE__ENUM_DEF, SHADE__EMPTY2)(class_, seq) \
    type_() \
   };

#define SHADE__DEF_METHODS(class_, seq) SHADE__DEF(shade::MethodAccessor, methods, class_, seq)
#define SHADE__DEF_ATTRIBUTES(class_, seq) SHADE__DEF(shade::AttributeAccessor, attributes, class_, seq)

#define SHADE__TEMPLATE_T_Q_DEF(type_, var, class_, seq) \
  template<template<class> class T, class Q> type_ class_<T, Q>::var[] = \
  { \
    BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq), SHADE__ENUM_DEF, SHADE__EMPTY2)(class_, seq) \
    type_() \
  };

#define SHADE__TEMPLATE_T_Q_DEF_METHODS(class_, seq) SHADE__TEMPLATE_T_Q_DEF(shade::MethodAccessor, methods, class_, seq)
#define SHADE__TEMPLATE_T_Q_DEF_ATTRIBUTES(class_, seq) SHADE__TEMPLATE_T_Q_DEF(shade::AttributeAccessor, attributes, class_, seq)

#define SHADE__TEMPLATE_T_DEF(type_, var, class_, seq) \
  template<template<class> class T> type_ class_<T>::var[] = \
  { \
    BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq), SHADE__ENUM_DEF, SHADE__EMPTY2)(class_, seq) \
    type_() \
  };

#define SHADE__TEMPLATE_T_DEF_METHODS(class_, seq) SHADE__TEMPLATE_T_DEF(shade::MethodAccessor, methods, class_, seq)
#define SHADE__TEMPLATE_T_DEF_ATTRIBUTES(class_, seq) SHADE__TEMPLATE_T_DEF(shade::AttributeAccessor, attributes, class_, seq)

#define SHADE__TRANSFORM_SHADER_ENV(d, env, value) (value, env)

/*
 * Macro to be used by users
 */

#define SHADE_BASE_DECL(class_) \
   friend class shade::ShaderBase<class_>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_DERIVED_DECL(class_, base) \
   friend class shade::ShaderBase<class_, base>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_MULTI_DERIVED_DECL(class_, bases) \
   friend class shade::ShaderBase<class_, BOOST_PP_SEQ_ENUM(bases)>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_BASE_DECL(type, class_) \
   friend class shade::ShaderBase<class_<type> >; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_DERIVED_DECL(type, class_, base) \
   friend class shade::ShaderBase<class_<type> , base>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_MULTI_DERIVED_DECL(type, class_, bases) \
   friend class shade::ShaderBase<class_<type> , BOOST_PP_SEQ_ENUM(bases)>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_Q_BASE_DECL(type, qualifier, class_) \
   friend class shade::ShaderBase<class_<type, qualifier> >; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_Q_DERIVED_DECL(type, qualifier, class_, base) \
   friend class shade::ShaderBase<class_<type, qualifier> , base>; \
   SHADE__CLASS_VARS(class_)

#define SHADE_TEMPLATE_T_Q_MULTI_DERIVED_DECL(type, qualifier, class_, bases) \
   friend class shade::ShaderBase<class_<type, qualifier> , BOOST_PP_SEQ_ENUM(bases)>; \
   SHADE__CLASS_VARS(class_)


#define SHADE_CLASS_INIT(class_, file, method_seq, attribute_seq) \
  const std::string class_::class_name(BOOST_PP_STRINGIZE(class_)); \
  const std::string class_::source_name(file); \
  SHADE__DEF_METHODS(class_, method_seq) \
  SHADE__DEF_ATTRIBUTES(class_, attribute_seq)

#define SHADE_TEMPLATE_T_Q_INIT(class_, file, method_seq, attribute_seq) \
  template<template<class> class T, class Q> const std::string class_<T, Q>::class_name(BOOST_PP_STRINGIZE(class_)); \
  template<template<class> class T, class Q> const std::string class_<T, Q>::source_name(file); \
  SHADE__TEMPLATE_T_Q_DEF_METHODS(class_, method_seq) \
  SHADE__TEMPLATE_T_Q_DEF_ATTRIBUTES(class_, attribute_seq)

#define SHADE_TEMPLATE_T_INIT(class_, file, method_seq, attribute_seq) \
  template<template<class> class T> const std::string class_<T>::class_name(BOOST_PP_STRINGIZE(class_)); \
  template<template<class> class T> const std::string class_<T>::source_name(file); \
  SHADE__TEMPLATE_T_DEF_METHODS(class_, method_seq) \
  SHADE__TEMPLATE_T_DEF_ATTRIBUTES(class_, attribute_seq)

#define SHADE_ENV_DEFS(env, list) BOOST_PP_SEQ_TRANSFORM(SHADE__TRANSFORM_SHADER_ENV, env, list)
#define SHADE_DEFS(list) SHADE_ENV_DEFS(shade::invariant_shader, list)

#endif /* shade_utility_H */
