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

#ifndef shade_formatter_GL2Generator
#define shade_formatter_GL2Generator shade_formatter_GL2Generator

#include <shade/formatter/Generator.h>
#include <shade/ShaderEnvironment.h>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

namespace shade
{
namespace formatter
{
class GL2Generator : public Generator
{
  public:
    GL2Generator(ShaderEnvironment env);

    std::string get_code(void) const;

    /*virtual*/ void handle_identifier(const std::string& name);
    /*virtual*/ void handle_verbatim(const std::string& code);
    /*virtual*/ void handle_type(Constants::Type type);
    /*virtual*/ void handle_qualifier(Constants::Qualifier qualifier);
    /*virtual*/ void handle_operator(Constants::Operator op);

    /*virtual*/ void handle_literal_objref(ObjectMap::Index ref);
    /*virtual*/ void handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end);
    /*virtual*/ void handle_literal_int(int value);
    /*virtual*/ void handle_literal_float(float value);
    /*virtual*/ void handle_literal_vec2(float x, float y);
    /*virtual*/ void handle_literal_vec3(float x, float y, float z);
    /*virtual*/ void handle_literal_vec4(float x, float y, float z, float w);

    /*virtual*/ void handle_array_begin(void);
    /*virtual*/ void handle_array_type_begin(void);
    /*virtual*/ void handle_array_type_end(void);
    /*virtual*/ void handle_array_size_begin(void);
    /*virtual*/ void handle_array_size_end(void);
    /*virtual*/ void handle_array_end(void);

    /*virtual*/ void handle_switch_begin(void);
    /*virtual*/ void handle_switch_value_begin(void);
    /*virtual*/ void handle_switch_value_end(void);
    /*virtual*/ void handle_switch_case_begin(void);
    /*virtual*/ void handle_switch_case_end(void);
    /*virtual*/ void handle_switch_code_begin(void);
    /*virtual*/ void handle_switch_code_end(void);
    /*virtual*/ void handle_switch_default_begin(void);
    /*virtual*/ void handle_switch_default_end(void);
    /*virtual*/ void handle_switch_end(void);

    /*virtual*/ void handle_for_begin(void);
    /*virtual*/ void handle_for_init_begin(void);
    /*virtual*/ void handle_for_init_end(void);
    /*virtual*/ void handle_for_condition_begin(void);
    /*virtual*/ void handle_for_condition_end(void);
    /*virtual*/ void handle_for_increment_begin(void);
    /*virtual*/ void handle_for_increment_end(void);
    /*virtual*/ void handle_for_code_begin(void);
    /*virtual*/ void handle_for_code_end(void);
    /*virtual*/ void handle_for_end(void);

    /*virtual*/ void handle_var_definition_begin(void);
    /*virtual*/ void handle_var_definition_qualifier_list_begin(void);
    /*virtual*/ void handle_var_definition_qualifier_begin(void);
    /*virtual*/ void handle_var_definition_qualifier_end(void);
    /*virtual*/ void handle_var_definition_qualifier_list_end(void);
    /*virtual*/ void handle_var_definition_type_begin(void);
    /*virtual*/ void handle_var_definition_type_end(void);
    /*virtual*/ void handle_var_definition_name_begin(void);
    /*virtual*/ void handle_var_definition_name_end(void);
    /*virtual*/ void handle_var_definition_code_begin(void);
    /*virtual*/ void handle_var_definition_code_end(void);
    /*virtual*/ void handle_var_definition_end(void);

    /*virtual*/ void handle_statement_begin(void);
    /*virtual*/ void handle_statement_end(void);

    /*virtual*/ void handle_block_begin(void);
    /*virtual*/ void handle_block_end(void);

    /*virtual*/ void handle_function_definition_begin(void);
    /*virtual*/ void handle_function_definition_return_type_begin(void);
    /*virtual*/ void handle_function_definition_return_type_end(void);
    /*virtual*/ void handle_function_definition_name_begin(void);
    /*virtual*/ void handle_function_definition_name_end(void);
    /*virtual*/ void handle_function_definition_parameter_list_begin(void);
    /*virtual*/ void handle_function_definition_parameter_qualifier_list_begin(void);
    /*virtual*/ void handle_function_definition_parameter_qualifier_begin(void);
    /*virtual*/ void handle_function_definition_parameter_qualifier_end(void);
    /*virtual*/ void handle_function_definition_parameter_qualifier_list_end(void);
    /*virtual*/ void handle_function_definition_parameter_type_begin(void);
    /*virtual*/ void handle_function_definition_parameter_type_end(void);
    /*virtual*/ void handle_function_definition_parameter_name_begin(void);
    /*virtual*/ void handle_function_definition_parameter_name_end(void);
    /*virtual*/ void handle_function_definition_parameter_list_end(void);
    /*virtual*/ void handle_function_definition_code_begin(void);
    /*virtual*/ void handle_function_definition_code_end(void);
    /*virtual*/ void handle_function_definition_end(void);

    /*virtual*/ void handle_function_call_begin(void);
    /*virtual*/ void handle_function_call_name_begin(void);
    /*virtual*/ void handle_function_call_name_end(void);
    /*virtual*/ void handle_function_call_parameter_list_begin(void);
    /*virtual*/ void handle_function_call_parameter_begin(void);
    /*virtual*/ void handle_function_call_parameter_end(void);
    /*virtual*/ void handle_function_call_parameter_list_end(void);
    /*virtual*/ void handle_function_call_end(void);

    /*virtual*/ void handle_return_begin(void);
    /*virtual*/ void handle_return_end(void);

    /*virtual*/ void handle_self_begin(void);
    /*virtual*/ void handle_self_name_begin(void);
    /*virtual*/ void handle_self_name_end(void);
    /*virtual*/ void handle_self_parameter_list_begin(void);
    /*virtual*/ void handle_self_parameter_type_begin(void);
    /*virtual*/ void handle_self_parameter_type_end(void);
    /*virtual*/ void handle_self_parameter_name_begin(void);
    /*virtual*/ void handle_self_parameter_name_end(void);
    /*virtual*/ void handle_self_parameter_list_end(void);
    /*virtual*/ void handle_self_end(void);

    /*virtual*/ bool handle_environment_begin(ShaderEnvironment env);
    /*virtual*/ void handle_environment_end(void);

    /*virtual*/ void append(const Generator&);
    /*virtual*/ boost::shared_ptr<Generator> construct(void) const;

    struct Extension
    {
        enum Behavior
        {
            require,
            enable,
            warn,
            disable
        };
    };

    void insert_define(const std::string& name, const std::string& value);
    void insert_extension(const std::string& name, Extension::Behavior behavior);
    void insert_sl_version(int version);
    void insert_type_definitions(void);

  private:
    std::ostringstream& indent(void);

    ShaderEnvironment m_env;
    bool m_switch_needs_else;
    bool m_function_has_code;
    bool m_function_parameter_type_set;
    bool m_function_parameter_needs_separator;
    bool m_identifier_set;
    Constants::Operator m_operator;
    std::vector<std::pair<std::string, std::string>> m_function_parameter_parts;
    int m_indentation_level;

    bool m_type_is_array;
    std::string m_array_type;
    std::string m_array_size;

    std::ostringstream m_code;
    std::vector<std::string> m_diverted_outputs;

    bool m_environment_matches;
};
} // namespace formatter
} // namespace shade

#endif /* shade_formatter_GL2Generator */
