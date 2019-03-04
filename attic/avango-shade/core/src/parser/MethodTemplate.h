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

#ifndef shade_MethodTemplate_H
#define shade_MethodTemplate_H shade_MethodTemplate_H

#include "FunctionCall.h"
#include <string>
#include <iostream>
#include <boost/function.hpp>

namespace shade
{
namespace formatter
{
class Generator;
}

namespace parser
{
class ValueList;

class MethodTemplate
{
  public:
    MethodTemplate(const std::string& name) : m_name(name) {}

    virtual ~MethodTemplate(void) {}

    const std::string& get_name(void) const { return m_name; }

    virtual bool instantiate(const std::string& name, const ValueList& values, formatter::Generator& generator, const ScopeLayer& scope, std::ostream& error_log, FunctionCall call) const = 0;

  private:
    const std::string m_name;
};

} // namespace parser
} // namespace shade

#endif /* shade_MethodTemplate_H */
