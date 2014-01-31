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

#include "GL2Generator.h"
#include <boost/lexical_cast.hpp>
#include <iostream>


using namespace shade::formatter;

GL2Generator::GL2Generator(ShaderEnvironment env) :
  m_env(env),
  m_switch_needs_else(false),
  m_indentation_level(0)
{
  m_code.setf(std::ios_base::scientific, std::ios_base::floatfield);
}

std::string GL2Generator::get_code(void) const
{
  return m_code.str();
}

std::ostringstream& GL2Generator::indent(void)
{
  for (int i = 0; i != m_indentation_level; ++i)
    m_code << "  ";
  return m_code;
}

void GL2Generator::handle_identifier(const std::string& name)
{
  m_code << name;
  m_identifier_set = true;
}

void GL2Generator::handle_verbatim(const std::string& code)
{
  m_code << code;
}

void GL2Generator::handle_type(Constants::Type type)
{
  static const char* type_names[] =
  {
    "float", "vec2", "vec3", "vec4", "mat4",
    "sampler2D", "sampler2DShadow", "sampler2DArray", "usampler2D",
    "samplerCube",
    "OBJREF", "LIST",
    "void", "int"
  };

  m_code << type_names[type];
}

void GL2Generator::handle_qualifier(Constants::Qualifier qualifier)
{
  std::string result;
  if (qualifier == formatter::Constants::input)
  {
    static const char* qualifier_input_names[] =
    {
      "attribute",
      "varying",
      "varying in",
    };
    result = qualifier_input_names[m_env];
  }
  else if (qualifier == formatter::Constants::output)
  {
    static const char* qualifier_output_names[] =
    {
      "varying",
      "",
      "varying out",
    };
    result = qualifier_output_names[m_env];
  }
  else
  {
    static const char* qualifier_names[] =
    {
      "",
      "uniform",
      "const",
    };
    result = qualifier_names[qualifier];
  }

  m_code << result;
}

void GL2Generator::handle_operator(Constants::Operator op)
{
  m_identifier_set = false;
  m_operator = op;
}


void GL2Generator::handle_literal_objref(ObjectMap::Index ref)
{
  m_code << boost::lexical_cast<std::string>(ref);
}

void GL2Generator::handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end)
{
  m_code << "LIST(";
  m_code << boost::lexical_cast<std::string>(begin);
  m_code << ", ";
  m_code << boost::lexical_cast<std::string>(end);
  m_code << ")";
}

void GL2Generator::handle_literal_int(int value)
{
  m_code << boost::lexical_cast<std::string>(value);
}

void GL2Generator::handle_literal_float(float value)
{
  m_code << value;
}

void GL2Generator::handle_literal_vec2(float x, float y)
{
  m_code << "vec2(";
  m_code << boost::lexical_cast<std::string>(x);
  m_code << ", " << boost::lexical_cast<std::string>(y);
  m_code << ")";
}

void GL2Generator::handle_literal_vec3(float x, float y, float z)
{
  m_code << "vec3(";
  m_code << boost::lexical_cast<std::string>(x);
  m_code << ", " << boost::lexical_cast<std::string>(y);
  m_code << ", " << boost::lexical_cast<std::string>(z);
  m_code << ")";
}

void GL2Generator::handle_literal_vec4(float x, float y, float z, float w)
{
  m_code << "vec4(";
  m_code << boost::lexical_cast<std::string>(x);
  m_code << ", " << boost::lexical_cast<std::string>(y);
  m_code << ", " << boost::lexical_cast<std::string>(z);
  m_code << ", " << boost::lexical_cast<std::string>(w);
  m_code << ")";
}


void GL2Generator::handle_array_begin(void)
{
  m_type_is_array = true;
}

void GL2Generator::handle_array_type_begin(void)
{
}

void GL2Generator::handle_array_type_end(void)
{
}

void GL2Generator::handle_array_size_begin(void)
{
  m_diverted_outputs.push_back(m_code.str());
  m_code.str("");
}

void GL2Generator::handle_array_size_end(void)
{
  m_array_size = m_code.str();

  if (m_diverted_outputs.empty())
    return;

  m_code.str("");
  m_code << m_diverted_outputs.back();
  m_diverted_outputs.pop_back();
}

void GL2Generator::handle_array_end(void)
{
}


void GL2Generator::handle_switch_begin(void)
{
  handle_block_begin();
  m_switch_needs_else = false;
}

void GL2Generator::handle_switch_value_begin(void)
{
  indent() << "int shade_comparison_value = ";
}

void GL2Generator::handle_switch_value_end(void)
{
  m_code << ";\n";
}

void GL2Generator::handle_switch_case_begin(void)
{
  indent();
  if (m_switch_needs_else)
    m_code << "else ";
  m_code << "if (shade_comparison_value == ";
  m_switch_needs_else = true;
}

