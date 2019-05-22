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

#include "DumpGenerator.h"
using namespace shade::formatter;

std::string DumpGenerator::get_code(void) const { return m_code.str(); }

void DumpGenerator::handle_identifier(const std::string& name) { m_code << "<id>" << name << "</id>"; }

void DumpGenerator::handle_verbatim(const std::string& code) { m_code << "<verbatim>" << code << "</verbatim>"; }

void DumpGenerator::handle_type(Constants::Type type)
{
    static const char* type_names[] = {"float", "vec2", "vec3", "vec4", "mat4", "sampler2D", "sampler2DShadow", "sampler2DArray", "usampler2D", "samplerCube", "OBJREF", "LIST", "void", "int"};

    m_code << "<type>" << type_names[type] << "</type>";
}

void DumpGenerator::handle_qualifier(Constants::Qualifier qualifier)
{
    static const char* qualifier_names[] = {"none", "uniform", "const", "input", "output"};

    m_code << "<qualifier>" << qualifier_names[qualifier] << "</qualifier>";
}

void DumpGenerator::handle_operator(Constants::Operator op)
{
    static const char* operator_names[] = {
        "pre_increment", "post_increment", "equal", "not_equal", "assignment", "array_access", "component_access", "multiply", "divide", "add", "subtract", "assignment_add"};

    m_code << "<operator>" << operator_names[op] << "</operator>";
}

void DumpGenerator::handle_literal_objref(ObjectMap::Index ref) { m_code << "<literal_objref>" << ref << "</literal_objref>"; }

void DumpGenerator::handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end) { m_code << "<literal_list>" << begin << " " << end << "</literal_list>"; }

void DumpGenerator::handle_literal_int(int value) { m_code << "<literal_int>" << value << "</literal_int>"; }

void DumpGenerator::handle_literal_float(float value) { m_code << "<literal_float>" << value << "</literal_float>"; }

void DumpGenerator::handle_literal_vec2(float x, float y) { m_code << "<literal_vec2>" << x << " " << y << "</literal_vec2>"; }

void DumpGenerator::handle_literal_vec3(float x, float y, float z) { m_code << "<literal_vec3>" << x << " " << y << " " << z << "</literal_vec3>"; }

void DumpGenerator::handle_literal_vec4(float x, float y, float z, float w) { m_code << "<literal_vec4>" << x << " " << y << " " << z << " " << w << "</literal_vec4>"; }

void DumpGenerator::handle_array_begin(void) { m_code << "<array>"; }

void DumpGenerator::handle_array_type_begin(void) { m_code << "<array_type>"; }

void DumpGenerator::handle_array_type_end(void) { m_code << "</array_type>"; }

void DumpGenerator::handle_array_size_begin(void) { m_code << "<array_size>"; }

void DumpGenerator::handle_array_size_end(void) { m_code << "</array_size>"; }

void DumpGenerator::handle_array_end(void) { m_code << "</array>"; }

void DumpGenerator::handle_switch_begin(void) { m_code << "<switch>"; }

void DumpGenerator::handle_switch_value_begin(void) { m_code << "<switch_value>"; }

void DumpGenerator::handle_switch_value_end(void) { m_code << "</switch_value>"; }

void DumpGenerator::handle_switch_case_begin(void) { m_code << "<switch_case>"; }

void DumpGenerator::handle_switch_case_end(void) { m_code << "</switch_case>"; }

void DumpGenerator::handle_switch_code_begin(void) { m_code << "<switch_code>"; }

void DumpGenerator::handle_switch_code_end(void) { m_code << "</switch_code>"; }

void DumpGenerator::handle_switch_default_begin(void) { m_code << "<switch_default>"; }

void DumpGenerator::handle_switch_default_end(void) { m_code << "</switch_default>"; }

void DumpGenerator::handle_switch_end(void) { m_code << "</switch>"; }

void DumpGenerator::handle_for_begin(void) { m_code << "<for>"; }

void DumpGenerator::handle_for_init_begin(void) { m_code << "<for_init>"; }

void DumpGenerator::handle_for_init_end(void) { m_code << "</for_init>"; }

void DumpGenerator::handle_for_condition_begin(void) { m_code << "<for_condition>"; }

void DumpGenerator::handle_for_condition_end(void) { m_code << "</for_condition>"; }

void DumpGenerator::handle_for_increment_begin(void) { m_code << "<for_increment>"; }

void DumpGenerator::handle_for_increment_end(void) { m_code << "</for_increment>"; }

void DumpGenerator::handle_for_code_begin(void) { m_code << "<for_code>"; }

void DumpGenerator::handle_for_code_end(void) { m_code << "</for_code>"; }

void DumpGenerator::handle_for_end(void) { m_code << "</for>"; }

void DumpGenerator::handle_var_definition_begin(void) { m_code << "<vardef>"; }

void DumpGenerator::handle_var_definition_qualifier_list_begin(void) { m_code << "<vardef_qualifier_list>"; }

void DumpGenerator::handle_var_definition_qualifier_begin(void) { m_code << "<vardef_qualifier>"; }

void DumpGenerator::handle_var_definition_qualifier_end(void) { m_code << "</vardef_qualifier>"; }

void DumpGenerator::handle_var_definition_qualifier_list_end(void) { m_code << "</vardef_qualifier_list>"; }

void DumpGenerator::handle_var_definition_type_begin(void) { m_code << "<vardef_type>"; }

void DumpGenerator::handle_var_definition_type_end(void) { m_code << "</vardef_type>"; }

