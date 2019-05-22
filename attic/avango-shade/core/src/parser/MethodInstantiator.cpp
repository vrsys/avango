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

#include "MethodInstantiator.h"
#include "ValueList.h"
#include "Value.h"
#include <shade/formatter/Generator.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace shade::parser;

void MethodInstantiator::add_method(boost::shared_ptr<MethodTemplate> method) { m_methods[method->get_name()] = method; }

bool MethodInstantiator::build(formatter::Generator& generator, const ScopeLayer& scope, std::ostream& error_log) const
{
    this->result_generator = &generator;

    m_seen.clear();
    m_instantiations.clear();

    return !call_method("main", ValueList(), scope, error_log).empty();
}

std::string MethodInstantiator::get_unique_function_name(const std::string& name) const
{
    if(m_instantiations.find(name) == m_instantiations.end())
    {
        (m_instantiations[name] = 0);
        return name;
    }

    int count = ++m_instantiations[name];
    return name + boost::lexical_cast<std::string>(count);
}

std::string MethodInstantiator::call_method(const std::string& name, const ValueList& parameters, const ScopeLayer& scope, std::ostream& error_log) const
{
    Methods::const_iterator i = m_methods.find(name);
    if(i == m_methods.end())
    {
        // Unknown methods are considered to be part of the runtime library
        return name;
    }

    std::pair<std::string, ValueList> signature(std::make_pair(name, parameters));
    if(m_seen.find(signature) != m_seen.end())
    {
        error_log << "Method " << name << " called recursively." << std::endl;
        return "";
    }

    m_seen.insert(signature);

    bool instantiate_result(true);
    std::string unique_name;
    InstantiatedMethodNames::const_iterator j = m_instantiated_method_names.find(signature);
    if(j != m_instantiated_method_names.end())
    {
        unique_name = j->second;
    }
    else
    {
        unique_name = get_unique_function_name(name);

        boost::shared_ptr<formatter::Generator> generator(result_generator->construct());
        instantiate_result = i->second->instantiate(unique_name, parameters, *generator, scope, error_log, boost::bind(&MethodInstantiator::call_method, this, _1, _2, _3, _4));
        result_generator->append(*generator);

        if(instantiate_result)
            m_instantiated_method_names[signature] = unique_name;
    }

    m_seen.erase(signature);

    if(!instantiate_result)
        return "";
    return unique_name;
}

void MethodInstantiator::append(const MethodInstantiator& other)
{
    for(Methods::const_iterator m = other.m_methods.begin(); m != other.m_methods.end(); ++m)
    {
        m_methods[m->first] = m->second;
    }
}
