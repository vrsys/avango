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

#include <shade/shaders/Value.h>
#include <shade/Type.h>
#include <shade/FileResolver.h>
#include <shade/FileAccumulator.h>
#include <shade/PipelineState.h>
#include <shade/ShaderEnvironment.h>
#include <shade/types/vec3.h>
#include <shade/types/float.h>
#include <shade/types/uniform.h>
#include <shade/types/inout.h>
#include <shade/types/local.h>
#include "StorageFormatter.h"
#include "formatter/DumpGenerator.h"
#include "formatter/diff_dump.h"
#include "../src/Component.h"
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>


SUITE(Value)
{
  TEST(GetClassName)
  {
    shade::shaders::Value<shade::vec3, shade::local > value;
    CHECK_EQUAL("Value_vec3", value.get_class_name());

    shade::shaders::Value<shade::float_> value_float;
    CHECK_EQUAL("Value_float", value_float.get_class_name());
  }

  TEST(IndirectGetClassName)
  {
    shade::shaders::Value<shade::vec3> value;
    shade::Shader* ptr = &value;
    CHECK_EQUAL("Value_vec3", ptr->get_class_name());

    shade::shaders::Value<shade::float_> value_float;
    ptr = &value_float;
    CHECK_EQUAL("Value_float", ptr->get_class_name());
  }

  TEST(GetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3>().get();
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_get_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_get_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_get_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      , generator.get_code()));
   }

  TEST(SetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3>().set(shade::vec3<shade::Type>());
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      , generator.get_code()));
  }

  TEST(UniformGetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3, shade::uniform>().get();
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      , generator.get_code()));
  }

  TEST(UniformSetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3, shade::uniform>().set(shade::vec3<shade::Type>());
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<id>value</id>"
        "</return>"
      "</env>"
      , generator.get_code()));
  }

  TEST(VaryingGetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3, shade::raw_varying>().get();
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_inout_0_1_vec3_get_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_inout_0_1_vec3_get_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<literal_vec3>0 0 0</literal_vec3>"
        "</return>"
      "</env>"
      , generator.get_code()));
  }

  TEST(VaryingSetterMethod)
  {
    shade::vec3<shade::Type> type = shade::shaders::Value<shade::vec3, shade::raw_varying>().set(shade::vec3<shade::Type>());
    CHECK(type.has_inline_function());
    shade::formatter::DumpGenerator generator;
    type.generate_inline(generator);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_inout_0_1_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_inout_0_1_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<literal_vec3>0 0 0</literal_vec3>"
        "</return>"
      "</env>"
      , generator.get_code()));
  }


  struct CompilationFixture
  {
    CompilationFixture(void) :
      value(new shade::shaders::Value<shade::vec3>),
      value_id(value->get_uniq_id()),
      component(value),
      acc(resolver)
    {
      shade::PipelineState ps;
      component.setup(acc, ps);
      component.compile(fmt, acc, shade::vertex_shader);
    }

    boost::shared_ptr<shade::Shader> value;
    std::string value_id;
    shade::Component component;
    shade::FileResolver resolver;
    shade::FileAccumulator acc;
    shade::StorageFormatter fmt;
  };

  TEST_FIXTURE(CompilationFixture, Attribute)
  {
    CHECK(fmt.has_attribute(value_id, "value"));
    CHECK_EQUAL(shade::formatter::Constants::vec3,
        fmt.get_attribute(value_id, "value").type);
    CHECK_EQUAL(shade::formatter::Constants::none,
        fmt.get_attribute(value_id, "value").qualifier);
  }

  TEST_FIXTURE(CompilationFixture, Methods)
  {
    std::string class_settable("Settable_vec3");
    std::string class_value("Value_vec3");
    shade::formatter::Constants::Type vec3_type = shade::formatter::Constants::vec3;

    CHECK(fmt.has_function_dispatch(class_settable, "set", 0));
    CHECK_EQUAL(value_id, fmt.get_function_dispatch(class_settable, "set", 0).first);
    CHECK_EQUAL("set", fmt.get_function_dispatch(class_settable, "set", 0).second);

    CHECK(fmt.has_inline_method(class_value, "set"));
    CHECK_EQUAL(vec3_type, fmt.get_inline_method(class_value, "set").first);
    CHECK(shade::formatter::check_dump_output(
      "<env value=\"SHADE_VERTEX_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_FRAGMENT_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      "<env value=\"SHADE_GEOMETRY_SHADER\">"
        "<return>"
          "<func_call>"
            "<func_call_name>"
              "<id>Value_vec3_set_value</id>"
            "</func_call_name>"
            "<func_call_param_list>"
              "<func_call_param>"
                "<id>self</id>"
              "</func_call_param>"
              "<func_call_param>"
                "<id>arg1</id>"
              "</func_call_param>"
            "</func_call_param_list>"
          "</func_call>"
        "</return>"
      "</env>"
      , fmt.get_inline_method(class_value, "set").second));
    CHECK_EQUAL(1u, fmt.get_inline_method_args(class_value, "set").size());
    CHECK_EQUAL(vec3_type, fmt.get_inline_method_args(class_value, "set")[0]);
  }

}
