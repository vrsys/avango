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

#include "../../src/formatter/TraversalGenerator.h"
#include <shade/ObjectMap.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include "DumpGenerator.h"
#include "diff_dump.h"
#include <UnitTest++.h>

SUITE(TestTraversalGenerator)
{
  void generate_function_header_void_void(shade::formatter::Generator& generator, const std::string& name)
  {
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(shade::formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_list_end();
  }

  void generate_function_header_void_objref(shade::formatter::Generator& generator, const std::string& name, const std::string& arg1)
  {
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(shade::formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier(arg1);
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
  }

  void generate_function_header_void_objref_vec3(shade::formatter::Generator& generator, const std::string& name, const std::string& arg1, const std::string& arg2, bool use_verbatim = false)
  {
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    if (use_verbatim)
      generator.handle_verbatim("void");
    else
      generator.handle_type(shade::formatter::Constants::void_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::objref);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier(arg1);
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier(arg2);
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
  }

  void generate_function_call_void(shade::formatter::Generator& generator, const std::string& name)
  {
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
  }

  void generate_function_call_objref(shade::formatter::Generator& generator, const std::string& name, shade::ObjectMap::Index index, bool use_verbatim = false)
  {
    if (!use_verbatim)
      generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    if (use_verbatim)
      generator.handle_verbatim(boost::lexical_cast<std::string>(index));
    else
      generator.handle_literal_objref(index);
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    if (use_verbatim)
      generator.handle_verbatim(";");
    else
      generator.handle_statement_end();
  }

  void generate_function_call_objref_vec3(shade::formatter::Generator& generator, const std::string& name, shade::ObjectMap::Index index)
  {
    generator.handle_statement_begin();
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_literal_objref(index);
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    generator.handle_literal_vec3(1.1, 2.2, 3.3);
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
    generator.handle_statement_end();
  }

  void generate_var_definition(shade::formatter::Generator& generator, shade::formatter::Constants::Type type, const std::string& name, shade::ObjectMap::Index index = 0)
  {
    generator.handle_statement_begin();
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_type(type);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_var_definition_name_end();
    if (index)
    {
      generator.handle_var_definition_code_begin();
      generator.handle_literal_objref(index);
      generator.handle_var_definition_code_end();
    }
    generator.handle_var_definition_end();
    generator.handle_statement_end();
  }

  TEST(PassThough)
  {
    shade::formatter::TraversalGenerator generator;
    generate_var_definition(generator, shade::formatter::Constants::vec3, "data");
    generate_function_header_void_void(generator, "main");
    generator.handle_function_definition_code_begin();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    shade::formatter::DumpGenerator expected_generator;
    generate_var_definition(expected_generator, shade::formatter::Constants::vec3, "data");
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generator.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(SelectedFunctionInclusion)
  {
    shade::formatter::TraversalGenerator generator;
    generate_function_header_void_objref(generator, "A", "self");
    generator.handle_function_definition_code_begin();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
    generate_function_header_void_objref(generator, "B", "self");
    generator.handle_function_definition_code_begin();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
    generate_function_header_void_void(generator, "main");
    generator.handle_function_definition_code_begin();
    generate_function_call_objref(generator, "A", 1);
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    shade::formatter::DumpGenerator expected_generator;
    generate_function_header_void_objref(expected_generator, "A", "self");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_objref(expected_generator, "A", 1);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generator.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(VerbatimMethod)
  {
    shade::formatter::TraversalGenerator generator;
    generate_function_header_void_objref(generator, "A", "self");
    generator.handle_function_definition_code_begin();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
    generate_function_header_void_void(generator, "main");
    generator.handle_function_definition_code_begin();
    generate_function_call_objref_vec3(generator, "B", 1);
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    generator.handle_self_begin();
    generator.handle_self_name_begin();
    generator.handle_identifier("Class_SELF");
    generator.handle_self_name_end();
    generator.handle_self_parameter_list_begin();
    generator.handle_self_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_self_parameter_type_end();
    generator.handle_self_parameter_name_begin();
    generator.handle_identifier("position");
    generator.handle_self_parameter_name_end();
    generator.handle_self_parameter_list_end();
    generator.handle_self_end();

    generator.handle_verbatim(
        "void B(Class_SELF)\n"
        "{\n"
        "  A(2);\n"
        "}\n"
        );

    shade::formatter::DumpGenerator expected_generator;
    generate_function_header_void_objref(expected_generator, "A", "self");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_objref_vec3(expected_generator, "B", "self", "position", true);
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_objref(expected_generator, "A", 2, true);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_objref_vec3(expected_generator, "B", 1);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generator.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(AccessGlobalVariablesInMain)
  {
    std::string code("value;");
    std::string var("value");
    shade::ObjectMap::Index var_value(2);

    shade::formatter::TraversalGenerator generator;
    generate_var_definition(generator, shade::formatter::Constants::objref, var, var_value);
    generate_function_header_void_void(generator, "main");
    generator.handle_function_definition_code_begin();
    generator.handle_switch_begin();
    generator.handle_switch_value_begin();
    generator.handle_identifier(var);
    generator.handle_switch_value_end();
    generator.handle_switch_case_begin();
    generator.handle_literal_objref(var_value);
    generator.handle_switch_case_end();
    generator.handle_switch_code_begin();
    generator.handle_verbatim(code);
    generator.handle_switch_code_end();
    generator.handle_switch_end();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    shade::formatter::DumpGenerator expected_generator;
    generate_var_definition(expected_generator, shade::formatter::Constants::objref, var, var_value);
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_block_begin();
    expected_generator.handle_verbatim(code);
    expected_generator.handle_block_end();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generator.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(AccessGlobalVariablesInFunction)
  {
    std::string code("value;");
    std::string var("value");
    shade::ObjectMap::Index var_value(2);

    shade::formatter::TraversalGenerator generator;
    generate_var_definition(generator, shade::formatter::Constants::objref, var, var_value);
    generate_function_header_void_void(generator, "A");
    generator.handle_function_definition_code_begin();
    generator.handle_switch_begin();
    generator.handle_switch_value_begin();
    generator.handle_identifier(var);
    generator.handle_switch_value_end();
    generator.handle_switch_case_begin();
    generator.handle_literal_objref(var_value);
    generator.handle_switch_case_end();
    generator.handle_switch_code_begin();
    generator.handle_verbatim(code);
    generator.handle_switch_code_end();
    generator.handle_switch_end();
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
    generate_function_header_void_void(generator, "main");
    generator.handle_function_definition_code_begin();
    generate_function_call_void(generator, "A");
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();

    shade::formatter::DumpGenerator expected_generator;
    generate_var_definition(expected_generator, shade::formatter::Constants::objref, var, var_value);
    generate_function_header_void_void(expected_generator, "A");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_block_begin();
    expected_generator.handle_verbatim(code);
    expected_generator.handle_block_end();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_void(expected_generator, "A");
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generator.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(Append)
  {
    shade::formatter::TraversalGenerator generatorA;
    shade::formatter::TraversalGenerator generatorB;

    generate_var_definition(generatorA, shade::formatter::Constants::vec3, "dataA");
    generate_var_definition(generatorB, shade::formatter::Constants::vec3, "dataB");
    generate_function_header_void_objref(generatorA, "A", "self");
    generatorA.handle_function_definition_code_begin();
    generatorA.handle_function_definition_code_end();
    generatorA.handle_function_definition_end();
    generate_function_header_void_void(generatorB, "main");
    generatorB.handle_function_definition_code_begin();
    generate_function_call_objref_vec3(generatorB, "B", 1);
    generatorB.handle_function_definition_code_end();
    generatorB.handle_function_definition_end();
    generatorB.handle_self_begin();
    generatorB.handle_self_name_begin();
    generatorB.handle_identifier("Class_SELF");
    generatorB.handle_self_name_end();
    generatorB.handle_self_parameter_list_begin();
    generatorB.handle_self_parameter_type_begin();
    generatorB.handle_type(shade::formatter::Constants::vec3);
    generatorB.handle_self_parameter_type_end();
    generatorB.handle_self_parameter_name_begin();
    generatorB.handle_identifier("position");
    generatorB.handle_self_parameter_name_end();
    generatorB.handle_self_parameter_list_end();
    generatorB.handle_self_end();

    generatorA.append(generatorB);

    generatorA.handle_verbatim(
        "void B(Class_SELF)\n"
        "{\n"
        "  A(2);\n"
        "}\n"
        );

    shade::formatter::DumpGenerator expected_generator;
    generate_var_definition(expected_generator, shade::formatter::Constants::vec3, "dataA");
    generate_var_definition(expected_generator, shade::formatter::Constants::vec3, "dataB");
    generate_function_header_void_objref(expected_generator, "A", "self");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_objref_vec3(expected_generator, "B", "self", "position", true);
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_objref(expected_generator, "A", 2, true);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    generate_function_call_objref_vec3(expected_generator, "B", 1);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generatorA.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }

  TEST(AppendInUnfinishedFunction)
  {
    std::string code("noop;");
    shade::formatter::TraversalGenerator generatorA;
    shade::formatter::TraversalGenerator generatorB;

    generate_function_header_void_void(generatorA, "main");
    generatorA.handle_function_definition_code_begin();
    generatorB.handle_verbatim(code);
    generatorA.append(generatorB);
    generatorA.handle_function_definition_code_end();
    generatorA.handle_function_definition_end();

    shade::formatter::DumpGenerator expected_generator;
    generate_function_header_void_void(expected_generator, "main");
    expected_generator.handle_function_definition_code_begin();
    expected_generator.handle_verbatim(code);
    expected_generator.handle_function_definition_code_end();
    expected_generator.handle_function_definition_end();

    shade::formatter::DumpGenerator result_generator;
    std::ostringstream error_log;
    CHECK(generatorA.traverse(result_generator, error_log));
    CHECK(shade::formatter::check_dump_output(
          expected_generator.get_code()
          , result_generator.get_code()));
  }
}