void GL2Generator::handle_switch_case_end(void)
{
  m_code << ")\n";
}

void GL2Generator::handle_switch_code_begin(void)
{
  handle_block_begin();
}

void GL2Generator::handle_switch_code_end(void)
{
  handle_block_end();
}

void GL2Generator::handle_switch_default_begin(void)
{
  if (m_switch_needs_else)
    indent() << "else\n";
  handle_block_begin();
}

void GL2Generator::handle_switch_default_end(void)
{
  handle_block_end();
}

void GL2Generator::handle_switch_end(void)
{
  handle_block_end();
}

void GL2Generator::handle_for_begin(void)
{
  indent() << "for (";
}

void GL2Generator::handle_for_init_begin(void)
{
}

void GL2Generator::handle_for_init_end(void)
{
  m_code << "; ";
}

void GL2Generator::handle_for_condition_begin(void)
{
}

void GL2Generator::handle_for_condition_end(void)
{
  m_code << "; ";
}

void GL2Generator::handle_for_increment_begin(void)
{
}

void GL2Generator::handle_for_increment_end(void)
{
  m_code << ")\n";
}

void GL2Generator::handle_for_code_begin(void)
{
  handle_block_begin();
}

void GL2Generator::handle_for_code_end(void)
{
  handle_block_end();
}

void GL2Generator::handle_for_end(void)
{
}

void GL2Generator::handle_var_definition_begin(void)
{
}

void GL2Generator::handle_var_definition_qualifier_list_begin(void)
{
}

void GL2Generator::handle_var_definition_qualifier_begin(void)
{
}

void GL2Generator::handle_var_definition_qualifier_end(void)
{
  m_code << " ";
}

void GL2Generator::handle_var_definition_qualifier_list_end(void)
{
}

void GL2Generator::handle_var_definition_type_begin(void)
{
  m_type_is_array = false;
}

void GL2Generator::handle_var_definition_type_end(void)
{
  m_code << " ";
}

void GL2Generator::handle_var_definition_name_begin(void)
{
}

void GL2Generator::handle_var_definition_name_end(void)
{
  if (m_type_is_array)
    m_code << "[" << m_array_size << "]";
}

void GL2Generator::handle_var_definition_code_begin(void)
{
  m_code << " = ";
}

void GL2Generator::handle_var_definition_code_end(void)
{
}

void GL2Generator::handle_var_definition_end(void)
{
}

void GL2Generator::handle_statement_begin(void)
{
  indent();
}

void GL2Generator::handle_statement_end(void)
{
  m_code << ";\n";
}

void GL2Generator::handle_block_begin(void)
{
  indent() << "{\n";
  m_indentation_level += 1;
}

void GL2Generator::handle_block_end(void)
{
  m_indentation_level -= 1;
  indent() << "}\n";
}

void GL2Generator::handle_function_definition_begin(void)
{
  indent(); // This should always be a noop as functions can not be nested
}

void GL2Generator::handle_function_definition_return_type_begin(void)
{
}

void GL2Generator::handle_function_definition_return_type_end(void)
{
  m_code << " ";
}

void GL2Generator::handle_function_definition_name_begin(void)
{
  m_function_has_code = false;
}

void GL2Generator::handle_function_definition_name_end(void)
{
  m_code << "(";
}

void GL2Generator::handle_function_definition_parameter_list_begin(void)
{
  m_function_parameter_type_set = false;
  m_function_parameter_needs_separator = false;
}

void GL2Generator::handle_function_definition_parameter_qualifier_list_begin(void)
{
}

void GL2Generator::handle_function_definition_parameter_qualifier_begin(void)
{
}

void GL2Generator::handle_function_definition_parameter_qualifier_end(void)
{
}

void GL2Generator::handle_function_definition_parameter_qualifier_list_end(void)
{
}

void GL2Generator::handle_function_definition_parameter_type_begin(void)
{
  if (m_function_parameter_needs_separator)
    m_code << ", ";
}

void GL2Generator::handle_function_definition_parameter_type_end(void)
{
  m_function_parameter_type_set = true;
}

void GL2Generator::handle_function_definition_parameter_name_begin(void)
{
  if (m_function_parameter_type_set)
    m_code << " ";
}

void GL2Generator::handle_function_definition_parameter_name_end(void)
{
  m_function_parameter_needs_separator = true;
}

void GL2Generator::handle_function_definition_parameter_list_end(void)
{
  m_code << ")";
}

void GL2Generator::handle_function_definition_code_begin(void)
{
  m_code << "\n";
  handle_block_begin();
}

void GL2Generator::handle_function_definition_code_end(void)
{
  handle_block_end();
  m_function_has_code = true;
}

void GL2Generator::handle_function_definition_end(void)
{
  if (!m_function_has_code)
    m_code << ";\n";
}

void GL2Generator::handle_function_call_begin(void)
{
}

