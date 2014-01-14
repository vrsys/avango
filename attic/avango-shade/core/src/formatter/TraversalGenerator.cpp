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

#include "TraversalGenerator.h"
#include "../parser/Scope.h"
#include "../parser/ScopeLayer.h"
#include "../parser/parse.h"
#include <sstream>
using namespace shade::formatter;

namespace
{
  std::string identity(const std::string& name, const shade::parser::ValueList& param,
      const shade::parser::ScopeLayer&, std::ostream& error_log)
  {
    return name;
  }
}


TraversalGenerator::TraversalGenerator(void) :
  m_generator_index(0)
{
}

bool TraversalGenerator::traverse(Generator& generator, std::ostream& error_log) const
{
  boost::shared_ptr<shade::parser::Element> element(m_generator[0].get_element());
  if (!element)
    return false;

  parser::ScopeLayer global;
  parser::Scope scope(global);
  element->get_content(generator, scope, identity, error_log);

  return m_instantiator.build(generator, scope.get_local(), error_log);
}

std::string TraversalGenerator::get_code(void) const
{
  return "";
}

void TraversalGenerator::handle_identifier(const std::string& name)
{
  if (m_generator_index == 2)
  {
    m_last_identifier = name;
    return;
  }

  m_generator[m_generator_index].handle_identifier(name);
}

void TraversalGenerator::handle_verbatim(const std::string& code)
{
  if (m_generator_index == 0)
  {
    if (parser::parse(code, *this, m_translator))
      return;
  }

  m_generator[m_generator_index].handle_verbatim(code);
}

void TraversalGenerator::handle_type(Constants::Type type)
{
  if (m_generator_index == 2)
  {
    m_last_type = type;
    return;
  }

  m_generator[m_generator_index].handle_type(type);
}

void TraversalGenerator::handle_qualifier(Constants::Qualifier qualifier)
{
  m_generator[m_generator_index].handle_qualifier(qualifier);
}

void TraversalGenerator::handle_operator(Constants::Operator op)
{
  m_generator[m_generator_index].handle_operator(op);
}


void TraversalGenerator::handle_literal_objref(ObjectMap::Index ref)
{
  m_generator[m_generator_index].handle_literal_objref(ref);
}

void TraversalGenerator::handle_literal_list(ObjectMap::Index begin, ObjectMap::Index end)
{
  m_generator[m_generator_index].handle_literal_list(begin, end);
}

void TraversalGenerator::handle_literal_int(int value)
{
  m_generator[m_generator_index].handle_literal_int(value);
}

void TraversalGenerator::handle_literal_float(float value)
{
  m_generator[m_generator_index].handle_literal_float(value);
}

void TraversalGenerator::handle_literal_vec2(float x, float y)
{
  m_generator[m_generator_index].handle_literal_vec2(x, y);
}

void TraversalGenerator::handle_literal_vec3(float x, float y, float z)
{
  m_generator[m_generator_index].handle_literal_vec3(x, y, z);
}

void TraversalGenerator::handle_literal_vec4(float x, float y, float z, float w)
{
  m_generator[m_generator_index].handle_literal_vec4(x, y, z, w);
}


void TraversalGenerator::handle_array_begin(void)
{
  m_generator[m_generator_index].handle_array_begin();
}

void TraversalGenerator::handle_array_type_begin(void)
{
  m_generator[m_generator_index].handle_array_type_begin();
}

void TraversalGenerator::handle_array_type_end(void)
{
  m_generator[m_generator_index].handle_array_type_end();
}

void TraversalGenerator::handle_array_size_begin(void)
{
  m_generator[m_generator_index].handle_array_size_begin();
}

void TraversalGenerator::handle_array_size_end(void)
{
  m_generator[m_generator_index].handle_array_size_end();
}

void TraversalGenerator::handle_array_end(void)
{
  m_generator[m_generator_index].handle_array_end();
}


void TraversalGenerator::handle_switch_begin(void)
{
  m_generator[m_generator_index].handle_switch_begin();
}

void TraversalGenerator::handle_switch_value_begin(void)
{
  m_generator[m_generator_index].handle_switch_value_begin();
}

void TraversalGenerator::handle_switch_value_end(void)
{
  m_generator[m_generator_index].handle_switch_value_end();
}

void TraversalGenerator::handle_switch_case_begin(void)
{
  m_generator[m_generator_index].handle_switch_case_begin();
}

void TraversalGenerator::handle_switch_case_end(void)
{
  m_generator[m_generator_index].handle_switch_case_end();
}

void TraversalGenerator::handle_switch_code_begin(void)
{
  m_generator[m_generator_index].handle_switch_code_begin();
}

