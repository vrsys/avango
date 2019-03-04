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

#include "ForeachSegment.h"
#include <shade/formatter/Generator.h>

using namespace shade::parser;

ForeachSegment::ForeachSegment(
    const std::string& index, const std::string& list, boost::shared_ptr<Segment> body, const formatter::MarkBuffer::iterator& begin, const formatter::MarkBuffer::iterator& end)
    : Segment(begin, end), m_index(index), m_list(list), m_body(body)
{
}

void ForeachSegment::get_content(formatter::Generator& generator, Scope& scope, const FunctionCall& call, std::ostream& error_log) const
{
    generator.handle_for_begin();
    generator.handle_for_init_begin();
    {
        generator.handle_var_definition_begin();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::objref);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier(m_index);
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_code_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_list_begin");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier(m_list);
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_var_definition_code_end();
        generator.handle_var_definition_end();
    }
    generator.handle_for_init_end();
    generator.handle_for_condition_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::not_equal);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(m_index);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_list_end");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier(m_list);
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_for_condition_end();
    generator.handle_for_increment_begin();
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::pre_increment);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(m_index);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
    }
    generator.handle_for_increment_end();
    generator.handle_for_code_begin();
    {
        m_body->get_content(generator, scope, call, error_log);
    }
    generator.handle_for_code_end();
    generator.handle_for_end();
}
