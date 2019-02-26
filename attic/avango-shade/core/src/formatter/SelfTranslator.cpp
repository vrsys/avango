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

#include "SelfTranslator.h"
#include <utility>
using namespace shade::formatter;

SelfTranslator::SelfTranslator(void) : m_store_self_name(false), m_store_type(false), m_store_name(false) {}

void SelfTranslator::handle_self_begin(void) {}

void SelfTranslator::handle_self_name_begin(void) { m_store_self_name = true; }

void SelfTranslator::handle_self_name_end(void) { m_store_self_name = false; }

void SelfTranslator::handle_self_parameter_list_begin(void) {}

void SelfTranslator::handle_self_parameter_type_begin(void) { m_store_type = true; }

void SelfTranslator::handle_self_parameter_type_end(void) { m_store_type = false; }

void SelfTranslator::handle_self_parameter_name_begin(void) { m_store_name = true; }

void SelfTranslator::handle_self_parameter_name_end(void)
{
    m_store_name = false;
    m_parameters[m_cur_self_name].push_back(std::make_pair(m_cur_type, m_cur_name));
}

void SelfTranslator::handle_self_parameter_list_end(void) {}

void SelfTranslator::handle_self_end(void) {}

void SelfTranslator::handle_identifier(const std::string& name)
{
    if(m_store_self_name)
        m_cur_self_name = name;

    if(m_store_name)
        m_cur_name = name;
}

void SelfTranslator::handle_type(Constants::Type type)
{
    if(m_store_type)
        m_cur_type = type;
}

void SelfTranslator::generate(Generator& generator, const std::string& name) const
{
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("self");
    generator.handle_function_definition_parameter_name_end();

    std::map<std::string, ParameterList>::const_iterator parameters(m_parameters.find(name));
    if(parameters == m_parameters.end())
        return;

    for(ParameterList::const_iterator param = parameters->second.begin(); param != parameters->second.end(); ++param)
    {
        generator.handle_function_definition_parameter_type_begin();
        generator.handle_type(param->first);
        generator.handle_function_definition_parameter_type_end();
        generator.handle_function_definition_parameter_name_begin();
        generator.handle_identifier(param->second);
        generator.handle_function_definition_parameter_name_end();
    }
}

void SelfTranslator::append(const SelfTranslator& other)
{
    NamedParameterLists::const_iterator end(other.m_parameters.end());
    for(NamedParameterLists::const_iterator l = other.m_parameters.begin(); l != end; ++l)
    {
        m_parameters[l->first] = l->second;
    }
}
