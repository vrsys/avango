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

#include "Scope.h"
#include "Value.h"

using namespace shade::parser;

Scope::Scope(const ScopeLayer& global) :
  m_global(global)
{
}

void shade::parser::Scope::add_value(const std::string& var, boost::shared_ptr<const Value> value)
{
  m_local.add_value(var, value);
}

boost::shared_ptr<const Value> shade::parser::Scope::get_value(const std::string& var) const
{
  boost::shared_ptr<const Value> result(m_local.get_raw_value(var));
  if (result)
    return result;

  return m_global.get_value(var);
}

const ScopeLayer& shade::parser::Scope::get_global(void) const
{
  return m_global;
}

const ScopeLayer& shade::parser::Scope::get_local(void) const
{
  return m_local;
}
