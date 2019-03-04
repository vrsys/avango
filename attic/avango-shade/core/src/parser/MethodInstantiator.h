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

#ifndef shade_MethodInstantiator_H
#define shade_MethodInstantiator_H shade_MethodInstantiator_H

#include "MethodTemplate.h"
#include "ValueList.h"
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <boost/function.hpp>

namespace shade
{
namespace formatter
{
class Generator;
}

namespace parser
{
class ScopeLayer;

class MethodInstantiator
{
  public:
    void add_method(boost::shared_ptr<MethodTemplate> method);
    void append(const MethodInstantiator& other);
    bool build(formatter::Generator& generator, const ScopeLayer& scope, std::ostream& error_log) const;

  private:
    std::string call_method(const std::string& name, const ValueList& parameters, const ScopeLayer& scope, std::ostream& error_log) const;
    std::string get_unique_function_name(const std::string& name) const;

    typedef std::map<std::string, boost::shared_ptr<MethodTemplate>> Methods;
    Methods m_methods;

    typedef std::pair<std::string, ValueList> Signature;
    typedef std::map<Signature, std::string> InstantiatedMethodNames;
    mutable InstantiatedMethodNames m_instantiated_method_names;

    mutable formatter::Generator* result_generator;
    mutable std::set<Signature> m_seen;
    mutable std::map<std::string, int> m_instantiations;
};

} // namespace parser
} // namespace shade

#endif /* shade_MethodInstantiator_H */
