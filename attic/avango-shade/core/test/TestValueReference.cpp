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

#include <shade/Type.h>
#include <shade/shaders/Value.h>
#include <shade/FileResolver.h>
#include <shade/FileAccumulator.h>
#include <shade/PipelineState.h>
#include <shade/ShaderEnvironment.h>
#include <shade/types/vec3.h>
#include <shade/types/ValueReference.h>
#include "StorageFormatter.h"
#include "formatter/DumpGenerator.h"
#include <boost/shared_ptr.hpp>
#include <shade/utility.h>
#include "../src/Component.h"
#include <UnitTest++.h>

SUITE(ValueReference)
{
    class ClassWithValueReference : public shade::ShaderBase<ClassWithValueReference>
    {
      public:
        shade::ValueReference<shade::vec3, shade::local> color;

      private:
        SHADE_BASE_DECL(ClassWithValueReference)
    };

    SHADE_CLASS_INIT(ClassWithValueReference, "", SHADE_NONE, SHADE_DEFS((color)))

    struct CompilationFixture
    {
        CompilationFixture(void) : value(new ClassWithValueReference), value_id(value->get_uniq_id()), component(value), acc(resolver)
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

    TEST_FIXTURE(CompilationFixture, Attribute) { CHECK(!fmt.has_attribute(value_id, "color")); }

    TEST_FIXTURE(CompilationFixture, DeferredProperty)
    {
        std::string class_("ClassWithValueReference");

        CHECK_EQUAL(shade::StorageFormatter::deferred, fmt.get_property_dispatch_type(class_, "color"));
        CHECK_EQUAL(1u, fmt.get_deferred_property_dispatch(class_, "color", 0).object);
        CHECK_EQUAL("get", fmt.get_deferred_property_dispatch(class_, "color", 0).getter_method);
        CHECK_EQUAL("Gettable_vec3", fmt.get_deferred_property_dispatch(class_, "color", 0).getter_class);
        CHECK_EQUAL("set", fmt.get_deferred_property_dispatch(class_, "color", 0).setter_method);
        CHECK_EQUAL("Settable_vec3", fmt.get_deferred_property_dispatch(class_, "color", 0).setter_class);
    }

    TEST(DefaultDefaultConstructor)
    {
        shade::ValueReference<shade::vec3, shade::local> value;
        shade::vec3<> comparison_value;
        CHECK_EQUAL(comparison_value.get_constructor_str(), value.get_constructor_str());
    }

    TEST(GenerateConstructor)
    {
        shade::formatter::DumpGenerator generator;
        shade::ValueReference<shade::vec3, shade::local> value;
        value.generate_constructor(generator);
        CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", generator.get_code());
    }

    TEST(GetSetValue)
    {
        ClassWithValueReference value_class;

        value_class.color.set_value(shade::vec3<>(1., 2., 3.));
        shade::vec3<shade::Type> value(value_class.color.get_value());
        float x, y, z;
        value.get(x, y, z);
        CHECK_EQUAL(1., x);
        CHECK_EQUAL(2., y);
        CHECK_EQUAL(3., z);
    }
}