void GL2Generator::handle_function_call_name_begin(void)
{
}

void GL2Generator::handle_function_call_name_end(void)
{
  std::string prefix;
  std::string separator;
  std::string postfix;

  if (m_identifier_set)
  {
    prefix = "(";
    separator = ", ";
    postfix = ")";
  }
  else
  {
    static const char* prefixes[] =
    {
      "++(", "(",
      "(", "(",
      "((", "((", "(",
      "(", "(", "(", "(",
      ""
    };
    static const char* separators[] =
    {
      "", "",
      ")==(", ")!=(",
      ")=(", ")[", ").",
      "*", "/", "+" , "-",
      "+="
    };
    static const char* postfixes[] =
    {
      ")", ")++",
      ")", ")",
      "))", "])", "",
      ")", ")", ")", ")",
      ""
    };

    prefix = prefixes[m_operator];
    separator = separators[m_operator];
    postfix =  postfixes[m_operator];
  }

  m_code << prefix;
  m_function_parameter_parts.push_back(std::make_pair(separator, postfix));
}

void GL2Generator::handle_function_call_parameter_list_begin(void)
{
  m_function_parameter_needs_separator = false;
}

void GL2Generator::handle_function_call_parameter_begin(void)
{
  if (m_function_parameter_needs_separator)
    m_code << m_function_parameter_parts.back().first;
  m_function_parameter_needs_separator = true;
}

void GL2Generator::handle_function_call_parameter_end(void)
{
}

void GL2Generator::handle_function_call_parameter_list_end(void)
{
  m_code << m_function_parameter_parts.back().second;
  m_function_parameter_parts.pop_back();
}

void GL2Generator::handle_function_call_end(void)
{
}


void GL2Generator::handle_return_begin(void)
{
  m_code << "return";

  m_diverted_outputs.push_back(m_code.str());
  m_code.str("");
}

void GL2Generator::handle_return_end(void)
{
  std::string value(m_code.str());
  if (!m_diverted_outputs.empty())
  {
    m_code.str("");
    m_code << m_diverted_outputs.back();
    m_diverted_outputs.pop_back();
  }

  if (!value.empty())
  {
    m_code << " " << value;
  }

  m_code << ";\n";
}


void GL2Generator::handle_self_begin(void)
{
  m_code << "#define ";
}

void GL2Generator::handle_self_name_begin(void)
{
}

void GL2Generator::handle_self_name_end(void)
{
}

void GL2Generator::handle_self_parameter_list_begin(void)
{
  m_code << " in OBJREF self";
}

void GL2Generator::handle_self_parameter_type_begin(void)
{
  m_code << ", in ";
}

void GL2Generator::handle_self_parameter_type_end(void)
{
}

void GL2Generator::handle_self_parameter_name_begin(void)
{
  m_code << " ";
}

void GL2Generator::handle_self_parameter_name_end(void)
{
}

void GL2Generator::handle_self_parameter_list_end(void)
{
}

void GL2Generator::handle_self_end(void)
{
  m_code << "\n";
}


bool GL2Generator::handle_environment_begin(ShaderEnvironment env)
{
  m_diverted_outputs.push_back(m_code.str());
  m_code.str("");

  m_environment_matches = ((env == invariant_shader) || (env == m_env));

  return m_environment_matches;
}

void GL2Generator::handle_environment_end(void)
{
  std::string code(m_code.str());
  if (!m_diverted_outputs.empty())
  {
    m_code.str("");
    m_code << m_diverted_outputs.back();
    m_diverted_outputs.pop_back();
  }

  if (m_environment_matches)
    m_code << code;
}


void GL2Generator::append(const Generator& generator)
{
  const GL2Generator* other = dynamic_cast<const GL2Generator*>(&generator);
  if (!other)
    return;

  m_code << other->m_code.str();
}

void GL2Generator::insert_define(const std::string& name, const std::string& value)
{
  m_code << "#define " << name << " " << value << "\n";
}

void GL2Generator::insert_extension(const std::string& name, Extension::Behavior behavior)
{
  const char* behavior_names[] =
  {
    "require", "enable", "warn", "disable"
  };

  m_code << "#extension ";
  m_code << name << ": " << behavior_names[behavior];
  m_code << "\n";
}

void GL2Generator::insert_sl_version(int version)
{
  m_code << "#version " << version << "\n";
}

void GL2Generator::insert_type_definitions(void)
{
  static std::string types("#define OBJREF int\n"
      "#define FOREACH(index, list) for(OBJREF index = list.begin; index != list.end; ++index)\n"
      "struct LIST\n"
      "{\n"
      "  int begin;\n"
      "  int end;\n"
      "};\n");

  m_code << types;
}

boost::shared_ptr<Generator> GL2Generator::construct(void) const
{
  return boost::shared_ptr<Generator>(new GL2Generator(m_env));
}