void DumpGenerator::handle_var_definition_name_begin(void) { m_code << "<vardef_name>"; }

void DumpGenerator::handle_var_definition_name_end(void) { m_code << "</vardef_name>"; }

void DumpGenerator::handle_var_definition_code_begin(void) { m_code << "<vardef_code>"; }

void DumpGenerator::handle_var_definition_code_end(void) { m_code << "</vardef_code>"; }

void DumpGenerator::handle_var_definition_end(void) { m_code << "</vardef>"; }

void DumpGenerator::handle_statement_begin(void) { m_code << "<statement>"; }

void DumpGenerator::handle_statement_end(void) { m_code << "</statement>"; }

void DumpGenerator::handle_block_begin(void) { m_code << "<block>"; }

void DumpGenerator::handle_block_end(void) { m_code << "</block>"; }

void DumpGenerator::handle_function_definition_begin(void) { m_code << "<funcdef>"; }

void DumpGenerator::handle_function_definition_return_type_begin(void) { m_code << "<funcdef_type>"; }

void DumpGenerator::handle_function_definition_return_type_end(void) { m_code << "</funcdef_type>"; }

void DumpGenerator::handle_function_definition_name_begin(void) { m_code << "<funcdef_name>"; }

void DumpGenerator::handle_function_definition_name_end(void) { m_code << "</funcdef_name>"; }

void DumpGenerator::handle_function_definition_parameter_list_begin(void) { m_code << "<funcdef_param_list>"; }

void DumpGenerator::handle_function_definition_parameter_qualifier_list_begin(void) { m_code << "<funcdef_param_qualifier_list>"; }

void DumpGenerator::handle_function_definition_parameter_qualifier_begin(void) { m_code << "<funcdef_param_qualifier>"; }

void DumpGenerator::handle_function_definition_parameter_qualifier_end(void) { m_code << "</funcdef_param_qualifier>"; }

void DumpGenerator::handle_function_definition_parameter_qualifier_list_end(void) { m_code << "</funcdef_param_qualifier_list>"; }

void DumpGenerator::handle_function_definition_parameter_type_begin(void) { m_code << "<funcdef_param_type>"; }

void DumpGenerator::handle_function_definition_parameter_type_end(void) { m_code << "</funcdef_param_type>"; }

void DumpGenerator::handle_function_definition_parameter_name_begin(void) { m_code << "<funcdef_param_name>"; }

void DumpGenerator::handle_function_definition_parameter_name_end(void) { m_code << "</funcdef_param_name>"; }

void DumpGenerator::handle_function_definition_parameter_list_end(void) { m_code << "</funcdef_param_list>"; }

void DumpGenerator::handle_function_definition_code_begin(void) { m_code << "<funcdef_code>"; }

void DumpGenerator::handle_function_definition_code_end(void) { m_code << "</funcdef_code>"; }

void DumpGenerator::handle_function_definition_end(void) { m_code << "</funcdef>"; }

void DumpGenerator::handle_function_call_begin(void) { m_code << "<func_call>"; }

void DumpGenerator::handle_function_call_name_begin(void) { m_code << "<func_call_name>"; }

void DumpGenerator::handle_function_call_name_end(void) { m_code << "</func_call_name>"; }

void DumpGenerator::handle_function_call_parameter_list_begin(void) { m_code << "<func_call_param_list>"; }

void DumpGenerator::handle_function_call_parameter_begin(void) { m_code << "<func_call_param>"; }

void DumpGenerator::handle_function_call_parameter_end(void) { m_code << "</func_call_param>"; }

void DumpGenerator::handle_function_call_parameter_list_end(void) { m_code << "</func_call_param_list>"; }

void DumpGenerator::handle_function_call_end(void) { m_code << "</func_call>"; }

void DumpGenerator::handle_return_begin(void) { m_code << "<return>"; }

void DumpGenerator::handle_return_end(void) { m_code << "</return>"; }

void DumpGenerator::handle_self_begin(void) { m_code << "<self>"; }

void DumpGenerator::handle_self_name_begin(void) { m_code << "<self_name>"; }

void DumpGenerator::handle_self_name_end(void) { m_code << "</self_name>"; }

void DumpGenerator::handle_self_parameter_list_begin(void) { m_code << "<self_param_list>"; }

void DumpGenerator::handle_self_parameter_type_begin(void) { m_code << "<self_param_type>"; }

void DumpGenerator::handle_self_parameter_type_end(void) { m_code << "</self_param_type>"; }

void DumpGenerator::handle_self_parameter_name_begin(void) { m_code << "<self_param_name>"; }

void DumpGenerator::handle_self_parameter_name_end(void) { m_code << "</self_param_name>"; }

void DumpGenerator::handle_self_parameter_list_end(void) { m_code << "</self_param_list>"; }

void DumpGenerator::handle_self_end(void) { m_code << "</self>"; }

bool DumpGenerator::handle_environment_begin(ShaderEnvironment env)
{
    m_code << "<env value=\"" << get_shader_env_define(env) << "\">";
    return true;
}

void DumpGenerator::handle_environment_end(void) { m_code << "</env>"; }

void DumpGenerator::append(const Generator& generator)
{
    const DumpGenerator* other = dynamic_cast<const DumpGenerator*>(&generator);
    if(!other)
        return;

    m_code << other->m_code.str();
}

boost::shared_ptr<Generator> DumpGenerator::construct(void) const { return boost::shared_ptr<Generator>(new DumpGenerator); }
