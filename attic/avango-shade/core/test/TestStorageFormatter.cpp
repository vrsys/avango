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

#include "StorageFormatter.h"
#include <shade/Type.h>
#include <shade/types/vec3.h>
#include <shade/types/vec2.h>
#include <shade/types/float.h>
#include <shade/types/sampler2D.h>
#include <shade/formatter/Generator.h>
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>

SUITE(StorageFormatter)
{
    TEST(EmptyFunctionDispatcher)
    {
        shade::vec3<> type_a;
        shade::sampler2D type_b;
        shade::float_<> type_c;

        shade::StorageFormatter fmt;
        fmt.begin_dispatcher("Class", "Test", type_a.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_b.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_c.get_formatter_type());
        fmt.begin_dispatcher_finish();
        boost::shared_ptr<shade::formatter::Generator> generator(fmt.set_dispatcher_return());
        type_a.generate_constructor(*generator);
        fmt.end_dispatcher();

        CHECK_EQUAL(3u, fmt.get_function_args("Class", "Test").size());
        CHECK_EQUAL(shade::formatter::Constants::sampler2D, fmt.get_function_args("Class", "Test")[1]);
        CHECK_EQUAL(shade::formatter::Constants::float_, fmt.get_function_args("Class", "Test")[2]);
        CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt.get_function_return("Class", "Test"));
    };

    TEST(FunctionDispatcher)
    {
        shade::vec3<> type_a;
        shade::sampler2D type_b;
        shade::float_<> type_c;

        shade::StorageFormatter fmt;
        fmt.begin_dispatcher("Class", "Test", type_a.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_b.get_formatter_type());
        fmt.begin_dispatcher_add_arg(type_c.get_formatter_type());
        fmt.begin_dispatcher_finish();

        fmt.insert_dispatcher(1, "obj_0x7", "Test_1_impl");
        fmt.insert_dispatcher_finish();
        fmt.insert_dispatcher(2, "obj_0x11", "Test_2_impl");
        fmt.insert_dispatcher_add_arg("position");
        fmt.insert_dispatcher_add_arg("direction");
        fmt.insert_dispatcher_finish();

        boost::shared_ptr<shade::formatter::Generator> generator(fmt.set_dispatcher_return());
        type_a.generate_constructor(*generator);
        fmt.end_dispatcher();

        CHECK(fmt.has_function_dispatch("Class", "Test", 1));
        CHECK_EQUAL("obj_0x7", fmt.get_function_dispatch("Class", "Test", 1).first);
        CHECK_EQUAL("Test_1_impl", fmt.get_function_dispatch("Class", "Test", 1).second);
        CHECK_EQUAL("Test_2_impl", fmt.get_function_dispatch("Class", "Test", 2).second);
        CHECK(fmt.has_function_dispatch("Class", "Test", 2));
        CHECK_EQUAL(2u, fmt.get_function_dispatch_args("Class", "Test", 2).size());
        CHECK_EQUAL("position", fmt.get_function_dispatch_args("Class", "Test", 2)[0]);
        CHECK_EQUAL("direction", fmt.get_function_dispatch_args("Class", "Test", 2)[1]);
    };

    TEST(WritePropertyDispatcher)
    {
        shade::vec3<> type_value;
        shade::StorageFormatter fmt;
        boost::shared_ptr<shade::formatter::Generator> generator;

        fmt.begin_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        generator = fmt.set_property_dispatcher_return();
        type_value.generate_constructor(*generator);
        fmt.end_property_dispatcher();

        CHECK_EQUAL(shade::StorageFormatter::normal, fmt.get_property_dispatch_type("Class", "value"));
        CHECK_EQUAL("obj_0x0", fmt.get_property_dispatch("Class", "value", 1));
        CHECK_EQUAL("obj_0x2", fmt.get_property_dispatch("Class", "value", 3));
        CHECK_EQUAL("", fmt.get_property_dispatch("Class", "value", 5));
        CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt.get_property_dispatch_return("Class", "value"));
    }

    TEST(WriteInputPropertyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::StorageFormatter fmt;

        fmt.begin_array_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        fmt.end_property_dispatcher();

        CHECK_EQUAL(shade::StorageFormatter::array, fmt.get_property_dispatch_type("Class", "value"));
        CHECK_EQUAL("obj_0x0", fmt.get_property_dispatch("Class", "value", 1));
        CHECK_EQUAL("obj_0x2", fmt.get_property_dispatch("Class", "value", 3));
        CHECK_EQUAL("", fmt.get_property_dispatch("Class", "value", 5));
    }

    TEST(WriteLocalPropertyDispatcherInGeometryShader)
    {
        shade::vec3<> type_value;
        shade::StorageFormatter fmt;

        fmt.begin_multiple_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_property_dispatcher(1, "obj_0x0");
        fmt.insert_property_dispatcher(3, "obj_0x2");
        fmt.end_property_dispatcher();

        CHECK_EQUAL(shade::StorageFormatter::multiple, fmt.get_property_dispatch_type("Class", "value"));
        CHECK_EQUAL("obj_0x0", fmt.get_property_dispatch("Class", "value", 1));
        CHECK_EQUAL("obj_0x2", fmt.get_property_dispatch("Class", "value", 3));
        CHECK_EQUAL("", fmt.get_property_dispatch("Class", "value", 5));
    }

    TEST(WriteDeferredProperteyDispatcher)
    {
        shade::vec3<> type_value;
        shade::StorageFormatter fmt;

        fmt.begin_deferred_property_dispatcher("Class", "value", type_value.get_formatter_type());
        fmt.insert_deferred_property_dispatcher(1, 2, "Value", "get_value", "Value", "set_value");
        fmt.insert_deferred_property_dispatcher(3, 5, "ReadonlyValue", "get", "", "");
        fmt.end_property_dispatcher();

        CHECK_EQUAL(shade::StorageFormatter::deferred, fmt.get_property_dispatch_type("Class", "value"));
        CHECK_EQUAL(2u, fmt.get_deferred_property_dispatch("Class", "value", 1).object);
        CHECK_EQUAL("Value", fmt.get_deferred_property_dispatch("Class", "value", 1).getter_class);
        CHECK_EQUAL("get_value", fmt.get_deferred_property_dispatch("Class", "value", 1).getter_method);
        CHECK_EQUAL("Value", fmt.get_deferred_property_dispatch("Class", "value", 1).setter_class);
        CHECK_EQUAL("set_value", fmt.get_deferred_property_dispatch("Class", "value", 1).setter_method);
        CHECK_EQUAL(5u, fmt.get_deferred_property_dispatch("Class", "value", 3).object);
        CHECK_EQUAL("ReadonlyValue", fmt.get_deferred_property_dispatch("Class", "value", 3).getter_class);
        CHECK_EQUAL("get", fmt.get_deferred_property_dispatch("Class", "value", 3).getter_method);
        CHECK(fmt.get_deferred_property_dispatch("Class", "value", 3).setter_method.empty());
    }

    TEST(WriteAttributes)
    {
        shade::StorageFormatter gmt;

        gmt.insert_attribute(shade::formatter::Constants::vec3, "obj_0x0", "color");
        gmt.insert_attribute(shade::formatter::Constants::sampler2D, "obj_0x1", "tex", shade::formatter::Constants::uniform);
        gmt.insert_attribute(shade::formatter::Constants::float_, "obj_0x2", "luminance", shade::formatter::Constants::output);

        CHECK(gmt.has_attribute("obj_0x0", "color"));
        CHECK(gmt.has_attribute("obj_0x1", "tex"));
        CHECK(gmt.has_attribute("obj_0x2", "luminance"));

        CHECK_EQUAL(shade::formatter::Constants::vec3, gmt.get_attribute("obj_0x0", "color").type);
        CHECK_EQUAL(shade::formatter::Constants::none, gmt.get_attribute("obj_0x0", "color").qualifier);

        CHECK_EQUAL(shade::formatter::Constants::sampler2D, gmt.get_attribute("obj_0x1", "tex").type);
        CHECK_EQUAL(shade::formatter::Constants::uniform, gmt.get_attribute("obj_0x1", "tex").qualifier);

        CHECK_EQUAL(shade::formatter::Constants::float_, gmt.get_attribute("obj_0x2", "luminance").type);
        CHECK_EQUAL(shade::formatter::Constants::output, gmt.get_attribute("obj_0x2", "luminance").qualifier);
    }

    TEST(WriteAttributesWithInit)
    {
        shade::StorageFormatter gmt;
        shade::vec3<> value(1.1, 2.2, 3.3);
        boost::shared_ptr<shade::formatter::Generator> generator;

        generator = gmt.begin_insert_init_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position", shade::formatter::Constants::const_);
        value.generate_constructor(*generator);
        gmt.end_insert_init_attribute();
        gmt.insert_attribute(shade::formatter::Constants::list, "obj_0x1", "lights");

        CHECK(gmt.has_attribute("obj_0x3", "position"));
        CHECK(gmt.has_attribute("obj_0x1", "lights"));

        CHECK_EQUAL(shade::formatter::Constants::vec3, gmt.get_attribute("obj_0x3", "position").type);
        CHECK_EQUAL(shade::formatter::Constants::const_, gmt.get_attribute("obj_0x3", "position").qualifier);
        CHECK_EQUAL("<literal_vec3>1.1 2.2 3.3</literal_vec3>", gmt.get_attribute("obj_0x3", "position").initializer);

        CHECK_EQUAL(shade::formatter::Constants::list, gmt.get_attribute("obj_0x1", "lights").type);
        CHECK_EQUAL(shade::formatter::Constants::none, gmt.get_attribute("obj_0x1", "lights").qualifier);
        CHECK(gmt.get_attribute("obj_0x1", "lights").initializer.empty());
    }

    TEST(WriteAttributesAsArray)
    {
        shade::StorageFormatter gmt;

        gmt.insert_array_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position");
        gmt.insert_attribute(shade::formatter::Constants::list, "obj_0x1", "lights");

        CHECK(gmt.has_attribute("obj_0x3", "position"));
        CHECK(gmt.has_attribute("obj_0x1", "lights"));

        CHECK_EQUAL(shade::formatter::Constants::vec3, gmt.get_attribute("obj_0x3", "position").type);
        CHECK_EQUAL(shade::formatter::Constants::input, gmt.get_attribute("obj_0x3", "position").qualifier);
        CHECK(gmt.get_attribute("obj_0x3", "position").initializer.empty());
        CHECK(gmt.get_attribute("obj_0x3", "position").is_array);

        CHECK_EQUAL(shade::formatter::Constants::list, gmt.get_attribute("obj_0x1", "lights").type);
        CHECK_EQUAL(shade::formatter::Constants::none, gmt.get_attribute("obj_0x1", "lights").qualifier);
        CHECK(gmt.get_attribute("obj_0x1", "lights").initializer.empty());
        CHECK(!gmt.get_attribute("obj_0x1", "lights").is_array);
    }

    TEST(WriteAttributesAsMultiple)
    {
        shade::StorageFormatter gmt(shade::geometry_shader);
        gmt.setup(4, shade::formatter::Constants::triangles);

        gmt.insert_multiple_attribute(shade::formatter::Constants::vec3, "obj_0x3", "position");

        CHECK(gmt.has_attribute("obj_0x3", "position"));
        CHECK_EQUAL(shade::formatter::Constants::vec3, gmt.get_attribute("obj_0x3", "position").type);
        CHECK(gmt.get_attribute("obj_0x3", "position").initializer.empty());
        CHECK(!gmt.get_attribute("obj_0x3", "position").is_array);
        CHECK_EQUAL(3u, gmt.get_attribute("obj_0x3", "position").num_copies);
    }

    TEST(WriteSelfArguments)
    {
        shade::StorageFormatter fmt;

        fmt.begin_self_argument("LightSource");
        fmt.insert_self_argument(shade::formatter::Constants::float_, "intensity");
        fmt.end_self_argument();

        fmt.begin_self_argument("PointLightSource");
        fmt.insert_self_argument(shade::formatter::Constants::vec3, "position");
        fmt.insert_self_argument(shade::formatter::Constants::vec3, "fragpos");
        fmt.end_self_argument();

        CHECK_EQUAL(1u, fmt.get_self_argument("LightSource").size());
        CHECK_EQUAL(shade::formatter::Constants::float_, fmt.get_self_argument("LightSource")[0].first);
        CHECK_EQUAL("intensity", fmt.get_self_argument("LightSource")[0].second);
        CHECK_EQUAL(2u, fmt.get_self_argument("PointLightSource").size());
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_self_argument("PointLightSource")[0].first);
        CHECK_EQUAL("position", fmt.get_self_argument("PointLightSource")[0].second);
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_self_argument("PointLightSource")[1].first);
        CHECK_EQUAL("fragpos", fmt.get_self_argument("PointLightSource")[1].second);
    }

    TEST(MainEntry)
    {
        shade::StorageFormatter fmt;
        std::string main("shade");
        std::string class_("Class");
        fmt.invoke(class_, main);
        CHECK_EQUAL(fmt.get_method_name(class_, main), fmt.get_invoke());
    }

    TEST(InitializeOneClass)
    {
        shade::StorageFormatter fmt;
        std::string main("shade");
        std::string class_("Class");
        std::string function("something");
        fmt.begin_initialize(function);
        fmt.initialize(1, class_, main);
        fmt.end_initialize();
        CHECK_EQUAL(1u, fmt.get_initializers().size());
        CHECK_EQUAL(fmt.get_method_name(function, fmt.get_method_name(class_, main)), fmt.get_initializers().begin()->first);
        CHECK_EQUAL(1u, fmt.get_initializers().begin()->second);
    }

    TEST(InitializeMultipleClasses)
    {
        shade::StorageFormatter fmt;
        std::string main1("shade");
        std::string main2("init");
        std::string class1("Class1");
        std::string class2("Class2");
        std::string function("something");
        fmt.begin_initialize(function);
        fmt.initialize(1, class1, main1);
        fmt.initialize(2, class2, main1);
        fmt.initialize(8, class1, main2);
        fmt.end_initialize();

        CHECK_EQUAL(3u, fmt.get_initializers().size());

        CHECK_EQUAL(fmt.get_method_name(function, fmt.get_method_name(class1, main1)), fmt.get_initializers()[0].first);
        CHECK_EQUAL(1u, fmt.get_initializers()[0].second);
        CHECK_EQUAL(fmt.get_method_name(function, fmt.get_method_name(class2, main1)), fmt.get_initializers()[1].first);
        CHECK_EQUAL(2u, fmt.get_initializers()[1].second);
        CHECK_EQUAL(fmt.get_method_name(function, fmt.get_method_name(class1, main2)), fmt.get_initializers()[2].first);
        CHECK_EQUAL(8u, fmt.get_initializers()[2].second);
    }

    TEST(InsertInlineMethod)
    {
        std::string content("return something;\n");

        shade::vec3<> type_a;
        shade::vec2<> type_b;
        shade::float_<> type_c;

        shade::StorageFormatter fmt;

        fmt.begin_inline_method("Class", "test", type_a.get_formatter_type());
        fmt.add_arg_inline_method(type_b.get_formatter_type());
        fmt.add_arg_inline_method(type_c.get_formatter_type());
        boost::shared_ptr<shade::formatter::Generator> generator(fmt.insert_inline_method());
        generator->handle_verbatim(content);
        fmt.end_inline_method();

        CHECK(fmt.has_inline_method("Class", "test"));
        CHECK_EQUAL(type_a.get_formatter_type(), fmt.get_inline_method("Class", "test").first);
        CHECK_EQUAL("<verbatim>" + content + "</verbatim>", fmt.get_inline_method("Class", "test").second);
        CHECK_EQUAL(2u, fmt.get_inline_method_args("Class", "test").size());
        CHECK_EQUAL(type_b.get_formatter_type(), fmt.get_inline_method_args("Class", "test")[0]);
        CHECK_EQUAL(type_c.get_formatter_type(), fmt.get_inline_method_args("Class", "test")[1]);
    }

    TEST(InsertSources)
    {
        std::string content("return something;\n");
        shade::StorageFormatter fmt;

        fmt.insert_sources(content);

        CHECK_EQUAL(content, fmt.get_imported_sources());
    }
}
