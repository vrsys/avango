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

#include "../../shaders/Value.h"
#include "../../shaders/Surface.h"


template<template<class> class T, template<template<class> class> class I>
shade::Varying<T, I>::State::State(const PipelineState& ps)
{
  shaders::Surface* root = dynamic_cast<shaders::Surface*>(ps.get_root_shader().get());
  if ((root) && (root->geometry))
  {
    vertex_to_geometry_value = boost::shared_ptr<VertexToGeometryValue>(new VertexToGeometryValue);
    geometry_to_fragment_value = boost::shared_ptr<GeometryToFragmentValue>(new GeometryToFragmentValue);
    interpolator_value = boost::shared_ptr<I<T> >(new I<T>);
    interpolator_value->input = vertex_to_geometry_value;
    interpolator_value->output = geometry_to_fragment_value;
    interpolator_value->geometry = root->geometry.get();
  }
  else
  {
    vertex_to_fragment_value = boost::shared_ptr<VertexToFragmentValue>(new VertexToFragmentValue);
  }
}


template<template<class> class T, template<template<class> class> class I>
shade::Varying<T, I>::Varying(void) :
  TypeBase<Varying<T, I>, Type>(T<Type>().get_formatter_type())
{
}

template<template<class> class T, template<template<class> class> class I>
boost::shared_ptr<shade::Type::State> shade::Varying<T, I>::create_state(const PipelineState& ps) const
{
  return boost::shared_ptr<State>(new State(ps));
}

template<template<class> class T, template<template<class> class> class I>
void shade::Varying<T, I>::collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)> cb) const
{
  State* st = dynamic_cast<State*>(state.get());
  if (st->vertex_to_fragment_value)
    cb(st->vertex_to_fragment_value.get());
  else
  {
    cb(st->vertex_to_geometry_value.get());
    cb(st->geometry_to_fragment_value.get());
    cb(st->interpolator_value.get());
  }
}

template<template<class> class T, template<template<class> class> class I>
void shade::Varying<T, I>::insert_references(boost::shared_ptr<Type::State> state, ObjectMap& objects) const
{
  State* st = dynamic_cast<State*>(state.get());
  if (st->vertex_to_fragment_value)
  {
    st->index = objects.get_next_index();
    objects.add_object(st->vertex_to_fragment_value.get());
  }
  else
  {
    st->vertex_to_geometry_index = objects.get_next_index();
    objects.add_object(st->vertex_to_geometry_value.get());
    st->geometry_to_fragment_index = objects.get_next_index();
    objects.add_object(st->geometry_to_fragment_value.get());
    objects.add_object(st->interpolator_value.get());
  }
}

template<template<class> class T, template<template<class> class> class I>
std::string shade::Varying<T, I>::get_constructor_str(void) const
{
  return T<Type>().get_constructor_str();
}

template<template<class> class T, template<template<class> class> class I>
void shade::Varying<T, I>::output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const
{
  /* Do nothing */
}

template<template<class> class T, template<template<class> class> class I>
bool shade::Varying<T, I>::output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const
{
  fmt->begin_deferred_property_dispatcher(obj, name, this->get_formatter_type());
  return true;
}

template<template<class> class T, template<template<class> class> class I>
void shade::Varying<T, I>::output_insert_property_dispatcher(boost::shared_ptr<Type::State> state, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const
{
  State* st = dynamic_cast<State*>(state.get());
  std::string getter_class;
  std::string setter_class;
  ObjectMap::Index deferred_index(0); // silence compiler warning

  if (st->vertex_to_fragment_value)
  {
    deferred_index = st->index;
    getter_class = st->vertex_to_fragment_value->shade::shaders::Gettable<T>::get_class_name();
    setter_class = st->vertex_to_fragment_value->shade::shaders::Settable<T>::get_class_name();
  }
  else
  {
    if (fmt->get_shader_env() == vertex_shader)
    {
      deferred_index = st->vertex_to_geometry_index;
      getter_class = st->vertex_to_geometry_value->shade::shaders::Gettable<T>::get_class_name();
      setter_class = st->vertex_to_geometry_value->shade::shaders::Settable<T>::get_class_name();
    }
    else if (fmt->get_shader_env() == fragment_shader)
    {
      deferred_index = st->geometry_to_fragment_index;
      getter_class = st->geometry_to_fragment_value->shade::shaders::Gettable<T>::get_class_name();
      setter_class = st->geometry_to_fragment_value->shade::shaders::Settable<T>::get_class_name();
    }
  }

  if (!getter_class.empty())
    fmt->insert_deferred_property_dispatcher(index, deferred_index, getter_class, "get", setter_class, "set");
}

template<template<class> class T, template<template<class> class> class I>
std::string shade::Varying<T, I>::get_uniq_id(void) const
{
  T<Type> type;
  return join_names("Varying", type.get_uniq_id());
}

template<template<class> class T, template<template<class> class> class I>
void shade::Varying<T, I>::generate_constructor(formatter::Generator& generator) const
{
  return T<Type>().generate_constructor(generator);
}
