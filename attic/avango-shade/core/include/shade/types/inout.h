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

#ifndef shade_types_inout_H
#define shade_types_inout_H shade_types_inout_H

#include "../Type.h"

namespace shade
{
template <ShaderEnvironment output = invariant_shader, ShaderEnvironment input = invariant_shader>
class inout : public Type
{
  public:
    inout(formatter::Constants::Type type);
    /*virtual*/ bool is_writable(const ShaderEnvironment& se) const;
    /*virtual*/ bool exists(const ShaderEnvironment& se) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const;
    /*virtual*/ bool output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const;
};

typedef inout<invariant_shader, vertex_shader> raw_attribute;
typedef inout<vertex_shader, fragment_shader> raw_varying;
} // namespace shade

#include "impl/inout_impl.cpp"

#endif /* shade_types_inout_H */
