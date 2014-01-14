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

#include <shade/types/local.h>
#include <shade/formatter/Generator.h>
#include <boost/shared_ptr.hpp>


shade::local::local(formatter::Constants::Type type) :
  Type(type)
{
}

void shade::local::output_attribute(boost::shared_ptr<Type::State> state, Formatter* fmt, const std::string& obj, const std::string& name) const
{
  if (fmt->get_shader_env() == geometry_shader)
    fmt->insert_multiple_attribute(m_type, obj, name);
  else
    fmt->insert_attribute(m_type, obj, name);
}

bool shade::local::output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const
{
  if (fmt->get_shader_env() == geometry_shader)
    fmt->begin_multiple_property_dispatcher(obj, name, m_type);
  else
    fmt->begin_property_dispatcher(obj, name, m_type);

  return true;
}
