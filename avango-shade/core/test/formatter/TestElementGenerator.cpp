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

#include "../../src/formatter/ElementGenerator.h"
#include "DumpGenerator.h"
#include "diff_dump.h"
#include "../../src/parser/Scope.h"
#include "../../src/parser/ScopeLayer.h"
#include "../../src/parser/Value.h"
#include "../../src/parser/ReferenceValue.h"
#include "../../src/parser/ListValue.h"
#include "../../src/parser/IntValue.h"
#include "../../src/parser/ValueList.h"
#include <string>
#include <sstream>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <UnitTest++.h>

namespace shade
{
  namespace parser
  {
    class ValueList;
  }
}


SUITE(TestElementGenerator)
{
  typedef boost::function<void (shade::formatter::Generator&)> CodeChain;

  void generate_noop(shade::formatter::Generator& generator)
  {
    generator.handle_verbatim("noop;");
  }

  std::string translate_function_name(const std::string& name, const shade::parser::ValueList& param,
      const shade::parser::ScopeLayer& scope, std::ostream& error_log)
  {
    if (name == "translate_name")
    {
      std::string translated_name("name");
      for (shade::parser::ValueList::const_iterator i = param.begin(); i != param.end(); ++i)
      {
        const shade::parser::ReferenceValue* value(dynamic_cast<const shade::parser::ReferenceValue*>((*i).get()));
        if (value)
        {
          translated_name += "_"+boost::lexical_cast<std::string>(value->get());
        }
        else
        {
          translated_name += "_X";
        }
      }
      return translated_name;
    }
    return name;
  }

  struct Generators
  {
    Generators(void) :
      scope(global_scope)
    {
      boost::shared_ptr<const shade::parser::Value> value(new shade::parser::IntValue(42));
      global_scope.add_value("shade_num_classes", value);
    }

    std::string get_code(void)
    {
      boost::shared_ptr<shade::parser::Element> element(generator.get_element());
      if (!element)
        return "";

      shade::formatter::DumpGenerator dump_generator;
      element->get_content(dump_generator, scope, translate_function_name, error_log);
      return dump_generator.get_code();
    }

    shade::formatter::ElementGenerator generator;
    shade::parser::ScopeLayer global_scope;
    shade::parser::Scope scope;
    std::stringstream error_log;
  };

  bool check_reference_value(unsigned int expected, boost::shared_ptr<const shade::parser::Value> value)
  {
    if (!value->is_constant())
    {
      std::cerr << "Error: Value is not constant" << std::endl;
      return false;
    }

    const shade::parser::ReferenceValue* ref_value(dynamic_cast<const shade::parser::ReferenceValue*>(value.get()));
    if (!ref_value)
    {
      std::cerr << "Error: Value is not a ReferenceValue" << std::endl;
      return false;
    }

    bool result = (expected == ref_value->get());
    if (!result)
      std::cerr << "Error: Expected " << expected << " but was " << ref_value->get() << std::endl;

    return result;
  }

  bool check_int_value(int expected, boost::shared_ptr<const shade::parser::Value> value)
  {
    if (!value->is_constant())
    {
      std::cerr << "Error: Value is not constant" << std::endl;
      return false;
    }

    const shade::parser::IntValue* ref_value(dynamic_cast<const shade::parser::IntValue*>(value.get()));
    if (!ref_value)
    {
      std::cerr << "Error: Value is not a IntValue" << std::endl;
      return false;
    }

    bool result = (expected == ref_value->get());
    if (!result)
      std::cerr << "Error: Expected " << expected << " but was " << ref_value->get() << std::endl;

    return result;
  }


  TEST_FIXTURE(Generators, TestIdentifier)
  {
    generator.handle_identifier("value");
    CHECK_EQUAL("<id>value</id>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestEvaluateUnknownIdentifier)
  {
    generator.handle_identifier("value");
    CHECK(!generator.get_element()->evaluate(scope)->is_constant());
  }

  TEST_FIXTURE(Generators, TestEvaluateKnownIdentifier)
  {
    const int ref_value(3);
    const std::string ref_name("value");
    scope.add_value(ref_name, boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(ref_value)));
    generator.handle_identifier(ref_name);

    CHECK(check_reference_value(ref_value, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestVerbatim)
  {
    generator.handle_verbatim("noop");
    CHECK_EQUAL("<verbatim>noop</verbatim>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestType)
  {
    generator.handle_type(shade::formatter::Constants::vec3);
    CHECK_EQUAL("<type>vec3</type>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestQualifier)
  {
    generator.handle_qualifier(shade::formatter::Constants::uniform);
    CHECK_EQUAL("<qualifier>uniform</qualifier>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestOperator)
  {
    generator.handle_operator(shade::formatter::Constants::pre_increment);
    CHECK_EQUAL("<operator>pre_increment</operator>", this->get_code());
  }


  TEST_FIXTURE(Generators, TestLiteralObjref)
  {
    generator.handle_literal_objref(5);
    CHECK_EQUAL("<literal_objref>5</literal_objref>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestEvaluateLiteralObjref)
  {
    const int ref_value(3);
    generator.handle_literal_objref(ref_value);

    CHECK(check_reference_value(ref_value, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestLiteralList)
  {
    generator.handle_literal_list(5, 7);
    CHECK_EQUAL("<literal_list>5 7</literal_list>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestEvaluateLiteralList)
  {
    const shade::ObjectMap::Index begin(3);
    const shade::ObjectMap::Index end(4);
    generator.handle_literal_list(begin, end);

    boost::shared_ptr<const shade::parser::Value> value(generator.get_element()->evaluate(scope));
    const shade::parser::ListValue* list_value(dynamic_cast<const shade::parser::ListValue*>(value.get()));
    CHECK(list_value);
    if (!list_value)
      return;

    CHECK_EQUAL(begin, list_value->begin());
    CHECK_EQUAL(end, list_value->end());
  }

  TEST_FIXTURE(Generators, TestLiteralInt)
  {
    generator.handle_literal_int(5);
    CHECK_EQUAL("<literal_int>5</literal_int>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestLiteralFloat)
  {
    generator.handle_literal_float(1.1);
    CHECK_EQUAL("<literal_float>1.1</literal_float>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestLiteralVec2)
  {
    generator.handle_literal_vec2(1.1, 2.2);
    CHECK_EQUAL("<literal_vec2>1.1 2.2</literal_vec2>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestLiteralVec3)
  {
    generator.handle_literal_vec3(1.1, 2.2, 3.3);
    CHECK_EQUAL("<literal_vec3>1.1 2.2 3.3</literal_vec3>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestLiteralVec4)
  {
    generator.handle_literal_vec4(1.1, 2.2, 3.3, 4.4);
    CHECK_EQUAL("<literal_vec4>1.1 2.2 3.3 4.4</literal_vec4>", this->get_code());
  }


  TEST_FIXTURE(Generators, TestBlock)
  {
    generator.handle_block_begin();
    generator.handle_verbatim("noop;");
    generator.handle_block_end();
    CHECK_EQUAL("<block><verbatim>noop;</verbatim></block>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestBlockWithMultipleInstructions)
  {
    generator.handle_block_begin();
    generator.handle_verbatim("statement1;");
    generator.handle_verbatim("statement2;");
    generator.handle_block_end();
    CHECK_EQUAL("<block>"
        "<verbatim>statement1;</verbatim>"
        "<verbatim>statement2;</verbatim>"
        "</block>"
        , this->get_code());
  }

  TEST_FIXTURE(Generators, TestStatement)
  {
    generator.handle_statement_begin();
    generator.handle_verbatim("noop;");
    generator.handle_statement_end();
    CHECK_EQUAL("<statement><verbatim>noop;</verbatim></statement>", this->get_code());
  }

  void generate_function_definition_begin(shade::formatter::Generator& generator, const std::string& name)
  {
    generator.handle_function_definition_begin();
    generator.handle_function_definition_return_type_begin();
    generator.handle_type(shade::formatter::Constants::float_);
    generator.handle_function_definition_return_type_end();
    generator.handle_function_definition_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_definition_name_end();
    generator.handle_function_definition_parameter_list_begin();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("u");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_function_definition_parameter_type_end();
    generator.handle_function_definition_parameter_name_begin();
    generator.handle_identifier("v");
    generator.handle_function_definition_parameter_name_end();
    generator.handle_function_definition_parameter_list_end();
  }

  void generate_function_definition_end(shade::formatter::Generator& generator, CodeChain code)
  {
    generator.handle_function_definition_code_begin();
    code(generator);
    generator.handle_function_definition_code_end();
    generator.handle_function_definition_end();
  }

  void generate_function_definition(shade::formatter::Generator& generator, const std::string& name = "func", CodeChain code = generate_noop)
  {
    generate_function_definition_begin(generator, name);
    generate_function_definition_end(generator, code);
  }

  TEST_FIXTURE(Generators, TestFunctionDefinition)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_function_definition(dump_generator);
    generate_function_definition(generator);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_empty(shade::formatter::Generator& generator)
  {
    /* do nothing */
  }

  TEST_FIXTURE(Generators, TestFunctionDefinitionWithEmptyBody)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_function_definition(dump_generator, "func", generate_empty);
    generate_function_definition(generator, "func", generate_empty);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  TEST_FIXTURE(Generators, TestFunctionDeclaration)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_function_definition_begin(dump_generator, "func");
    dump_generator.handle_function_definition_end();
    generate_function_definition_begin(generator, "func");
    generator.handle_function_definition_end();

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  TEST_FIXTURE(Generators, TestInstantiationOfSimpleFunctionDefinition)
  {
    shade::formatter::DumpGenerator dump_generator;
    const std::string func_name("name");
    const std::string reference_func_name("reference_name");

    generate_function_definition(dump_generator, func_name);
    generate_function_definition(generator, reference_func_name);

    boost::shared_ptr<shade::parser::MethodTemplate> method(generator.get_method_template());
    CHECK(method);
    if (!method)
      return;

    shade::formatter::DumpGenerator instance_generator;
    shade::parser::ValueList parameters;
    parameters.add(boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    parameters.add(boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    CHECK_EQUAL(reference_func_name, method->get_name());
    CHECK(method->instantiate(func_name, parameters, instance_generator, global_scope, error_log, translate_function_name));
    CHECK_EQUAL(dump_generator.get_code(), instance_generator.get_code());
  }

  void generate_function_call(shade::formatter::Generator& generator, const std::string& name = "func")
  {
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("u");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier("v");
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
  }

  TEST_FIXTURE(Generators, TestFunctionCall)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_function_call(dump_generator);
    generate_function_call(generator);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  TEST_FIXTURE(Generators, TestNameTranslationInFunctionCall)
  {
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("u", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    scope.add_value("v", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    generate_function_call(dump_generator, "name_1_2");
    generate_function_call(generator, "translate_name");

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_function(shade::formatter::Generator& generator, const std::string& name, const std::string& param1)
  {
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_identifier(param1);
    generator.handle_function_call_parameter_end();
    generator.handle_function_call_parameter_list_end();
  }

  TEST_FIXTURE(Generators, TestEvaluateListBegin)
  {
    const int list_begin(1);
    const int list_end(3);
    const std::string value_name("value");
    scope.add_value(value_name, boost::shared_ptr<const shade::parser::Value>(new shade::parser::ListValue(list_begin, list_end)));

    generate_function(generator, "shade_list_begin", value_name);

    CHECK(check_reference_value(list_begin, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestEvaluateListEnd)
  {
    const int list_begin(1);
    const int list_end(3);
    const std::string value_name("value");
    scope.add_value(value_name, boost::shared_ptr<const shade::parser::Value>(new shade::parser::ListValue(list_begin, list_end)));

    generate_function(generator, "shade_list_end", value_name);

    CHECK(check_reference_value(list_end, generator.get_element()->evaluate(scope)));
  }


  void generate_var_definition(shade::formatter::Generator& generator, const std::string& var_name, const int var_value)
  {
    generator.handle_var_definition_begin();
    generator.handle_var_definition_qualifier_list_begin();
    generator.handle_var_definition_qualifier_begin();
    generator.handle_qualifier(shade::formatter::Constants::input);
    generator.handle_var_definition_qualifier_end();
    generator.handle_var_definition_qualifier_list_end();
    generator.handle_var_definition_type_begin();
    generator.handle_type(shade::formatter::Constants::objref);
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier(var_name);
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_code_begin();
    generator.handle_literal_objref(var_value);
    generator.handle_var_definition_code_end();
    generator.handle_var_definition_end();
  }

  TEST_FIXTURE(Generators, TestVarDefinition)
  {
    const std::string var_name("i");
    const int var_value(42);
    shade::formatter::DumpGenerator dump_generator;

    generate_var_definition(dump_generator, var_name, var_value);
    generate_var_definition(generator, var_name, var_value);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_array_var_definition(shade::formatter::Generator& generator)
  {
    generator.handle_var_definition_begin();
    generator.handle_var_definition_type_begin();
    generator.handle_array_begin();
    generator.handle_array_type_begin();
    generator.handle_type(shade::formatter::Constants::float_);
    generator.handle_array_type_end();
    generator.handle_array_size_begin();
    generator.handle_literal_int(5);
    generator.handle_array_size_end();
    generator.handle_array_end();
    generator.handle_var_definition_type_end();
    generator.handle_var_definition_name_begin();
    generator.handle_identifier("values");
    generator.handle_var_definition_name_end();
    generator.handle_var_definition_end();
  }

  TEST_FIXTURE(Generators, TestArrayVarDefinition)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_array_var_definition(dump_generator);
    generate_array_var_definition(generator);

    CHECK(shade::formatter::check_dump_output(dump_generator.get_code(), this->get_code()));
  }


  void generate_forloop(shade::formatter::Generator& generator, CodeChain code, const std::string& loopvar, const std::string& list_begin, const std::string& list_end)
  {
    generator.handle_for_begin();
    generator.handle_for_init_begin();
    {
      generator.handle_var_definition_begin();
      generator.handle_var_definition_type_begin();
      generator.handle_type(shade::formatter::Constants::objref);
      generator.handle_var_definition_type_end();
      generator.handle_var_definition_name_begin();
      generator.handle_identifier(loopvar);
      generator.handle_var_definition_name_end();
      generator.handle_var_definition_code_begin();
      generator.handle_identifier(list_begin);
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
      generator.handle_identifier(loopvar);
      generator.handle_function_call_parameter_end();
      generator.handle_function_call_parameter_begin();
      generator.handle_identifier(list_end);
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
      generator.handle_identifier(loopvar);
      generator.handle_function_call_parameter_end();
      generator.handle_function_call_parameter_list_end();
      generator.handle_function_call_end();
    }
    generator.handle_for_increment_end();
    generator.handle_for_code_begin();
    {
      code(generator);
    }
    generator.handle_for_code_end();
    generator.handle_for_end();
  }

  TEST_FIXTURE(Generators, TestForLoopWithoutOptimization)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_forloop(dump_generator, generate_noop, "i", "list_begin", "list_end");
    generate_forloop(generator, generate_noop, "i", "list_begin", "list_end");

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_optimized_forloop(shade::formatter::Generator& generator, CodeChain code, const std::string& loopvar, int list_begin, int list_end)
  {
    for (int ref = list_begin; ref != list_end; ++ref)
    {
      generator.handle_block_begin();

      generator.handle_statement_begin();
      generator.handle_var_definition_begin();
      generator.handle_var_definition_type_begin();
      generator.handle_type(shade::formatter::Constants::objref);
      generator.handle_var_definition_type_end();
      generator.handle_var_definition_name_begin();
      generator.handle_identifier(loopvar);
      generator.handle_var_definition_name_end();
      generator.handle_var_definition_code_begin();
      generator.handle_literal_objref(ref);
      generator.handle_var_definition_code_end();
      generator.handle_var_definition_end();
      generator.handle_statement_end();

      code(generator);

      generator.handle_block_end();
    }
  }

  TEST_FIXTURE(Generators, TestForLoopWithOptimization)
  {
    const int list_begin(1);
    const int list_end(3);
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("list_begin", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(list_begin)));
    scope.add_value("list_end", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(list_end)));

    generate_optimized_forloop(dump_generator, generate_noop, "i", list_begin, list_end);
    generate_forloop(generator, generate_noop, "i", "list_begin", "list_end");

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_looped_noop(shade::formatter::Generator& generator)
  {
    generate_forloop(generator, generate_noop, "j", "i", "list_end");
  }

  void generate_optimized_looped_noop(shade::formatter::Generator& generator)
  {
    generate_optimized_forloop(generator, generate_noop, "j", 1, 2);
  }

  TEST_FIXTURE(Generators, TestNestedForLoopWithOptimization)
  {
    const int list_begin(1);
    const int list_end(2);
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("list_begin", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(list_begin)));
    scope.add_value("list_end", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(list_end)));

    generate_optimized_forloop(dump_generator, generate_optimized_looped_noop, "i", list_begin, list_end);
    generate_forloop(generator, generate_looped_noop, "i", "list_begin", "list_end");

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_switch(shade::formatter::Generator& generator, CodeChain code1, CodeChain code2, CodeChain default_)
  {
    generator.handle_switch_begin();
    generator.handle_switch_value_begin();
    generator.handle_identifier("refvalue");
    generator.handle_switch_value_end();
    generator.handle_switch_case_begin();
    generator.handle_identifier("value1");
    generator.handle_switch_case_end();
    generator.handle_switch_code_begin();
    code1(generator);
    generator.handle_switch_code_end();
    generator.handle_switch_case_begin();
    generator.handle_identifier("value2");
    generator.handle_switch_case_end();
    generator.handle_switch_code_begin();
    code2(generator);
    generator.handle_switch_code_end();
    generator.handle_switch_default_begin();
    default_(generator);
    generator.handle_switch_default_end();
    generator.handle_switch_end();
  }

  TEST_FIXTURE(Generators, TestSwitchWithoutOptimization)
  {
    shade::formatter::DumpGenerator dump_generator;

    generate_switch(dump_generator, generate_noop, generate_noop, generate_noop);
    generate_switch(generator, generate_noop, generate_noop, generate_noop);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_optimized_switch(shade::formatter::Generator& generator, CodeChain code)
  {
    generator.handle_block_begin();
    code(generator);
    generator.handle_block_end();
  }

  void generate_special_noop(shade::formatter::Generator& generator)
  {
    generator.handle_verbatim("special_noop;");
  }

  TEST_FIXTURE(Generators, TestSwitchWithOptimization)
  {
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("refvalue", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));
    scope.add_value("value1", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    scope.add_value("value2", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    generate_optimized_switch(dump_generator, generate_special_noop);
    generate_switch(generator, generate_noop, generate_special_noop, generate_noop);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  TEST_FIXTURE(Generators, TestDefaultInSwitchWithOptimization)
  {
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("refvalue", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(3)));
    scope.add_value("value1", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    scope.add_value("value2", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    generate_optimized_switch(dump_generator, generate_special_noop);
    generate_switch(generator, generate_noop, generate_noop, generate_special_noop);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  TEST_FIXTURE(Generators, TestNotTakenDefaultInSwitchWithOptimization)
  {
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("refvalue", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(3)));
    scope.add_value("value1", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    // "value2" intentionally not set

    generate_switch(dump_generator, generate_noop, generate_noop, generate_special_noop);
    generate_switch(generator, generate_noop, generate_noop, generate_special_noop);

    CHECK_EQUAL(dump_generator.get_code(), this->get_code());
  }

  void generate_looped_noop_for_function_definition(shade::formatter::Generator& generator)
  {
    generate_forloop(generator, generate_noop, "i", "u", "v");
  }

  void generate_optimized_looped_noop_for_function_definition(shade::formatter::Generator& generator)
  {
    generate_optimized_forloop(generator, generate_noop, "i", 1, 2);
  }

  TEST_FIXTURE(Generators, TestInstantiationOfOptimizedFunctionDefinition)
  {
    shade::formatter::DumpGenerator dump_generator;
    const std::string func_name("name");
    const std::string reference_func_name("reference_name");

    generate_function_definition(dump_generator, func_name, generate_optimized_looped_noop_for_function_definition);
    generate_function_definition(generator, reference_func_name, generate_looped_noop_for_function_definition);

    boost::shared_ptr<shade::parser::MethodTemplate> method(generator.get_method_template());
    CHECK(method);
    if (!method)
      return;

    shade::formatter::DumpGenerator instance_generator;
    shade::parser::ValueList parameters;
    parameters.add(boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    parameters.add(boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    CHECK(method->instantiate(func_name, parameters, instance_generator, global_scope, error_log, translate_function_name));
    CHECK_EQUAL(dump_generator.get_code(), instance_generator.get_code());
  }

  TEST_FIXTURE(Generators, TestReusingGeneratorAfterReset)
  {
    generator.handle_identifier("value");
    CHECK_EQUAL("<id>value</id>", this->get_code());
    generator.reset();
    generator.handle_identifier("other");
    CHECK_EQUAL("<id>other</id>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestReturn)
  {
    generator.handle_return_begin();
    generator.handle_literal_int(42);
    generator.handle_return_end();
    CHECK_EQUAL("<return><literal_int>42</literal_int></return>", this->get_code());
  }

  TEST_FIXTURE(Generators, TestSelf)
  {
    generator.handle_self_begin();
    generator.handle_self_name_begin();
    generator.handle_identifier("A_SELF");
    generator.handle_self_name_end();
    generator.handle_self_parameter_list_begin();
    generator.handle_self_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::vec3);
    generator.handle_self_parameter_type_end();
    generator.handle_self_parameter_name_begin();
    generator.handle_identifier("position");
    generator.handle_self_parameter_name_end();
    generator.handle_self_parameter_type_begin();
    generator.handle_type(shade::formatter::Constants::float_);
    generator.handle_self_parameter_type_end();
    generator.handle_self_parameter_name_begin();
    generator.handle_identifier("intensity");
    generator.handle_self_parameter_name_end();
    generator.handle_self_parameter_list_end();
    generator.handle_self_end();

    CHECK(shade::formatter::check_dump_output(
          "<self>"
          "<self_name><id>A_SELF</id></self_name>"
          "<self_param_list>"
          "<self_param_type><type>vec3</type></self_param_type>"
          "<self_param_name><id>position</id></self_param_name>"
          "<self_param_type><type>float</type></self_param_type>"
          "<self_param_name><id>intensity</id></self_param_name>"
          "</self_param_list>"
          "</self>"
          , this->get_code()));
  }

  TEST_FIXTURE(Generators, TestAppend)
  {
    generator.handle_block_begin();
    generator.handle_verbatim("noop;");
    generator.handle_block_end();

    shade::formatter::ElementGenerator generator2;
    generator2.handle_block_begin();
    generator2.handle_verbatim("noop;");
    generator2.handle_block_end();

    generator.append(generator2);

    CHECK_EQUAL("<block><verbatim>noop;</verbatim></block>"
        "<block><verbatim>noop;</verbatim></block>"
        , this->get_code());
  }

  TEST_FIXTURE(Generators, TestStateSettingInSequence)
  {
    const std::string var_name("refvalue");
    const int var_value(1);
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("value1", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    scope.add_value("value2", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    generate_var_definition(generator, var_name, var_value);
    generate_switch(generator, generate_special_noop, generate_noop, generate_noop);

    generate_var_definition(dump_generator, var_name, var_value);
    generate_optimized_switch(dump_generator, generate_special_noop);

    CHECK(shade::formatter::check_dump_output(dump_generator.get_code(), this->get_code()));
  }

  TEST_FIXTURE(Generators, TestBlockInhibitsStateSettingInSequence)
  {
    const std::string var_name("refvalue");
    const int var_value(1);
    shade::formatter::DumpGenerator dump_generator;
    scope.add_value("value1", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(1)));
    scope.add_value("value2", boost::shared_ptr<const shade::parser::Value>(new shade::parser::ReferenceValue(2)));

    generator.handle_block_begin();
    generate_var_definition(generator, var_name, var_value);
    generator.handle_block_end();
    generate_switch(generator, generate_special_noop, generate_noop, generate_noop);

    dump_generator.handle_block_begin();
    generate_var_definition(dump_generator, var_name, var_value);
    dump_generator.handle_block_end();
    generate_switch(dump_generator, generate_special_noop, generate_noop, generate_noop);

    CHECK(shade::formatter::check_dump_output(dump_generator.get_code(), this->get_code()));
  }

  TEST_FIXTURE(Generators, TestEnvironment)
  {
    generator.handle_environment_begin(shade::vertex_shader);
    generator.handle_return_begin();
    generator.handle_literal_int(42);
    generator.handle_return_end();
    generator.handle_environment_end();
    CHECK_EQUAL("<env value=\"SHADE_VERTEX_SHADER\"><return><literal_int>42</literal_int></return></env>", this->get_code());
  }

  void generate_call(shade::formatter::Generator& generator,
      const std::string& func_name, const int arg1,
      const int arg2 = -1, const bool use_objref = true)
  {
    generator.handle_function_call_begin();
    generator.handle_function_call_name_begin();
    generator.handle_identifier(func_name);
    generator.handle_function_call_name_end();
    generator.handle_function_call_parameter_list_begin();
    generator.handle_function_call_parameter_begin();
    generator.handle_literal_objref(arg1);
    generator.handle_function_call_parameter_end();
    if (arg2 != -1)
    {
      generator.handle_function_call_parameter_begin();
      if (use_objref)
        generator.handle_literal_objref(arg2);
      else
        generator.handle_literal_int(arg2);
      generator.handle_function_call_parameter_end();
    }
    generator.handle_function_call_parameter_list_end();
    generator.handle_function_call_end();
  }

  TEST_FIXTURE(Generators, TestGetBaseObjectEvaluation)
  {
    generate_call(generator, "shade_get_base_object", 85);
    CHECK(check_reference_value(1, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestGetObjectIndexEvaluation)
  {
    generate_call(generator, "shade_get_object_index", 85);
    CHECK(check_int_value(2, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestGetLInkedObjectEvaluation)
  {
    generate_call(generator, "shade_get_linked_object", 86, 43, true);
    CHECK(check_reference_value(84+1, generator.get_element()->evaluate(scope)));
  }

  TEST_FIXTURE(Generators, TestGetObjectEvaluation)
  {
    generate_call(generator, "shade_get_object", 2, 1, false);
    CHECK(check_reference_value(42+2, generator.get_element()->evaluate(scope)));
  }

}