void TraversalGenerator::handle_switch_code_end(void)
{
  m_generator[m_generator_index].handle_switch_code_end();
}

void TraversalGenerator::handle_switch_default_begin(void)
{
  m_generator[m_generator_index].handle_switch_default_begin();
}

void TraversalGenerator::handle_switch_default_end(void)
{
  m_generator[m_generator_index].handle_switch_default_end();
}

void TraversalGenerator::handle_switch_end(void)
{
  m_generator[m_generator_index].handle_switch_end();
}


void TraversalGenerator::handle_for_begin(void)
{
  m_generator[m_generator_index].handle_for_begin();
}

void TraversalGenerator::handle_for_init_begin(void)
{
  m_generator[m_generator_index].handle_for_init_begin();
}

void TraversalGenerator::handle_for_init_end(void)
{
  m_generator[m_generator_index].handle_for_init_end();
}

void TraversalGenerator::handle_for_condition_begin(void)
{
  m_generator[m_generator_index].handle_for_condition_begin();
}

void TraversalGenerator::handle_for_condition_end(void)
{
  m_generator[m_generator_index].handle_for_condition_end();
}

void TraversalGenerator::handle_for_increment_begin(void)
{
  m_generator[m_generator_index].handle_for_increment_begin();
}

void TraversalGenerator::handle_for_increment_end(void)
{
  m_generator[m_generator_index].handle_for_increment_end();
}

void TraversalGenerator::handle_for_code_begin(void)
{
  m_generator[m_generator_index].handle_for_code_begin();
}

void TraversalGenerator::handle_for_code_end(void)
{
  m_generator[m_generator_index].handle_for_code_end();
}

void TraversalGenerator::handle_for_end(void)
{
  m_generator[m_generator_index].handle_for_end();
}


void TraversalGenerator::handle_var_definition_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_begin();
}

void TraversalGenerator::handle_var_definition_qualifier_list_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_qualifier_list_begin();
}

void TraversalGenerator::handle_var_definition_qualifier_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_qualifier_begin();
}

void TraversalGenerator::handle_var_definition_qualifier_end(void)
{
  m_generator[m_generator_index].handle_var_definition_qualifier_end();
}

void TraversalGenerator::handle_var_definition_qualifier_list_end(void)
{
  m_generator[m_generator_index].handle_var_definition_qualifier_list_end();
}

void TraversalGenerator::handle_var_definition_type_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_type_begin();
}

void TraversalGenerator::handle_var_definition_type_end(void)
{
  m_generator[m_generator_index].handle_var_definition_type_end();
}

void TraversalGenerator::handle_var_definition_name_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_name_begin();
}

void TraversalGenerator::handle_var_definition_name_end(void)
{
  m_generator[m_generator_index].handle_var_definition_name_end();
}

void TraversalGenerator::handle_var_definition_code_begin(void)
{
  m_generator[m_generator_index].handle_var_definition_code_begin();
}

void TraversalGenerator::handle_var_definition_code_end(void)
{
  m_generator[m_generator_index].handle_var_definition_code_end();
}

void TraversalGenerator::handle_var_definition_end(void)
{
  m_generator[m_generator_index].handle_var_definition_end();
}


void TraversalGenerator::handle_statement_begin(void)
{
  m_generator[m_generator_index].handle_statement_begin();
}

void TraversalGenerator::handle_statement_end(void)
{
  m_generator[m_generator_index].handle_statement_end();
}


void TraversalGenerator::handle_block_begin(void)
{
  m_generator[m_generator_index].handle_block_begin();
}

void TraversalGenerator::handle_block_end(void)
{
  m_generator[m_generator_index].handle_block_end();
}


void TraversalGenerator::handle_function_definition_begin(void)
{
  m_generator_index = 1;
  m_generator[m_generator_index].handle_function_definition_begin();
}

void TraversalGenerator::handle_function_definition_return_type_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_return_type_begin();
}

void TraversalGenerator::handle_function_definition_return_type_end(void)
{
  m_generator[m_generator_index].handle_function_definition_return_type_end();
}

void TraversalGenerator::handle_function_definition_name_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_name_begin();
}

void TraversalGenerator::handle_function_definition_name_end(void)
{
  m_generator[m_generator_index].handle_function_definition_name_end();
}

void TraversalGenerator::handle_function_definition_parameter_list_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_list_begin();
}

void TraversalGenerator::handle_function_definition_parameter_qualifier_list_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_qualifier_list_begin();
}

void TraversalGenerator::handle_function_definition_parameter_qualifier_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_qualifier_begin();
}

void TraversalGenerator::handle_function_definition_parameter_qualifier_end(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_qualifier_end();
}

void TraversalGenerator::handle_function_definition_parameter_qualifier_list_end(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_qualifier_list_end();
}

