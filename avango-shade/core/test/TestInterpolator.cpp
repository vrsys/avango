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

#include <shade/shaders/Interpolator.h>
#include <shade/Type.h>
#include <shade/FileResolver.h>
#include <shade/FileAccumulator.h>
#include <shade/PipelineState.h>
#include <shade/ShaderEnvironment.h>
#include <shade/types/vec2.h>
#include "StorageFormatter.h"
#include "../src/Component.h"
#include <boost/shared_ptr.hpp>
#include "formatter/diff_dump.h"
#include <UnitTest++.h>


SUITE(Interpolator)
{
  struct CompilationFixture
  {
    CompilationFixture(void) :
      interpolator(new shade::shaders::Interpolator<shade::vec2>),
      interpolator_id(interpolator->get_uniq_id()),
      component(interpolator),
      acc(resolver)
    {
      shade::PipelineState ps;
      component.setup(acc, ps);
      component.compile(fmt, acc, shade::geometry_shader);
    }

    boost::shared_ptr<shade::Shader> interpolator;
    std::string interpolator_id;
    shade::Component component;
    shade::FileResolver resolver;
    shade::FileAccumulator acc;
    shade::StorageFormatter fmt;
  };

  TEST_FIXTURE(CompilationFixture, Methods)
  {
    std::string class_initializeable("Initializeable");
    std::string class_interpolator("Interpolator_vec2");
    std::string initializer_name("init_post_geometry");

    CHECK(fmt.has_function_dispatch(class_initializeable, initializer_name, 0));
    CHECK_EQUAL(interpolator_id, fmt.get_function_dispatch(class_initializeable, initializer_name, 0).first);
    CHECK_EQUAL(initializer_name, fmt.get_function_dispatch(class_initializeable, initializer_name, 0).second);

    CHECK(fmt.has_inline_method(class_interpolator, initializer_name));
    CHECK_EQUAL(shade::formatter::Constants::void_,
        fmt.get_inline_method(class_interpolator, initializer_name).first);
    CHECK(shade::formatter::check_dump_output(
      "<statement>"
        "<vardef>"
          "<vardef_type><type>vec3</type></vardef_type>"
          "<vardef_name><id>uvw</id></vardef_name>"
          "<vardef_code>"
            "<func_call>"
              "<func_call_name><id>Geometry_get_uvw</id></func_call_name>"
              "<func_call_param_list>"
                "<func_call_param><id>geometry</id></func_call_param>"
              "</func_call_param_list>"
            "</func_call>"
          "</vardef_code>"
        "</vardef>"
      "</statement>"
      "<statement>"
        "<vardef>"
          "<vardef_type><type>vec2</type></vardef_type>"
          "<vardef_name><id>value</id></vardef_name>"
          "<vardef_code>"
            "<func_call>"
              "<func_call_name><operator>multiply</operator></func_call_name>"
              "<func_call_param_list>"
                "<func_call_param>"
                  "<func_call>"
                    "<func_call_name><operator>component_access</operator></func_call_name>"
                    "<func_call_param_list>"
                      "<func_call_param><id>uvw</id></func_call_param>"
                      "<func_call_param><id>x</id></func_call_param>"
                    "</func_call_param_list>"
                  "</func_call>"
                "</func_call_param>"
                "<func_call_param>"
                  "<func_call>"
                    "<func_call_name><id>Interpolator_vec2_get_input</id></func_call_name>"
                    "<func_call_param_list>"
                      "<func_call_param>"
                        "<func_call>"
                          "<func_call_name><id>shade_get_object</id></func_call_name>"
                          "<func_call_param_list>"
                            "<func_call_param><id>self</id></func_call_param>"
                            "<func_call_param><literal_int>0</literal_int></func_call_param>"
                          "</func_call_param_list>"
                        "</func_call>"
                      "</func_call_param>"
                    "</func_call_param_list>"
                  "</func_call>"
                "</func_call_param>"
              "</func_call_param_list>"
            "</func_call>"
          "</vardef_code>"
        "</vardef>"
      "</statement>"
      "<statement>"
        "<func_call>"
          "<func_call_name><operator>assignment_add</operator></func_call_name>"
          "<func_call_param_list>"
            "<func_call_param><id>value</id></func_call_param>"
            "<func_call_param>"
              "<func_call>"
                "<func_call_name><operator>multiply</operator></func_call_name>"
                "<func_call_param_list>"
                  "<func_call_param>"
                    "<func_call>"
                      "<func_call_name><operator>component_access</operator></func_call_name>"
                      "<func_call_param_list>"
                        "<func_call_param><id>uvw</id></func_call_param>"
                        "<func_call_param><id>y</id></func_call_param>"
                      "</func_call_param_list>"
                    "</func_call>"
                  "</func_call_param>"
                  "<func_call_param>"
                    "<func_call>"
                      "<func_call_name><id>Interpolator_vec2_get_input</id></func_call_name>"
                      "<func_call_param_list>"
                        "<func_call_param>"
                          "<func_call>"
                            "<func_call_name><id>shade_get_object</id></func_call_name>"
                            "<func_call_param_list>"
                              "<func_call_param><id>self</id></func_call_param>"
                              "<func_call_param><literal_int>1</literal_int></func_call_param>"
                            "</func_call_param_list>"
                          "</func_call>"
                        "</func_call_param>"
                      "</func_call_param_list>"
                    "</func_call>"
                  "</func_call_param>"
                "</func_call_param_list>"
              "</func_call>"
            "</func_call_param>"
          "</func_call_param_list>"
        "</func_call>"
      "</statement>"
      "<statement>"
        "<func_call>"
          "<func_call_name><operator>assignment_add</operator></func_call_name>"
          "<func_call_param_list>"
            "<func_call_param><id>value</id></func_call_param>"
            "<func_call_param>"
              "<func_call>"
                "<func_call_name><operator>multiply</operator></func_call_name>"
                "<func_call_param_list>"
                  "<func_call_param>"
                    "<func_call>"
                      "<func_call_name><operator>component_access</operator></func_call_name>"
                      "<func_call_param_list>"
                        "<func_call_param><id>uvw</id></func_call_param>"
                        "<func_call_param><id>z</id></func_call_param>"
                      "</func_call_param_list>"
                    "</func_call>"
                  "</func_call_param>"
                  "<func_call_param>"
                    "<func_call>"
                      "<func_call_name><id>Interpolator_vec2_get_input</id></func_call_name>"
                      "<func_call_param_list>"
                        "<func_call_param>"
                          "<func_call>"
                            "<func_call_name><id>shade_get_object</id></func_call_name>"
                            "<func_call_param_list>"
                              "<func_call_param><id>self</id></func_call_param>"
                              "<func_call_param><literal_int>2</literal_int></func_call_param>"
                            "</func_call_param_list>"
                          "</func_call>"
                        "</func_call_param>"
                      "</func_call_param_list>"
                    "</func_call>"
                  "</func_call_param>"
                "</func_call_param_list>"
              "</func_call>"
            "</func_call_param>"
          "</func_call_param_list>"
        "</func_call>"
      "</statement>"
      "<statement>"
        "<func_call>"
          "<func_call_name><id>Interpolator_vec2_set_output</id></func_call_name>"
          "<func_call_param_list>"
            "<func_call_param><id>self</id></func_call_param>"
            "<func_call_param><id>value</id></func_call_param>"
          "</func_call_param_list>"
        "</func_call>"
      "</statement>"
      , fmt.get_inline_method(class_interpolator, initializer_name).second));
    CHECK_EQUAL(0u, fmt.get_inline_method_args(class_interpolator, initializer_name).size());
  }

  TEST_FIXTURE(CompilationFixture, Initializers)
  {
    CHECK_EQUAL(2u, fmt.get_initializers().size());
    CHECK_EQUAL("shade_initialize_Initializeable_init_geometry", fmt.get_initializers()[0].first);
    CHECK_EQUAL(0u, fmt.get_initializers()[0].second);
    CHECK_EQUAL("shade_initialize_post_Initializeable_init_post_geometry", fmt.get_initializers()[1].first);
    CHECK_EQUAL(0u, fmt.get_initializers()[1].second);
  }
}
