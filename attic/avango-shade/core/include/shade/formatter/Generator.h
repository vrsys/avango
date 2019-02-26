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

#ifndef shade_formatter_Generator
#define shade_formatter_Generator shade_formatter_Generator

#include "../formatter/Constants.h"
#include "../ObjectMap.h"
#include "../ShaderEnvironment.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace shade
{
namespace formatter
{
class Generator
{
  public:
    virtual ~Generator(void) {}

    virtual void handle_identifier(const std::string& name) = 0;
    virtual void handle_verbatim(const std::string& code) = 0;
    virtual void handle_type(Constants::Type type) = 0;
    virtual void handle_qualifier(Constants::Qualifier qualifier) = 0;
    virtual void handle_operator(Constants::Operator op) = 0;

    virtual void handle_literal_objref(ObjectMap::Index ref) = 0;
    virtual void handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end) = 0;
    virtual void handle_literal_int(int value) = 0;
    virtual void handle_literal_float(float value) = 0;
    virtual void handle_literal_vec2(float x, float y) = 0;
    virtual void handle_literal_vec3(float x, float y, float z) = 0;
    virtual void handle_literal_vec4(float x, float y, float z, float w) = 0;

    virtual void handle_array_begin(void) = 0;
    virtual void handle_array_type_begin(void) = 0;
    virtual void handle_array_type_end(void) = 0;
    virtual void handle_array_size_begin(void) = 0;
    virtual void handle_array_size_end(void) = 0;
    virtual void handle_array_end(void) = 0;

    virtual void handle_switch_begin(void) = 0;
    virtual void handle_switch_value_begin(void) = 0;
    virtual void handle_switch_value_end(void) = 0;
    virtual void handle_switch_case_begin(void) = 0;
    virtual void handle_switch_case_end(void) = 0;
    virtual void handle_switch_code_begin(void) = 0;
    virtual void handle_switch_code_end(void) = 0;
    virtual void handle_switch_default_begin(void) = 0;
    virtual void handle_switch_default_end(void) = 0;
    virtual void handle_switch_end(void) = 0;

    virtual void handle_for_begin(void) = 0;
    virtual void handle_for_init_begin(void) = 0;
    virtual void handle_for_init_end(void) = 0;
    virtual void handle_for_condition_begin(void) = 0;
    virtual void handle_for_condition_end(void) = 0;
    virtual void handle_for_increment_begin(void) = 0;
    virtual void handle_for_increment_end(void) = 0;
    virtual void handle_for_code_begin(void) = 0;
    virtual void handle_for_code_end(void) = 0;
    virtual void handle_for_end(void) = 0;

    virtual void handle_var_definition_begin(void) = 0;
    virtual void handle_var_definition_qualifier_list_begin(void) = 0;
    virtual void handle_var_definition_qualifier_begin(void) = 0;
    virtual void handle_var_definition_qualifier_end(void) = 0;
    virtual void handle_var_definition_qualifier_list_end(void) = 0;
    virtual void handle_var_definition_type_begin(void) = 0;
    virtual void handle_var_definition_type_end(void) = 0;
    virtual void handle_var_definition_name_begin(void) = 0;
    virtual void handle_var_definition_name_end(void) = 0;
    virtual void handle_var_definition_code_begin(void) = 0;
    virtual void handle_var_definition_code_end(void) = 0;
    virtual void handle_var_definition_end(void) = 0;

    virtual void handle_statement_begin(void) = 0;
    virtual void handle_statement_end(void) = 0;

    virtual void handle_block_begin(void) = 0;
    virtual void handle_block_end(void) = 0;

    virtual void handle_function_definition_begin(void) = 0;
    virtual void handle_function_definition_return_type_begin(void) = 0;
    virtual void handle_function_definition_return_type_end(void) = 0;
    virtual void handle_function_definition_name_begin(void) = 0;
    virtual void handle_function_definition_name_end(void) = 0;
    virtual void handle_function_definition_parameter_list_begin(void) = 0;
    virtual void handle_function_definition_parameter_qualifier_list_begin(void) = 0;
    virtual void handle_function_definition_parameter_qualifier_begin(void) = 0;
    virtual void handle_function_definition_parameter_qualifier_end(void) = 0;
    virtual void handle_function_definition_parameter_qualifier_list_end(void) = 0;
    virtual void handle_function_definition_parameter_type_begin(void) = 0;
    virtual void handle_function_definition_parameter_type_end(void) = 0;
    virtual void handle_function_definition_parameter_name_begin(void) = 0;
    virtual void handle_function_definition_parameter_name_end(void) = 0;
    virtual void handle_function_definition_parameter_list_end(void) = 0;
    virtual void handle_function_definition_code_begin(void) = 0;
    virtual void handle_function_definition_code_end(void) = 0;
    virtual void handle_function_definition_end(void) = 0;

    virtual void handle_function_call_begin(void) = 0;
    virtual void handle_function_call_name_begin(void) = 0;
    virtual void handle_function_call_name_end(void) = 0;
    virtual void handle_function_call_parameter_list_begin(void) = 0;
    virtual void handle_function_call_parameter_begin(void) = 0;
    virtual void handle_function_call_parameter_end(void) = 0;
    virtual void handle_function_call_parameter_list_end(void) = 0;
    virtual void handle_function_call_end(void) = 0;

    virtual void handle_return_begin(void) = 0;
    virtual void handle_return_end(void) = 0;

    virtual void handle_self_begin(void) = 0;
    virtual void handle_self_name_begin(void) = 0;
    virtual void handle_self_name_end(void) = 0;
    virtual void handle_self_parameter_list_begin(void) = 0;
    virtual void handle_self_parameter_type_begin(void) = 0;
    virtual void handle_self_parameter_type_end(void) = 0;
    virtual void handle_self_parameter_name_begin(void) = 0;
    virtual void handle_self_parameter_name_end(void) = 0;
    virtual void handle_self_parameter_list_end(void) = 0;
    virtual void handle_self_end(void) = 0;

    virtual bool handle_environment_begin(ShaderEnvironment env) = 0;
    virtual void handle_environment_end(void) = 0;

    virtual void append(const Generator&) = 0;
    virtual boost::shared_ptr<Generator> construct(void) const = 0;
};
} // namespace formatter
} // namespace shade

#endif /* shade_formatter_Generator */
