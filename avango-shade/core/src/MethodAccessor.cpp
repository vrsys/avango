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

#include <shade/MethodAccessor.h>

shade::MethodAccessor::MethodAccessor(void) :
  m_name("")
{
}

shade::MethodAccessor::MethodAccessor(boost::shared_ptr<MethodArgumentList> args, boost::shared_ptr<MethodCaller> caller, std::string name, ShaderEnvironment env) :
  m_args(args),
  m_caller(caller),
  m_name(name),
  m_env(env)
{
}


const std::string& shade::MethodAccessor::get_name(void) const
{
  return m_name;
}

shade::ShaderEnvironment shade::MethodAccessor::get_env(void) const
{
  return m_env;
}

shade::MethodArgumentIterator shade::MethodAccessor::type_begin(void) const
{
  MethodArgumentIterator iter(*m_args);
  iter.check();
  return iter;
}

shade::MethodArgumentIterator shade::MethodAccessor::type_end(void) const
{
  MethodArgumentIterator iter(*m_args);
  iter.seek_end();
  iter.check();
  return iter;
}

boost::shared_ptr<shade::Type> shade::MethodAccessor::invoke(Shader* obj) const
{
  return boost::shared_ptr<Type>(m_caller->invoke(obj));
}


shade::MethodArgumentIterator::MethodArgumentIterator(const MethodArgumentList& args) :
  m_args(args),
  m_pos(0)
{
}

void shade::MethodArgumentIterator::seek_end(void)
{
  m_pos = m_args.size();
}

void shade::MethodArgumentIterator::check(void)
{
  m_type = boost::shared_ptr<Type>(m_args.create_type(m_pos));
}

shade::MethodArgumentIterator& shade::MethodArgumentIterator::operator++(void)
{
  ++m_pos;
  check();
  return *this;
}

const shade::Type& shade::MethodArgumentIterator::operator*(void) const
{
  return *m_type;
}

shade::Type& shade::MethodArgumentIterator::operator*(void)
{
  return *m_type;
}

bool shade::MethodArgumentIterator::operator==(const MethodArgumentIterator& a)
{
  return (m_pos == a.m_pos);
}

bool shade::MethodArgumentIterator::operator!=(const MethodArgumentIterator& a)
{
  return !(*this == a);
}
