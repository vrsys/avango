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

#include <shade/Type.h>


shade::Type::Type(formatter::Constants::Type type) :
  m_type(type),
  m_invoke_is_inline(false)
{
}

shade::Type::~Type(void)
{
}

shade::Type::Type(const Type& t)
{
  m_type = t.m_type;
  m_invoke = t.m_invoke;
  m_invoke_generator = t.m_invoke_generator;
  m_invoke_is_inline = t.m_invoke_is_inline;
  m_invoke_ti = t.m_invoke_ti;
}

shade::Type& shade::Type::operator=(const Type& t)
{
  if (&t != this)
  {
    m_type = t.m_type;
    m_invoke = t.m_invoke;
    m_invoke_generator = t.m_invoke_generator;
    m_invoke_is_inline = t.m_invoke_is_inline;
    m_invoke_ti = t.m_invoke_ti;
  }
  return *this;
}

boost::shared_ptr<shade::Type::State> shade::Type::create_state(const PipelineState& ps) const
{
  return boost::shared_ptr<State>(new State);
}

std::string shade::Type::get_constructor_str(void) const
{
  return "";
}

std::string shade::Type::get_constructor_str(boost::shared_ptr<State>) const
{
  return "";
}

shade::formatter::Constants::Type shade::Type::get_formatter_type(void) const
{
  return m_type;
}

bool shade::Type::requires_initializer(void) const
{
  return false;
}

bool shade::Type::is_writable(const ShaderEnvironment& se) const
{
  return true;
}

bool shade::Type::exists(const ShaderEnvironment& se) const
{
  return true;
}

std::string shade::Type::get_function_name(void) const
{
  return m_invoke;
}

const std::type_info* shade::Type::get_function_type(void) const
{
  return m_invoke_ti;
}

void shade::Type::invoke(const std::string& func, const std::type_info* ti)
{
  m_invoke = func;
  m_invoke_ti = ti;
}

void shade::Type::invoke_inline(boost::function<void (formatter::Generator&)> generator, const std::type_info* ti)
{
  m_invoke_generator = generator;
  m_invoke_is_inline = true;
  m_invoke_ti = ti;
}

void shade::Type::generate_inline(formatter::Generator& generator) const
{
  m_invoke_generator(generator);
}

bool shade::Type::has_function(void) const
{
  return (!m_invoke.empty() || has_inline_function());
}

bool shade::Type::has_inline_function(void) const
{
  return m_invoke_is_inline;
}

void shade::Type::collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)>) const
{
  // noop
}

void shade::Type::insert_references(boost::shared_ptr<State>, ObjectMap&) const
{
  // noop
}

shade::Type::connection shade::Type::register_notify(signal::slot_function_type modified) const
{
  return m_modified_cb.connect(modified);
}

void shade::Type::touch(void)
{
  m_modified_cb(this);
}

shade::Type::LinkIndex shade::Type::get_link_index(boost::shared_ptr<GLSLWrapper>, const std::string& name) const
{
  return 0;
}

void shade::Type::upload(boost::shared_ptr<GLSLWrapper>, LinkIndex) const
{
}

void shade::Type::upload_uniform(boost::shared_ptr<GLSLWrapper>, LinkIndex) const
{
}

void shade::Type::output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const
{
  fmt->insert_attribute(m_type, obj, name);
}

bool shade::Type::output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const
{
  return false;
}

void shade::Type::output_insert_property_dispatcher(boost::shared_ptr<Type::State> state, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const
{
  fmt->insert_property_dispatcher(index, obj);
}

std::string shade::Type::get_uniq_id(void) const
{
  return "";
}

void shade::Type::generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const
{
  generate_constructor(generator);
}

void shade::Type::generate_constructor(formatter::Generator& generator) const
{
  /* noop */
}

std::string shade::Type::join_names(const std::string& a, const std::string& b) const
{
  if (a.empty())
    return b;
  else if (b.empty())
    return a;

  return a+"_"+b;
}