void TraversalGenerator::handle_function_definition_parameter_type_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_type_begin();
}

void TraversalGenerator::handle_function_definition_parameter_type_end(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_type_end();
}

void TraversalGenerator::handle_function_definition_parameter_name_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_name_begin();
}

void TraversalGenerator::handle_function_definition_parameter_name_end(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_name_end();
}

void TraversalGenerator::handle_function_definition_parameter_list_end(void)
{
  m_generator[m_generator_index].handle_function_definition_parameter_list_end();
}

void TraversalGenerator::handle_function_definition_code_begin(void)
{
  m_generator[m_generator_index].handle_function_definition_code_begin();
}

void TraversalGenerator::handle_function_definition_code_end(void)
{
  m_generator[m_generator_index].handle_function_definition_code_end();
}

void TraversalGenerator::handle_function_definition_end(void)
{
  m_generator[m_generator_index].handle_function_definition_end();

  boost::shared_ptr<parser::MethodTemplate> function(m_generator[m_generator_index].get_method_template());
  if (function)
    m_instantiator.add_method(function);

  m_generator[m_generator_index].reset();
  m_generator_index = 0;
}

void TraversalGenerator::handle_function_call_begin(void)
{
  m_generator[m_generator_index].handle_function_call_begin();
}

void TraversalGenerator::handle_function_call_name_begin(void)
{
  m_generator[m_generator_index].handle_function_call_name_begin();
}

void TraversalGenerator::handle_function_call_name_end(void)
{
  m_generator[m_generator_index].handle_function_call_name_end();
}

void TraversalGenerator::handle_function_call_parameter_list_begin(void)
{
  m_generator[m_generator_index].handle_function_call_parameter_list_begin();
}

void TraversalGenerator::handle_function_call_parameter_begin(void)
{
  m_generator[m_generator_index].handle_function_call_parameter_begin();
}

void TraversalGenerator::handle_function_call_parameter_end(void)
{
  m_generator[m_generator_index].handle_function_call_parameter_end();
}

void TraversalGenerator::handle_function_call_parameter_list_end(void)
{
  m_generator[m_generator_index].handle_function_call_parameter_list_end();
}

void TraversalGenerator::handle_function_call_end(void)
{
  m_generator[m_generator_index].handle_function_call_end();
}


void TraversalGenerator::handle_return_begin(void)
{
  m_generator[m_generator_index].handle_return_begin();
}

void TraversalGenerator::handle_return_end(void)
{
  m_generator[m_generator_index].handle_return_end();
}


void TraversalGenerator::handle_self_begin(void)
{
  m_generator_index = 2;
  m_translator.handle_self_begin();
}

void TraversalGenerator::handle_self_name_begin(void)
{
  m_translator.handle_self_name_begin();
}

void TraversalGenerator::handle_self_name_end(void)
{
  m_translator.handle_identifier(m_last_identifier);
  m_translator.handle_self_name_end();
}

void TraversalGenerator::handle_self_parameter_list_begin(void)
{
  m_translator.handle_self_parameter_list_begin();
}

void TraversalGenerator::handle_self_parameter_type_begin(void)
{
  m_translator.handle_self_parameter_type_begin();
}

void TraversalGenerator::handle_self_parameter_type_end(void)
{
  m_translator.handle_type(m_last_type);
  m_translator.handle_self_parameter_type_end();
}

void TraversalGenerator::handle_self_parameter_name_begin(void)
{
  m_translator.handle_self_parameter_name_begin();
}

void TraversalGenerator::handle_self_parameter_name_end(void)
{
  m_translator.handle_identifier(m_last_identifier);
  m_translator.handle_self_parameter_name_end();
}

void TraversalGenerator::handle_self_parameter_list_end(void)
{
  m_translator.handle_self_parameter_list_end();
}

void TraversalGenerator::handle_self_end(void)
{
  m_translator.handle_self_end();
  m_generator_index = 0;
  m_generator[2].reset();
}


bool TraversalGenerator::handle_environment_begin(ShaderEnvironment env)
{
  return m_generator[m_generator_index].handle_environment_begin(env);
}

void TraversalGenerator::handle_environment_end(void)
{
  m_generator[m_generator_index].handle_environment_end();
}


void TraversalGenerator::append(const Generator& generator)
{
  const TraversalGenerator* other = dynamic_cast<const TraversalGenerator*>(&generator);
  if (!other)
    return;

  m_generator[m_generator_index].append(other->m_generator[0]);
  m_translator.append(other->m_translator);
  m_instantiator.append(other->m_instantiator);
}

boost::shared_ptr<Generator> TraversalGenerator::construct(void) const
{
  return boost::shared_ptr<Generator>(new TraversalGenerator);
}
