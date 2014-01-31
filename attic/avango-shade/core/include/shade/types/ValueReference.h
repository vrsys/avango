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

#ifndef shade_types_ValueReference_H
#define shade_types_ValueReference_H shade_types_ValueReference_H

#include "../TypeBase.h"
#include "ObjrefAccessor.h"
#include <boost/shared_ptr.hpp>


namespace shade
{
  template<template<class> class T, class Q, class P = boost::shared_ptr<Shader> >
    class ValueReferenceBase : public TypeBase<ValueReferenceBase<T, Q, P>, Type>
  {
  public:
    ValueReferenceBase(void);

    P operator->(void);
    Shader* operator*(void);
    P get(void) const;
    P& operator=(const P& v);

    class NoValueAsReference {};
    T<Type> get_value(void);
    template<class Q2> const T<Q2>& set_value(const T<Q2>& arg);

    /*virtual*/ boost::shared_ptr<Type::State> create_state(const PipelineState& ps) const;
    /*virtual*/ void collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)>) const;
    /*virtual*/ void insert_references(boost::shared_ptr<Type::State>, ObjectMap&) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;
    /*virtual*/ void output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const;
    /*virtual*/ bool output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const;
    /*virtual*/ void output_insert_property_dispatcher(boost::shared_ptr<Type::State>, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const;

    class State : public Type::State
    {
    public:
      ObjectMap::Index index;
    };

  private:
    P m_ref;
  };

  template<template<class> class T, class Q, class P = boost::shared_ptr<Shader> >
  class ValueReference : public ValueReferenceBase<T, Q, P>
  {
  public:
    using ValueReferenceBase<T, Q, P>::operator->;
    using ValueReferenceBase<T, Q, P>::operator*;
    using ValueReferenceBase<T, Q, P>::operator=;
  };

  template<template<class> class T, class Q>
  class ValueReference<T, Q, boost::shared_ptr<Shader> > :
    public ValueReferenceBase<T, Q, boost::shared_ptr<Shader> >,
    public types::ObjrefAccessor
  {
  public:
    using ValueReferenceBase<T, Q, boost::shared_ptr<Shader> >::operator->;
    using ValueReferenceBase<T, Q, boost::shared_ptr<Shader> >::operator*;
    using ValueReferenceBase<T, Q, boost::shared_ptr<Shader> >::operator=;

    /*virtual*/ void set_generic(boost::shared_ptr<Shader> v);
    /*virtual*/ boost::shared_ptr<Shader> get_generic(void) const;
  };
}

#include "impl/ValueReference_impl.cpp"

#endif /* shade_types_ValueReference_H */
