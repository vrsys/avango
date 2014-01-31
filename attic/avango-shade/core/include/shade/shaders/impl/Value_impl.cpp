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

#include <boost/bind.hpp>

template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::get_deferred_getter(formatter::Generator& generator, bool setter) const
{
  std::string function(Value<T, Q>::get_class_name());
  if (setter)
    function += "_set_value";
  else
    function += "_get_value";

  generator.handle_return_begin();
  generator.handle_function_call_begin();
  generator.handle_function_call_name_begin();
  generator.handle_identifier(function);
  generator.handle_function_call_name_end();
  generator.handle_function_call_parameter_list_begin();
  generator.handle_function_call_parameter_begin();
  generator.handle_identifier("self");
  generator.handle_function_call_parameter_end();
  if (setter)
  {
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("arg1");
    generator.handle_function_call_parameter_end();
  }
  generator.handle_function_call_parameter_list_end();
  generator.handle_function_call_end();
  generator.handle_return_end();
}

template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::get_readonly_getter(formatter::Generator& generator) const
{
  generator.handle_return_begin();
  generator.handle_identifier("value");
  generator.handle_return_end();
}

template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::get_default_getter(formatter::Generator& generator, const T<Q>& type) const
{
  generator.handle_return_begin();
  type.generate_constructor(generator);
  generator.handle_return_end();
}

template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::get_getter(formatter::Generator& generator, ShaderEnvironment env, bool setter) const
{
  T<Q> type;
  if (type.exists(env))
  {
    if (type.is_writable(env))
    {
      return get_deferred_getter(generator, setter);
    }
    else
    {
      return get_readonly_getter(generator);
    }
  }
  else
  {
    return get_default_getter(generator, type);
  }
}


template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::get_inline(formatter::Generator& generator) const
{
  generator.handle_environment_begin(vertex_shader);
  get_getter(generator, vertex_shader, false);
  generator.handle_environment_end();
  generator.handle_environment_begin(fragment_shader);
  get_getter(generator, fragment_shader, false);
  generator.handle_environment_end();
  generator.handle_environment_begin(geometry_shader);
  get_getter(generator, geometry_shader, false);
  generator.handle_environment_end();
}

template<template<class> class T, class Q>
void shade::shaders::Value<T, Q>::set_inline(formatter::Generator& generator) const
{
  generator.handle_environment_begin(vertex_shader);
  get_getter(generator, vertex_shader, true);
  generator.handle_environment_end();
  generator.handle_environment_begin(fragment_shader);
  get_getter(generator, fragment_shader, true);
  generator.handle_environment_end();
  generator.handle_environment_begin(geometry_shader);
  get_getter(generator, geometry_shader, true);
  generator.handle_environment_end();
}

template<template<class> class T, class Q>
T<shade::Type> shade::shaders::Value<T, Q>::get(void)
{
  return this->template invoke_inline<T<Type> >(boost::bind(&Value<T, Q>::get_inline, boost::ref(*this), _1));
};

template<template<class> class T, class Q>
T<shade::Type> shade::shaders::Value<T, Q>::set(T<Type> arg)
{
  return this->template invoke_inline<T<Type> >(boost::bind(&Value<T, Q>::set_inline, boost::ref(*this), _1));
};
