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

#ifndef shade_types_Varying_H
#define shade_types_Varying_H shade_types_Varying_H

#include "../TypeBase.h"
#include "../shaders/Value.h"
#include "../shaders/Interpolator.h"
#include "inout.h"

namespace shade
{
template <template <class> class T, template <template <class> class> class I = shaders::Interpolator>
class Varying : public TypeBase<Varying<T, I>, Type>
{
  public:
    Varying(void);

    /*virtual*/ boost::shared_ptr<Type::State> create_state(const PipelineState& ps) const;
    /*virtual*/ void collect_references(boost::shared_ptr<Type::State> state, boost::function<void(Shader*)>) const;
    /*virtual*/ void insert_references(boost::shared_ptr<Type::State>, ObjectMap&) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;
    /*virtual*/ void output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const;
    /*virtual*/ bool output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const;
    /*virtual*/ void output_insert_property_dispatcher(boost::shared_ptr<Type::State>, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const;

  private:
    class State : public Type::State
    {
      public:
        State(const PipelineState& ps);

        ObjectMap::Index index;
        ObjectMap::Index vertex_to_geometry_index;
        ObjectMap::Index geometry_to_fragment_index;

        typedef shaders::Value<T, inout<vertex_shader, fragment_shader>> VertexToFragmentValue;
        typedef shaders::Value<T, inout<vertex_shader, geometry_shader>> VertexToGeometryValue;
        typedef shaders::Value<T, inout<geometry_shader, fragment_shader>> GeometryToFragmentValue;
        boost::shared_ptr<VertexToFragmentValue> vertex_to_fragment_value;
        boost::shared_ptr<VertexToGeometryValue> vertex_to_geometry_value;
        boost::shared_ptr<GeometryToFragmentValue> geometry_to_fragment_value;
        boost::shared_ptr<I<T>> interpolator_value;
    };
};
} // namespace shade

#include "impl/Varying_impl.cpp"

#endif /* shade_types_Varying_H */
