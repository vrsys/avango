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

#include "../src/formatter/GLSLFormatter.h"
#include "DumpGenerator.h"
#include "diff_dump.h"
#include <shade/Type.h>
#include <shade/types/vec3.h>
#include <shade/types/vec2.h>
#include <shade/types/float.h>
#include <shade/types/sampler2D.h>
#include <shade/types/void.h>
#include <UnitTest++.h>

SUITE(TestGLSLFormatter)
{
    TEST(WriteAttributes)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.insert_attribute(shade::formatter::Constants::vec3, "obj_0x0", "color");
        fmt.insert_attribute(shade::formatter::Constants::sampler2D, "obj_0x1", "tex", shade::formatter::Constants::uniform);
        fmt.insert_attribute(shade::formatter::Constants::float_, "obj_0x2", "luminance", shade::formatter::Constants::output);

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>vec3</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x0_color</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_qualifier_list>"
                                                          "<vardef_qualifier><qualifier>uniform</qualifier></vardef_qualifier>"
                                                          "</vardef_qualifier_list>"
                                                          "<vardef_type><type>sampler2D</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x1_tex</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_qualifier_list>"
                                                          "<vardef_qualifier><qualifier>output</qualifier></vardef_qualifier>"
                                                          "</vardef_qualifier_list>"
                                                          "<vardef_type><type>float</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x2_luminance</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>",
                                                          result_generator.get_code()));
    }

    TEST(WriteAttributesWithInit)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        boost::shared_ptr<shade::formatter::Generator> generator;
        generator = fmt.begin_insert_init_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position", shade::formatter::Constants::const_);
        generator->handle_literal_vec3(1.1, 2.2, 3.3);
        fmt.end_insert_init_attribute();

        generator = fmt.begin_insert_init_attribute(shade::formatter::Constants::list, "obj_0x1", "lights");
        generator->handle_literal_list(0, 1);
        fmt.end_insert_init_attribute();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<statement>"
                                                          "<vardef>"
                                                          "<vardef_qualifier_list>"
                                                          "<vardef_qualifier><qualifier>const</qualifier></vardef_qualifier>"
                                                          "</vardef_qualifier_list>"
                                                          "<vardef_type><type>vec3</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x3_position</id></vardef_name>"
                                                          "<vardef_code>"
                                                          "<literal_vec3>1.1 2.2 3.3</literal_vec3>"
                                                          "</vardef_code>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>LIST</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x1_lights</id></vardef_name>"
                                                          "<vardef_code>"
                                                          "<literal_list>0 1</literal_list>"
                                                          "</vardef_code>"
                                                          "</vardef>"
                                                          "</statement>",
                                                          result_generator.get_code()));
    }

    TEST(WriteAttributesAsArray)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        fmt.setup(4, shade::formatter::Constants::triangles);

        fmt.insert_array_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position");

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<statement>"
                                                          "<vardef>"
                                                          "<vardef_qualifier_list>"
                                                          "<vardef_qualifier><qualifier>input</qualifier></vardef_qualifier>"
                                                          "</vardef_qualifier_list>"
                                                          "<vardef_type>"
                                                          "<array>"
                                                          "<array_type>"
                                                          "<type>vec3</type>"
                                                          "</array_type>"
                                                          "<array_size>"
                                                          "<literal_int>3</literal_int>"
                                                          "</array_size>"
                                                          "</array>"
                                                          "</vardef_type>"
                                                          "<vardef_name><id>obj_0x3_position</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>",
                                                          result_generator.get_code()));
    }

    TEST(WriteLocalAttributesInGeometryShader)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        fmt.setup(4, shade::formatter::Constants::triangles);

        fmt.insert_multiple_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position");

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>vec3</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x3_0_position</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>vec3</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x3_1_position</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>vec3</type></vardef_type>"
                                                          "<vardef_name><id>obj_0x3_2_position</id></vardef_name>"
                                                          "</vardef>"
                                                          "</statement>",
                                                          result_generator.get_code()));
    }

    TEST(WriteSelfArguments)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_self_argument("LightSource");
        fmt.insert_self_argument(shade::formatter::Constants::float_, "intensity");
        fmt.end_self_argument();

        fmt.begin_self_argument("PointLightSource");
        fmt.insert_self_argument(shade::formatter::Constants::vec3, "position");
        fmt.insert_self_argument(shade::formatter::Constants::vec3, "fragpos");
        fmt.end_self_argument();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output("<self>"
                                                          "<self_name><id>LightSource_SELF</id></self_name>"
                                                          "<self_param_list>"
                                                          "<self_param_type><type>float</type></self_param_type>"
                                                          "<self_param_name><id>intensity</id></self_param_name>"
                                                          "</self_param_list>"
                                                          "</self>",
                                                          result));

        CHECK(shade::formatter::check_partial_dump_output("<self>"
                                                          "<self_name><id>PointLightSource_SELF</id></self_name>"
                                                          "<self_param_list>"
                                                          "<self_param_type><type>vec3</type></self_param_type>"
                                                          "<self_param_name><id>position</id></self_param_name>"
                                                          "<self_param_type><type>vec3</type></self_param_type>"
                                                          "<self_param_name><id>fragpos</id></self_param_name>"
                                                          "</self_param_list>"
                                                          "</self>",
                                                          result));
    }

    struct EmptyDispatcherFixture
    {
        shade::vec3<> type_a;
        shade::sampler2D type_b;
        shade::float_<> type_c;

        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt;

        EmptyDispatcherFixture(void) : fmt(shade::vertex_shader, result_generator)
        {
            fmt.begin_dispatcher("Class", "Test", type_a.get_formatter_type());
            fmt.begin_dispatcher_add_arg(type_b.get_formatter_type());
            fmt.begin_dispatcher_add_arg(type_c.get_formatter_type());
            fmt.begin_dispatcher_finish();
            boost::shared_ptr<shade::formatter::Generator> generator(fmt.set_dispatcher_return());
            generator->handle_literal_vec3(0., 0., 0.);
            fmt.end_dispatcher();
            fmt.generate(result_generator);
        }

        std::string get_expected_header(void)
        {
            return "<funcdef>"
                   "<funcdef_type><type>vec3</type></funcdef_type>"
                   "<funcdef_name><id>Class_Test</id></funcdef_name>"
                   "<funcdef_param_list>"
                   "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                   "<funcdef_param_name><id>self</id></funcdef_param_name>"
                   "<funcdef_param_type><type>sampler2D</type></funcdef_param_type>"
                   "<funcdef_param_name><id>arg1</id></funcdef_param_name>"
                   "<funcdef_param_type><type>float</type></funcdef_param_type>"
                   "<funcdef_param_name><id>arg2</id></funcdef_param_name>"
                   "</funcdef_param_list>";
        }
    };

    TEST_FIXTURE(EmptyDispatcherFixture, WriteEmptyDispatcher)
    {
        CHECK(shade::formatter::check_partial_dump_output(get_expected_header() + "<funcdef_code>"
                                                                                  "<return><literal_vec3>0 0 0</literal_vec3></return>"
                                                                                  "</funcdef_code>"
                                                                                  "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST_FIXTURE(EmptyDispatcherFixture, WriteEmptyDispatcherPre) { CHECK(shade::formatter::check_partial_dump_output(get_expected_header() + "</funcdef>", result_generator.get_code())); }

    std::string output_simple_dispatcher(shade::ShaderEnvironment env, bool use_inline = false)
    {
        shade::vec3<> type_a;
        shade::sampler2D type_b;
        shade::float_<> type_c;

        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(env, result_generator);

        fmt.begin_dispatcher("Class", "Test", type_a.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_b.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_c.get_formatter_type());
        fmt.begin_dispatcher_finish();
        if(use_inline)
            fmt.insert_inline_dispatcher(1, "obj_0x0", "LightSource", "illuminate");
        else
            fmt.insert_dispatcher(1, "obj_0x0", "LightSource_illuminate_impl");
        fmt.insert_dispatcher_add_arg("position");
        fmt.insert_dispatcher_finish();
        fmt.insert_dispatcher(3, "obj_0x2", "PointLightSource_illuminate_impl");
        fmt.insert_dispatcher_add_arg("intensity");
        fmt.insert_dispatcher_add_arg("direction");
        fmt.insert_dispatcher_finish();
        boost::shared_ptr<shade::formatter::Generator> generator(fmt.set_dispatcher_return());
        generator->handle_literal_vec3(0., 0., 0.);
        fmt.end_dispatcher();

        fmt.generate(result_generator);
        return result_generator.get_code();
    }

    std::string output_expected_simple_dispatcher(shade::ShaderEnvironment env)
    {
        std::ostringstream hout;
        hout << "<funcdef>"
                "<funcdef_type><type>vec3</type></funcdef_type>"
                "<funcdef_name><id>Class_Test</id></funcdef_name>"
                "<funcdef_param_list>"
                "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                "<funcdef_param_name><id>self</id></funcdef_param_name>"
                "<funcdef_param_type><type>sampler2D</type></funcdef_param_type>"
                "<funcdef_param_name><id>arg1</id></funcdef_param_name>"
                "<funcdef_param_type><type>float</type></funcdef_param_type>"
                "<funcdef_param_name><id>arg2</id></funcdef_param_name>"
                "</funcdef_param_list>"
                "<funcdef_code>";
        if(env == shade::geometry_shader)
        {
            hout << "<statement>"
                    "<vardef>"
                    "<vardef_type>"
                    "<type>OBJREF</type>"
                    "</vardef_type>"
                    "<vardef_name>"
                    "<id>base_self</id>"
                    "</vardef_name>"
                    "<vardef_code>"
                    "<func_call>"
                    "<func_call_name>"
                    "<id>shade_get_base_object</id>"
                    "</func_call_name>"
                    "<func_call_param_list>"
                    "<func_call_param>"
                    "<id>self</id>"
                    "</func_call_param>"
                    "</func_call_param_list>"
                    "</func_call>"
                    "</vardef_code>"
                    "</vardef>"
                    "</statement>";
        }
        hout << "<switch>";
        if(env == shade::geometry_shader)
            hout << "<switch_value><id>base_self</id></switch_value>";
        else
            hout << "<switch_value><id>self</id></switch_value>";
        hout << "<switch_case><literal_objref>1</literal_objref></switch_case>"
                "<switch_code>"
                "<return>"
                "<func_call>"
                "<func_call_name><id>LightSource_illuminate_impl</id></func_call_name>"
                "<func_call_param_list>";
        if(env == shade::geometry_shader)
            hout << "<func_call_param><id>self</id></func_call_param>";
        else
            hout << "<func_call_param><literal_objref>1</literal_objref></func_call_param>";
        hout << "<func_call_param><id>obj_0x0_position</id></func_call_param>"
                "<func_call_param><id>arg1</id></func_call_param>"
                "<func_call_param><id>arg2</id></func_call_param>"
                "</func_call_param_list>"
                "</func_call>"
                "</return>"
                "</switch_code>"
                "<switch_case><literal_objref>3</literal_objref></switch_case>"
                "<switch_code>"
                "<return>"
                "<func_call>"
                "<func_call_name><id>PointLightSource_illuminate_impl</id></func_call_name>"
                "<func_call_param_list>";
        if(env == shade::geometry_shader)
            hout << "<func_call_param><id>self</id></func_call_param>";
        else
            hout << "<func_call_param><literal_objref>3</literal_objref></func_call_param>";
        hout << "<func_call_param><id>obj_0x2_intensity</id></func_call_param>"
                "<func_call_param><id>obj_0x2_direction</id></func_call_param>"
                "<func_call_param><id>arg1</id></func_call_param>"
                "<func_call_param><id>arg2</id></func_call_param>"
                "</func_call_param_list>"
                "</func_call>"
                "</return>"
                "</switch_code>"
                "</switch>"
                "<return><literal_vec3>0 0 0</literal_vec3></return>"
                "</funcdef_code>"
                "</funcdef>";

        return hout.str();
    }

    TEST(WriteDispatcher) { CHECK(shade::formatter::check_partial_dump_output(output_expected_simple_dispatcher(shade::vertex_shader), output_simple_dispatcher(shade::vertex_shader))); }

    TEST(WriteInlineDispatcher) { CHECK(shade::formatter::check_partial_dump_output(output_expected_simple_dispatcher(shade::vertex_shader), output_simple_dispatcher(shade::vertex_shader, true))); }

    TEST(WriteDispatcherInGeometryShader)
    {
        CHECK(shade::formatter::check_partial_dump_output(output_expected_simple_dispatcher(shade::geometry_shader), output_simple_dispatcher(shade::geometry_shader)));
    }

    TEST(WriteVoidDispatcher)
    {
        shade::void_<> type_a;
        shade::sampler2D type_b;
        shade::float_<> type_c;

        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_dispatcher("Class", "Test", type_a.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_b.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_c.get_formatter_type());
        fmt.begin_dispatcher_finish();
        fmt.insert_dispatcher(1, "obj_0x0", "LightSource_illuminate_impl");
        fmt.insert_dispatcher_add_arg("position");
        fmt.insert_dispatcher_finish();
        fmt.set_dispatcher_return();
        fmt.end_dispatcher();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>Class_Test</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>self</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>sampler2D</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>arg1</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>float</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>arg2</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<switch>"
                                                          "<switch_value><id>self</id></switch_value>"
                                                          "<switch_case><literal_objref>1</literal_objref></switch_case>"
                                                          "<switch_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>LightSource_illuminate_impl</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>1</literal_objref></func_call_param>"
                                                          "<func_call_param><id>obj_0x0_position</id></func_call_param>"
                                                          "<func_call_param><id>arg1</id></func_call_param>"
                                                          "<func_call_param><id>arg2</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</switch_code>"
                                                          "</switch>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    void generate_property_begin(shade::formatter::Generator & generator, shade::formatter::Constants::Type retval, const std::string& method, bool setter = false, bool declaration = false)
    {
        generator.handle_function_definition_begin();
        generator.handle_function_definition_return_type_begin();
        generator.handle_type(retval);
        generator.handle_function_definition_return_type_end();
        generator.handle_function_definition_name_begin();
        generator.handle_identifier(method);
        generator.handle_function_definition_name_end();
        generator.handle_function_definition_parameter_list_begin();
        generator.handle_function_definition_parameter_type_begin();
        generator.handle_type(shade::formatter::Constants::objref);
        generator.handle_function_definition_parameter_type_end();
        generator.handle_function_definition_parameter_name_begin();
        generator.handle_identifier("self");
        generator.handle_function_definition_parameter_name_end();
        if(setter)
        {
            generator.handle_function_definition_parameter_type_begin();
            generator.handle_type(retval);
            generator.handle_function_definition_parameter_type_end();
            generator.handle_function_definition_parameter_name_begin();
            generator.handle_identifier("arg");
            generator.handle_function_definition_parameter_name_end();
        }
        generator.handle_function_definition_parameter_list_end();
        if(!declaration)
            generator.handle_function_definition_code_begin();
    }

    void generate_assign_base_self(shade::formatter::Generator & generator)
    {
        generator.handle_statement_begin();
        generator.handle_var_definition_begin();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::objref);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier("base_self");
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_code_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("shade_get_base_object");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("self");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_var_definition_code_end();
        generator.handle_var_definition_end();
        generator.handle_statement_end();
    }

    void generate_assign_index(shade::formatter::Generator & generator)
    {
        generator.handle_statement_begin();
        generator.handle_var_definition_begin();
        generator.handle_var_definition_type_begin();
        generator.handle_type(shade::formatter::Constants::objref);
        generator.handle_var_definition_type_end();
        generator.handle_var_definition_name_begin();
        generator.handle_identifier("index");
        generator.handle_var_definition_name_end();
        generator.handle_var_definition_code_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier("shade_get_object_index");
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("self");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_var_definition_code_end();
        generator.handle_var_definition_end();
        generator.handle_statement_end();
    }

    void generate_switch_begin(shade::formatter::Generator & generator, const std::string& self)
    {
        generator.handle_switch_begin();
        generator.handle_switch_value_begin();
        generator.handle_identifier(self);
        generator.handle_switch_value_end();
    }

    void generate_switch_case_return_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& var)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_identifier(var);
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_switch_case_return_indexed_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& var)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::array_access);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(var);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("index");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_switch_case_return_deferred_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& func, shade::ObjectMap::Index obj, bool geometry = false)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier(func);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        if(geometry)
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_get_linked_object");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("self");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_objref(obj);
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        else
        {
            generator.handle_literal_objref(obj);
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_switch_case_assign_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& var)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::assignment);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(var);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("arg");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_switch_case_assign_indexed_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& var)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::assignment);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_operator(shade::formatter::Constants::array_access);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier(var);
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("index");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("arg");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_switch_case_assign_deferred_value(shade::formatter::Generator & generator, shade::ObjectMap::Index value, const std::string& func, shade::ObjectMap::Index obj, bool geometry = false)
    {
        generator.handle_switch_case_begin();
        generator.handle_literal_objref(value);
        generator.handle_switch_case_end();
        generator.handle_switch_code_begin();
        generator.handle_return_begin();
        generator.handle_function_call_begin();
        generator.handle_function_call_name_begin();
        generator.handle_identifier(func);
        generator.handle_function_call_name_end();
        generator.handle_function_call_parameter_list_begin();
        generator.handle_function_call_parameter_begin();
        if(geometry)
        {
            generator.handle_function_call_begin();
            generator.handle_function_call_name_begin();
            generator.handle_identifier("shade_get_linked_object");
            generator.handle_function_call_name_end();
            generator.handle_function_call_parameter_list_begin();
            generator.handle_function_call_parameter_begin();
            generator.handle_identifier("self");
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_begin();
            generator.handle_literal_objref(obj);
            generator.handle_function_call_parameter_end();
            generator.handle_function_call_parameter_list_end();
            generator.handle_function_call_end();
        }
        else
        {
            generator.handle_literal_objref(obj);
        }
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_begin();
        generator.handle_identifier("arg");
        generator.handle_function_call_parameter_end();
        generator.handle_function_call_parameter_list_end();
        generator.handle_function_call_end();
        generator.handle_return_end();
        generator.handle_switch_code_end();
    }

    void generate_default_return_vec3(shade::formatter::Generator & generator)
    {
        generator.handle_switch_end();

        generator.handle_return_begin();
        generator.handle_literal_vec3(0., 0., 0.);
        generator.handle_return_end();
    }

    void generate_default_return_float(shade::formatter::Generator & generator)
    {
        generator.handle_switch_end();

        generator.handle_return_begin();
        generator.handle_literal_float(0.);
        generator.handle_return_end();
    }

    void generate_property_end(shade::formatter::Generator & generator)
    {
        generator.handle_function_definition_code_end();
        generator.handle_function_definition_end();
    }

    TEST(WritePropertyDispatcher)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_return_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_return_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_assign_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_assign_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteMultiplePropertyDispatcher)
    {
        shade::vec3<> type_value1;
        shade::float_<> type_value2;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_property_dispatcher("Class", "value1", type_value1.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        generator = fmt.set_property_dispatcher_return();
        type_value1.generate_constructor(*generator);
        fmt.end_property_dispatcher();
        fmt.begin_property_dispatcher("Class", "value2", type_value2.get_formatter_type());
        fmt.insert_property_dispatcher(2, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value2.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value1");
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_return_value(expected_generator, 1, "obj_0x0_value1");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value1", true);
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_assign_value(expected_generator, 1, "obj_0x0_value1");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::float_, "Class_get_value2");
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_return_value(expected_generator, 2, "obj_0x2_value2");
        generate_default_return_float(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::float_, "Class_set_value2", true);
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_assign_value(expected_generator, 2, "obj_0x2_value2");
        generate_default_return_float(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value1", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value1", true, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::float_, "Class_get_value2", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::float_, "Class_set_value2", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WritePropertyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::geometry_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_return_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_return_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_assign_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_assign_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteInputPropertyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::geometry_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_array_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_assign_base_self(expected_generator);
        generate_assign_index(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_return_indexed_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_return_indexed_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_assign_base_self(expected_generator);
        generate_assign_index(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_assign_indexed_value(expected_generator, 1, "obj_0x0_value");
        generate_switch_case_assign_indexed_value(expected_generator, 3, "obj_0x2_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteLocalPropertyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        fmt.setup(4, shade::formatter::Constants::lines);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_multiple_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_return_value(expected_generator, 1, "obj_0x0_0_value");
        generate_switch_case_return_value(expected_generator, 5, "obj_0x0_1_value");
        generate_switch_case_return_value(expected_generator, 3, "obj_0x2_0_value");
        generate_switch_case_return_value(expected_generator, 7, "obj_0x2_1_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_assign_value(expected_generator, 1, "obj_0x0_0_value");
        generate_switch_case_assign_value(expected_generator, 5, "obj_0x0_1_value");
        generate_switch_case_assign_value(expected_generator, 3, "obj_0x2_0_value");
        generate_switch_case_assign_value(expected_generator, 7, "obj_0x2_1_value");
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteDeferredProperteyDispatcher)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_deferred_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_deferred_property_dispatcher(1, 2, "Value", "get_value", "Value", "set_value");
        fmt.insert_deferred_property_dispatcher(3, 5, "ReadonlyValue", "get", "", "");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_return_deferred_value(expected_generator, 1, "Value_get_value", 2);
        generate_switch_case_return_deferred_value(expected_generator, 3, "ReadonlyValue_get", 5);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_switch_begin(expected_generator, "self");
        generate_switch_case_assign_deferred_value(expected_generator, 1, "Value_set_value", 2);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteDeferredProperteyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::geometry_shader, result_generator);
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_deferred_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_deferred_property_dispatcher(1, 2, "Value", "get_value", "Value", "set_value");
        fmt.insert_deferred_property_dispatcher(3, 5, "ReadonlyValue", "get", "", "");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_return_deferred_value(expected_generator, 1, "Value_get_value", 2, true);
        generate_switch_case_return_deferred_value(expected_generator, 3, "ReadonlyValue_get", 5, true);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_assign_deferred_value(expected_generator, 1, "Value_set_value", 2, true);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        shade::formatter::DumpGenerator expected_pre_generator;
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_get_value", false, true);
        expected_pre_generator.handle_function_definition_end();
        generate_property_begin(expected_pre_generator, shade::formatter::Constants::vec3, "Class_set_value", true, true);
        expected_pre_generator.handle_function_definition_end();

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output(expected_generator.get_code(), result));
        CHECK(shade::formatter::check_partial_dump_output(expected_pre_generator.get_code(), result));
    }

    TEST(WriteMainEntry)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.invoke("Class", "shade");

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>main</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>0</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST(WriteInitializeOneClass)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_initialize("some_function");
        fmt.initialize(1, "Class", "shade");
        fmt.end_initialize();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>some_function</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>1</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>main</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>some_function</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST(WriteInitializeMultipleClasses)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_initialize("some_function");
        fmt.initialize(1, "Class", "shade");
        fmt.initialize(2, "Class2", "shade");
        fmt.initialize(8, "Class3", "init");
        fmt.end_initialize();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>some_function</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>1</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class2_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>2</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class3_init</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>8</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>main</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>some_function</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST(WriteInitializeMultipleClassesWithoutInvocation)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_initialize("some_function", false);
        fmt.initialize(1, "Class", "shade");
        fmt.initialize(2, "Class2", "shade");
        fmt.initialize(8, "Class3", "init");
        fmt.end_initialize();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>some_function</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>1</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class2_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>2</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class3_init</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>8</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>main</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST(WriteInitializeMultipleClassesAndInvoke)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.begin_initialize("some_function");
        fmt.initialize(1, "Class", "shade");
        fmt.initialize(2, "Class2", "shade");
        fmt.initialize(8, "Class3", "init");
        fmt.end_initialize();
        fmt.invoke("Class", "shade");

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>some_function</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>1</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class2_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>2</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class3_init</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>8</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>main</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>some_function</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name><id>Class_shade</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_objref>0</literal_objref></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result_generator.get_code()));
    }

    void generate_return(shade::formatter::Generator & generator)
    {
        generator.handle_return_begin();
        generator.handle_literal_vec3(1.1, 2.2, 3.3);
        generator.handle_return_end();
    }

    TEST(InsertInlineMethod)
    {
        std::string content("return something;\n");

        shade::vec3<> type_a;
        shade::vec2<> type_b;
        shade::float_<> type_c;

        shade::formatter::DumpGenerator dump_generator;
        generate_return(dump_generator);

        std::string class_("Class");
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);
        fmt.begin_self_argument(class_);
        fmt.insert_self_argument(shade::formatter::Constants::vec3, "data");
        fmt.end_self_argument();
        fmt.begin_inline_method(class_, "test", type_a.get_formatter_type());
        fmt.add_arg_inline_method(type_b.get_formatter_type());
        fmt.add_arg_inline_method(type_c.get_formatter_type());
        boost::shared_ptr<shade::formatter::Generator> generator(fmt.insert_inline_method());
        generate_return(*generator);
        fmt.end_inline_method();

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<funcdef>"
                                                          "<funcdef_type><type>vec3</type></funcdef_type>"
                                                          "<funcdef_name><id>Class_test_impl</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>self</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>vec3</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>data</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>vec2</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>arg1</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>float</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>arg2</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>" +
                                                              dump_generator.get_code() +
                                                              "</funcdef_code>"
                                                              "</funcdef>",
                                                          result_generator.get_code()));
    }

    TEST(WritePredefinedFunctionsInGeometryShader)
    {
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::geometry_shader, result_generator);
        fmt.setup(42, shade::formatter::Constants::triangles);

        shade::formatter::DumpGenerator expected_generator;
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_get_value");
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_return_deferred_value(expected_generator, 1, "Value_get_value", 2, true);
        generate_switch_case_return_deferred_value(expected_generator, 3, "ReadonlyValue_get", 5, true);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);
        generate_property_begin(expected_generator, shade::formatter::Constants::vec3, "Class_set_value", true);
        generate_assign_base_self(expected_generator);
        generate_switch_begin(expected_generator, "base_self");
        generate_switch_case_assign_deferred_value(expected_generator, 1, "Value_set_value", 2, true);
        generate_default_return_vec3(expected_generator);
        generate_property_end(expected_generator);

        fmt.generate(result_generator);
        std::string result(result_generator.get_code());
        CHECK(shade::formatter::check_partial_dump_output("<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>int</type></vardef_type>"
                                                          "<vardef_name><id>shade_num_classes</id></vardef_name>"
                                                          "<vardef_code><literal_int>42</literal_int></vardef_code>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<vardef>"
                                                          "<vardef_type><type>int</type></vardef_type>"
                                                          "<vardef_name><id>shade_vertices_in</id></vardef_name>"
                                                          "<vardef_code><literal_int>3</literal_int></vardef_code>"
                                                          "</vardef>"
                                                          "</statement>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>OBJREF</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_get_base_object</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>index</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<return>"
                                                          "<func_call>"
                                                          "<func_call_name><type>int</type></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><id>mod</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>index</id></func_call_param>"
                                                          "<func_call_param><literal_int>42</literal_int></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</return>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>OBJREF</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_get_object_index</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>index</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<return>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>divide</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>subtract</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>index</id></func_call_param>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><id>shade_get_base_object</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>index</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "<func_call_param><literal_int>42</literal_int></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</return>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>OBJREF</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_get_linked_object</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>reference</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>index</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<return>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>add</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><id>shade_get_base_object</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>index</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>multiply</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_int>42</literal_int></func_call_param>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><id>shade_get_object_index</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>reference</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</return>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>OBJREF</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_get_object</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>obj</id></funcdef_param_name>"
                                                          "<funcdef_param_type><type>OBJREF</type></funcdef_param_type>"
                                                          "<funcdef_param_name><id>index</id></funcdef_param_name>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<return>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>add</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><id>shade_get_base_object</id></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><id>obj</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "<func_call_param>"
                                                          "<func_call>"
                                                          "<func_call_name><operator>multiply</operator></func_call_name>"
                                                          "<func_call_param_list>"
                                                          "<func_call_param><literal_int>42</literal_int></func_call_param>"
                                                          "<func_call_param><id>index</id></func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</func_call_param>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</return>"
                                                          "</funcdef_code>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_initialize_post</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "</funcdef>"
                                                          "<funcdef>"
                                                          "<funcdef_type><type>void</type></funcdef_type>"
                                                          "<funcdef_name><id>shade_emit_vertex</id></funcdef_name>"
                                                          "<funcdef_param_list>"
                                                          "</funcdef_param_list>"
                                                          "<funcdef_code>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name>"
                                                          "<id>shade_initialize_post</id>"
                                                          "</func_call_name>"
                                                          "<func_call_param_list>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "<statement>"
                                                          "<func_call>"
                                                          "<func_call_name>"
                                                          "<id>EmitVertex</id>"
                                                          "</func_call_name>"
                                                          "<func_call_param_list>"
                                                          "</func_call_param_list>"
                                                          "</func_call>"
                                                          "</statement>"
                                                          "</funcdef_code>"
                                                          "</funcdef>",
                                                          result));
    }

    TEST(InsertSources)
    {
        std::string content("return something;");
        shade::formatter::DumpGenerator result_generator;
        shade::GLSLFormatter fmt(shade::vertex_shader, result_generator);

        fmt.insert_sources(content);

        fmt.generate(result_generator);
        CHECK(shade::formatter::check_partial_dump_output("<verbatim>" + content + "</verbatim>", result_generator.get_code()));
    }
}
