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

#include "../../src/formatter/GL2Generator.h"
#include <shade/formatter/Constants.h>
#include <shade/ObjectMap.h>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <UnitTest++.h>
#include <iostream>

namespace
{
struct VertexShaderFixture
{
    VertexShaderFixture(void) : generator(shade::vertex_shader) {}

    shade::formatter::GL2Generator generator;
};
} // namespace

SUITE(TestGL2Generator)
{
    TEST_FIXTURE(VertexShaderFixture, TestIdentifier)
    {
        generator.handle_identifier("value");
        CHECK_EQUAL("value", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestVerbatim)
    {
        std::string verbatim("A B C");
        generator.handle_verbatim(verbatim);
        CHECK_EQUAL(verbatim, generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestObjrefLiteral)
    {
        shade::ObjectMap::Index literal = 5;
        generator.handle_literal_objref(literal);
        CHECK_EQUAL(boost::lexical_cast<std::string>(literal), generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestListLiteral)
    {
        shade::ObjectMap::Index begin = 5;
        shade::ObjectMap::Index end = 6;
        generator.handle_literal_list(begin, end);

        CHECK_EQUAL("LIST(" + boost::lexical_cast<std::string>(begin) + ", " + boost::lexical_cast<std::string>(end) + ")", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestIntLiteral)
    {
        int literal = 5;
        generator.handle_literal_int(literal);
        CHECK_EQUAL(boost::lexical_cast<std::string>(literal), generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFloatLiteral)
    {
        float v = 1.;
        generator.handle_literal_float(v);

        std::string result(generator.get_code());
        CHECK_CLOSE(v, boost::lexical_cast<float>(result), 0.01);
        // Always use a 'e' to indicate floating point literal
        CHECK(result.find("e") != std::string::npos);
    }

    TEST_FIXTURE(VertexShaderFixture, TestVec2Literal)
    {
        generator.handle_literal_vec2(1.1, 2.2);

        std::string result(generator.get_code());
        float x, y;
        std::sscanf(result.c_str(), "vec2(%f, %f)", &x, &y);
        CHECK_CLOSE(1.1, x, 0.01);
        CHECK_CLOSE(2.2, y, 0.01);
    }

    TEST_FIXTURE(VertexShaderFixture, TestVec3Literal)
    {
        generator.handle_literal_vec3(1.1, 2.2, 3.3);

        std::string result(generator.get_code());
        float x, y, z;
        std::sscanf(result.c_str(), "vec3(%f, %f, %f)", &x, &y, &z);
        CHECK_CLOSE(1.1, x, 0.01);
        CHECK_CLOSE(2.2, y, 0.01);
        CHECK_CLOSE(3.3, z, 0.01);
    }

    TEST_FIXTURE(VertexShaderFixture, TestVec4Literal)
    {
        generator.handle_literal_vec4(1.1, 2.2, 3.3, 4.4);

        std::string result(generator.get_code());
        float x, y, z, w;
        std::sscanf(result.c_str(), "vec4(%f, %f, %f, %f)", &x, &y, &z, &w);
        CHECK_CLOSE(1.1, x, 0.01);
        CHECK_CLOSE(2.2, y, 0.01);
        CHECK_CLOSE(3.3, z, 0.01);
        CHECK_CLOSE(4.4, w, 0.01);
    }

    TEST_FIXTURE(VertexShaderFixture, TestEmptySwitch)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier("value");
        generator.handle_switch_value_end();
        generator.handle_switch_end();

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSingleCaseSwitch)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier("value");
        generator.handle_switch_value_end();
        generator.handle_switch_case_begin();
        generator.handle_identifier("const_value");
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_verbatim("    noop;\n");
        generator.handle_switch_code_end();
        generator.handle_switch_end();

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "  if (shade_comparison_value == const_value)\n"
                    "  {\n"
                    "    noop;\n"
                    "  }\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestMultipleCaseSwitch)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier("value");
        generator.handle_switch_value_end();
        for(int i = 0; i != 3; ++i)
        {
            generator.handle_switch_case_begin();
            generator.handle_identifier("const_value" + boost::lexical_cast<std::string>(i));
            generator.handle_switch_case_end();
            generator.handle_switch_code_begin();
            generator.handle_verbatim("    noop" + boost::lexical_cast<std::string>(i) + ";\n");
            generator.handle_switch_code_end();
        }
        generator.handle_switch_end();

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "  if (shade_comparison_value == const_value0)\n"
                    "  {\n"
                    "    noop0;\n"
                    "  }\n"
                    "  else if (shade_comparison_value == const_value1)\n"
                    "  {\n"
                    "    noop1;\n"
                    "  }\n"
                    "  else if (shade_comparison_value == const_value2)\n"
                    "  {\n"
                    "    noop2;\n"
                    "  }\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSwitchWithDefault)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier("value");
        generator.handle_switch_value_end();
        generator.handle_switch_case_begin();
        generator.handle_identifier("const_value");
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_verbatim("    noop;\n");
        generator.handle_switch_code_end();
        generator.handle_switch_default_begin();
        generator.handle_verbatim("    noop_default;\n");
        generator.handle_switch_default_end();
        generator.handle_switch_end();

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "  if (shade_comparison_value == const_value)\n"
                    "  {\n"
                    "    noop;\n"
                    "  }\n"
                    "  else\n"
                    "  {\n"
                    "    noop_default;\n"
                    "  }\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSwitchWithoutCaseButWithDefault)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier("value");
        generator.handle_switch_value_end();
        generator.handle_switch_default_begin();
        generator.handle_verbatim("    noop_default;\n");
        generator.handle_switch_default_end();
        generator.handle_switch_end();

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "  {\n"
                    "    noop_default;\n"
                    "  }\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestMultipleSwitch)
    {
        for(int j = 0; j != 2; ++j)
        {
            generator.handle_switch_begin();
            generator.handle_switch_value_begin();
            generator.handle_identifier("value");
            generator.handle_switch_value_end();
            for(int i = 0; i != 2; ++i)
            {
                generator.handle_switch_case_begin();
                generator.handle_identifier("const_value" + boost::lexical_cast<std::string>(i));
                generator.handle_switch_case_end();
                generator.handle_switch_code_begin();
                generator.handle_verbatim("    noop" + boost::lexical_cast<std::string>(i) + ";\n");
                generator.handle_switch_code_end();
            }
            generator.handle_switch_end();
        }

        CHECK_EQUAL("{\n"
                    "  int shade_comparison_value = value;\n"
                    "  if (shade_comparison_value == const_value0)\n"
                    "  {\n"
                    "    noop0;\n"
                    "  }\n"
                    "  else if (shade_comparison_value == const_value1)\n"
                    "  {\n"
                    "    noop1;\n"
                    "  }\n"
                    "}\n"
                    "{\n"
                    "  int shade_comparison_value = value;\n"
                    "  if (shade_comparison_value == const_value0)\n"
                    "  {\n"
                    "    noop0;\n"
                    "  }\n"
                    "  else if (shade_comparison_value == const_value1)\n"
                    "  {\n"
                    "    noop1;\n"
                    "  }\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestForLoop)
    {
        generator.handle_for_begin();
        generator.handle_for_init_begin();
        generator.handle_verbatim("int i=0");
        generator.handle_for_init_end();
        generator.handle_for_condition_begin();
        generator.handle_verbatim("i != 5");
        generator.handle_for_condition_end();
        generator.handle_for_increment_begin();
        generator.handle_verbatim("++i");
        generator.handle_for_increment_end();
        generator.handle_for_code_begin();
        generator.handle_verbatim("  do_something(i);\n");
        generator.handle_for_code_end();
        generator.handle_for_end();

        CHECK_EQUAL("for (int i=0; i != 5; ++i)\n"
                    "{\n"
                    "  do_something(i);\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSimpleVarDefinition)
    {
        generator.handle_var_definition_begin();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::float_);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier("x");
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_end();

        CHECK_EQUAL("float x", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestVarDefinitionWithQualifiers)
    {
        generator.handle_var_definition_begin();
        generator.handle_var_definition_qualifier_list_begin();
        generator.handle_var_definition_qualifier_begin();
        generator.handle_qualifier(shade::formatter::Constants::input);
        generator.handle_var_definition_qualifier_end();
        generator.handle_var_definition_qualifier_list_end();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::vec3);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier("v");
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_end();

        CHECK_EQUAL("attribute vec3 v", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestVarDefinitionWithAssignment)
    {
        generator.handle_var_definition_begin();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::float_);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier("v");
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_code_begin();
        generator.handle_verbatim("5");
        generator.handle_var_definition_code_end();
        generator.handle_var_definition_end();

        CHECK_EQUAL("float v = 5", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestArrayVarDefinition)
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

        CHECK_EQUAL("float values[5]", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestStatement)
    {
        generator.handle_statement_begin();
        generator.handle_verbatim("5");
        generator.handle_statement_end();

        CHECK_EQUAL("5;\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestBlock)
    {
        generator.handle_block_begin();
        generator.handle_verbatim("5;\n");
        generator.handle_block_end();

        CHECK_EQUAL("{\n"
                    "5;\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestNestedBlocks)
    {
        generator.handle_block_begin();
        generator.handle_block_begin();
        generator.handle_statement_begin();
        generator.handle_verbatim("5");
        generator.handle_statement_end();
        generator.handle_block_end();
        generator.handle_statement_begin();
        generator.handle_verbatim("6");
        generator.handle_statement_end();
        generator.handle_block_end();

        CHECK_EQUAL("{\n"
                    "  {\n"
                    "    5;\n"
                    "  }\n"
                    "  6;\n"
                    "}\n",
                    generator.get_code());
    }

    void generate_function_definition(
        shade::formatter::GL2Generator & generator, int parameters, bool declaration = false, shade::formatter::Constants::Qualifier qualifier = shade::formatter::Constants::none)
    {
        generator.handle_function_definition_begin();
        generator.handle_function_definition_return_type_begin();
        generator.handle_type(shade::formatter::Constants::float_);
        generator.handle_function_definition_return_type_end();
        generator.handle_function_definition_name_begin();
        generator.handle_identifier("f");
        generator.handle_function_definition_name_end();
        generator.handle_function_definition_parameter_list_begin();
        for(int i = 0; i != parameters; ++i)
        {
            generator.handle_function_definition_parameter_type_begin();
            generator.handle_type(shade::formatter::Constants::vec3);
            generator.handle_function_definition_parameter_type_end();
            generator.handle_function_definition_parameter_name_begin();
            generator.handle_identifier("v" + boost::lexical_cast<std::string>(i));
            generator.handle_function_definition_parameter_name_end();
        }
        generator.handle_function_definition_parameter_list_end();
        if(!declaration)
        {
            generator.handle_function_definition_code_begin();
            generator.handle_verbatim("noop;\n");
            generator.handle_function_definition_code_end();
        }
        generator.handle_function_definition_end();
    }

    TEST_FIXTURE(VertexShaderFixture, TestSimpleFunctionDefinition)
    {
        generate_function_definition(generator, 1);

        CHECK_EQUAL("float f(vec3 v0)\n"
                    "{\n"
                    "noop;\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFunctionDefinition)
    {
        generate_function_definition(generator, 3);

        CHECK_EQUAL("float f(vec3 v0, vec3 v1, vec3 v2)\n"
                    "{\n"
                    "noop;\n"
                    "}\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFunctionDeclaration)
    {
        generate_function_definition(generator, 3, true);

        CHECK_EQUAL("float f(vec3 v0, vec3 v1, vec3 v2);\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSimpleFunctionCall)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("f");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("v");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();

        CHECK_EQUAL("f(v)", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFunctionCallWithMultipleArguments)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("f");
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

        CHECK_EQUAL("f(u, v)", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFunctionCallWithMultipleNestedArguments)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("f");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("u");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("g");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("x");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("y");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("v");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();

        CHECK_EQUAL("f(u, g(x, y), v)", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestBinaryOperator)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::equal);
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

        CHECK_EQUAL("(u)==(v)", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestFunctionCallWithMultipleNestedOperator)
    {
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("f");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("u");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::post_increment);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("v");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();

        CHECK_EQUAL("f(u, (v)++)", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSimpleReturn)
    {
        generator.handle_return_begin();
        generator.handle_return_end();

        CHECK_EQUAL("return;\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestReturnWithValue)
    {
        generator.handle_return_begin();
        generator.handle_literal_int(42);
        generator.handle_return_end();

        CHECK_EQUAL("return 42;\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestSelf)
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
        generator.handle_self_parameter_list_end();
        generator.handle_self_end();

        CHECK_EQUAL("#define A_SELF in OBJREF self, in vec3 position\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestEmptySelf)
    {
        generator.handle_self_begin();
        generator.handle_self_name_begin();
        generator.handle_identifier("A_SELF");
        generator.handle_self_name_end();
        generator.handle_self_parameter_list_begin();
        generator.handle_self_parameter_list_end();
        generator.handle_self_end();

        CHECK_EQUAL("#define A_SELF in OBJREF self\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestAppend)
    {
        generator.handle_return_begin();
        generator.handle_literal_int(42);
        generator.handle_return_end();

        shade::formatter::GL2Generator generator2(shade::vertex_shader);
        generator2.handle_return_begin();
        generator2.handle_literal_int(42);
        generator2.handle_return_end();

        generator.append(generator2);

        CHECK_EQUAL("return 42;\n"
                    "return 42;\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, InsertMacro)
    {
        generator.insert_define("SHADE_VERTEX_SHADER", "1");
        generator.insert_define("SHADE_SOMETHING", "A B C");
        CHECK_EQUAL("#define SHADE_VERTEX_SHADER 1\n"
                    "#define SHADE_SOMETHING A B C\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, InsertExtension)
    {
        generator.insert_extension("GLEXT_something", shade::formatter::GL2Generator::Extension::enable);
        generator.insert_extension("GLEXT_something_else", shade::formatter::GL2Generator::Extension::require);
        CHECK_EQUAL("#extension GLEXT_something: enable\n"
                    "#extension GLEXT_something_else: require\n",
                    generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, InsertVersion)
    {
        generator.insert_sl_version(110);
        CHECK_EQUAL("#version 110\n", generator.get_code());
    }

    TEST_FIXTURE(VertexShaderFixture, TestEnvironment)
    {
        generator.handle_environment_begin(shade::vertex_shader);
        generator.handle_return_begin();
        generator.handle_literal_int(42);
        generator.handle_return_end();
        generator.handle_environment_end();
        generator.handle_environment_begin(shade::fragment_shader);
        generator.handle_return_begin();
        generator.handle_literal_int(24);
        generator.handle_return_end();
        generator.handle_environment_end();

        CHECK_EQUAL("return 42;\n", generator.get_code());
    }
}
