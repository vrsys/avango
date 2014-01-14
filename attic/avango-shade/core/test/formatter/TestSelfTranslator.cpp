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

#include "../../src/formatter/SelfTranslator.h"
#include "DumpGenerator.h"
#include "diff_dump.h"
#include <UnitTest++.h>


namespace
{
}

SUITE(TestSelfTranslator)
{
  struct FunctionFrame
  {
    shade::formatter::SelfTranslator translator;
    shade::formatter::DumpGenerator generator;

    void generate_parameters(const std::string& class_)
    {
      generator.handle_function_definition_begin();
      generator.handle_function_definition_return_type_begin();
      generator.handle_type(shade::formatter::Constants::void_);
      generator.handle_function_definition_return_type_end();
      generator.handle_function_definition_name_begin();
      generator.handle_identifier("func");
      generator.handle_function_definition_name_end();
      generator.handle_function_definition_parameter_list_begin();
      translator.generate(generator, class_);
      generator.handle_function_definition_parameter_list_end();
      generator.handle_function_definition_end();
    }
  };

  TEST_FIXTURE(FunctionFrame, TestSelf)
  {
    std::string class_("Class");

    translator.handle_self_begin();
    translator.handle_self_name_begin();
    translator.handle_identifier(class_);
    translator.handle_self_name_end();
    translator.handle_self_parameter_list_begin();
    translator.handle_self_parameter_type_begin();
    translator.handle_type(shade::formatter::Constants::vec3);
    translator.handle_self_parameter_type_end();
    translator.handle_self_parameter_name_begin();
    translator.handle_identifier("position");
    translator.handle_self_parameter_name_end();
    translator.handle_self_parameter_type_begin();
    translator.handle_type(shade::formatter::Constants::float_);
    translator.handle_self_parameter_type_end();
    translator.handle_self_parameter_name_begin();
    translator.handle_identifier("intensity");
    translator.handle_self_parameter_name_end();
    translator.handle_self_parameter_list_end();
    translator.handle_self_end();

    generate_parameters(class_);

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><type>void</type></funcdef_type>"
        "<funcdef_name><id>func</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
          "<funcdef_param_type><type>vec3</type></funcdef_param_type>"
          "<funcdef_param_name><id>position</id></funcdef_param_name>"
          "<funcdef_param_type><type>float</type></funcdef_param_type>"
          "<funcdef_param_name><id>intensity</id></funcdef_param_name>"
        "</funcdef_param_list>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(FunctionFrame, TestMultipleSelf)
  {
    std::string class1("Class1");
    std::string class2("Class2");

    translator.handle_self_begin();
    translator.handle_self_name_begin();
    translator.handle_identifier(class1);
    translator.handle_self_name_end();
    translator.handle_self_parameter_list_begin();
    translator.handle_self_parameter_type_begin();
    translator.handle_type(shade::formatter::Constants::vec3);
    translator.handle_self_parameter_type_end();
    translator.handle_self_parameter_name_begin();
    translator.handle_identifier("position");
    translator.handle_self_parameter_name_end();
    translator.handle_self_parameter_list_end();
    translator.handle_self_end();

    translator.handle_self_begin();
    translator.handle_self_name_begin();
    translator.handle_identifier(class2);
    translator.handle_self_name_end();
    translator.handle_self_parameter_list_begin();
    translator.handle_self_parameter_type_begin();
    translator.handle_type(shade::formatter::Constants::float_);
    translator.handle_self_parameter_type_end();
    translator.handle_self_parameter_name_begin();
    translator.handle_identifier("intensity");
    translator.handle_self_parameter_name_end();
    translator.handle_self_parameter_list_end();
    translator.handle_self_end();

    generate_parameters(class1);
    generate_parameters(class2);

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><type>void</type></funcdef_type>"
        "<funcdef_name><id>func</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
          "<funcdef_param_type><type>vec3</type></funcdef_param_type>"
          "<funcdef_param_name><id>position</id></funcdef_param_name>"
        "</funcdef_param_list>"
      "</funcdef>"
      "<funcdef>"
        "<funcdef_type><type>void</type></funcdef_type>"
        "<funcdef_name><id>func</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
          "<funcdef_param_type><type>float</type></funcdef_param_type>"
          "<funcdef_param_name><id>intensity</id></funcdef_param_name>"
        "</funcdef_param_list>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(FunctionFrame, TestEmptySelf)
  {
    std::string class_("Class");

    translator.handle_self_begin();
    translator.handle_self_name_begin();
    translator.handle_identifier(class_);
    translator.handle_self_name_end();
    translator.handle_self_parameter_list_begin();
    translator.handle_self_parameter_list_end();
    translator.handle_self_end();

    generate_parameters(class_);

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><type>void</type></funcdef_type>"
        "<funcdef_name><id>func</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
        "</funcdef_param_list>"
      "</funcdef>"
      , generator.get_code()));
  }

  TEST_FIXTURE(FunctionFrame, TestAppend)
  {
    std::string class_("Class");

    shade::formatter::SelfTranslator translator_other;
    translator_other.handle_self_begin();
    translator_other.handle_self_name_begin();
    translator_other.handle_identifier(class_);
    translator_other.handle_self_name_end();
    translator_other.handle_self_parameter_list_begin();
    translator_other.handle_self_parameter_type_begin();
    translator_other.handle_type(shade::formatter::Constants::vec3);
    translator_other.handle_self_parameter_type_end();
    translator_other.handle_self_parameter_name_begin();
    translator_other.handle_identifier("position");
    translator_other.handle_self_parameter_name_end();
    translator_other.handle_self_parameter_type_begin();
    translator_other.handle_type(shade::formatter::Constants::float_);
    translator_other.handle_self_parameter_type_end();
    translator_other.handle_self_parameter_name_begin();
    translator_other.handle_identifier("intensity");
    translator_other.handle_self_parameter_name_end();
    translator_other.handle_self_parameter_list_end();
    translator_other.handle_self_end();

    translator.append(translator_other);
    generate_parameters(class_);

    CHECK(shade::formatter::check_dump_output(
      "<funcdef>"
        "<funcdef_type><type>void</type></funcdef_type>"
        "<funcdef_name><id>func</id></funcdef_name>"
        "<funcdef_param_list>"
          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
          "<funcdef_param_name><id>self</id></funcdef_param_name>"
          "<funcdef_param_type><type>vec3</type></funcdef_param_type>"
          "<funcdef_param_name><id>position</id></funcdef_param_name>"
          "<funcdef_param_type><type>float</type></funcdef_param_type>"
          "<funcdef_param_name><id>intensity</id></funcdef_param_name>"
        "</funcdef_param_list>"
      "</funcdef>"
      , generator.get_code()));
  }


}
