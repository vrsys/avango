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
#include <shade/Shader.h>
#include <shade/utility.h>
#include <shade/types/objref.h>
#include <shade/types/int.h>
#include <shade/types/float.h>
#include <shade/types/vec2.h>
#include <shade/types/vec3.h>
#include <shade/types/vec4.h>
#include <shade/types/sampler2D.h>
#include <shade/types/list.h>
#include <shade/types/const.h>
#include "StorageFormatter.h"
#include "formatter/DumpGenerator.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <UnitTest++.h>

SUITE(Type)
{
  TEST(TypeName)
  {
    shade::vec3<> a;
    CHECK_EQUAL(shade::formatter::Constants::vec3, a.get_formatter_type());

    shade::sampler2D b;
    CHECK_EQUAL(shade::formatter::Constants::sampler2D, b.get_formatter_type());

    shade::float_<> c;
    CHECK_EQUAL(shade::formatter::Constants::float_, c.get_formatter_type());
  };

  TEST(TypeClone)
  {
    shade::vec3<> a;
    boost::shared_ptr<shade::Type> pa = boost::shared_ptr<shade::Type>(a.clone());
    CHECK_EQUAL(shade::formatter::Constants::vec3, pa->get_formatter_type());
  }

  TEST(GenerateObjrefConstructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::objref<> a;
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_objref>0</literal_objref>", generator.get_code());
  }

  TEST(GenerateObjrefConstructorWithState)
  {
    shade::formatter::DumpGenerator generator;
    shade::PipelineState ps;
    shade::objref<> a;
    boost::shared_ptr<shade::Type::State> state(a.create_state(ps));
    a.generate_constructor(generator, state);
    CHECK_EQUAL("<literal_objref>0</literal_objref>", generator.get_code());
  }

  TEST(GenerateIntConstructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::int_<> a(42);
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_int>42</literal_int>", generator.get_code());
  }

  TEST(GenerateFloatConstructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::float_<> a(1.1);
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_float>1.1</literal_float>", generator.get_code());
  }

  TEST(GenerateVec2Constructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::vec2<> a(1.1, 2.2);
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_vec2>1.1 2.2</literal_vec2>", generator.get_code());
  }

  TEST(GenerateVec3Constructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::vec3<> a(1.1, 2.2, 3.3);
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_vec3>1.1 2.2 3.3</literal_vec3>", generator.get_code());
  }

  TEST(GenerateVec4Constructor)
  {
    shade::formatter::DumpGenerator generator;
    shade::vec4<> a(1.1, 2.2, 3.3, 4.4);
    a.generate_constructor(generator);
    CHECK_EQUAL("<literal_vec4>1.1 2.2 3.3 4.4</literal_vec4>", generator.get_code());
  }

  namespace
  {
    struct TestCallback
    {
      TestCallback(const shade::Type* type) :
        m_type(type),
        was_modified(false)
      {
      }

      void modified(const shade::Type* t)
      {
        if (t == m_type)
          was_modified = true;
      }

      const shade::Type* m_type;
      bool was_modified;
    };

  }

  TEST(RegisterCallback)
  {
    shade::vec3<> a;
    TestCallback tc(&a);

    a.set(1., 2., 3.);
    CHECK(!tc.was_modified);
    a.register_notify(boost::bind(&TestCallback::modified, &tc, _1));
    CHECK(!tc.was_modified);
    a.set(1., 2., 3.);
    CHECK(tc.was_modified);
  }

  TEST(UnregisterCallback)
  {
    shade::vec3<> a;
    TestCallback tc(&a);

    a.set(1., 2., 3.);
    CHECK(!tc.was_modified);
    shade::Type::connection con = a.register_notify(boost::bind(&TestCallback::modified, &tc, _1));
    CHECK(!tc.was_modified);
    a.set(1., 2., 3.);
    CHECK(tc.was_modified);

    tc.was_modified = false;
    con.disconnect();
    a.set(1., 2., 3.);
    CHECK(!tc.was_modified);
  }

  TEST(DeleteCallback)
  {
    shade::vec3<>* a = new shade::vec3<>;
    TestCallback tc(a);

    shade::Type::connection con = a->register_notify(boost::bind(&TestCallback::modified, &tc, _1));
    CHECK(con.connected());
    delete a;
    CHECK(!con.connected());
  }

  TEST(RegisterMultipleCallback)
  {
    shade::vec3<> a;
    TestCallback tc1(&a);
    TestCallback tc2(&a);

    a.set(1., 2., 3.);
    CHECK(!tc1.was_modified);
    CHECK(!tc2.was_modified);
    a.register_notify(boost::bind(&TestCallback::modified, &tc1, _1));
    a.register_notify(boost::bind(&TestCallback::modified, &tc2, _1));
    CHECK(!tc1.was_modified);
    CHECK(!tc2.was_modified);
    a.set(1., 2., 3.);
    CHECK(tc1.was_modified);
    CHECK(tc2.was_modified);
  }

  TEST(QualifiedType) 
  { 
    shade::vec3<> a; 
    CHECK_EQUAL(shade::formatter::Constants::vec3, a.get_formatter_type()); 
    CHECK_EQUAL("vec3", a.get_uniq_id()); 

    shade::vec3<shade::uniform> b; 
    CHECK_EQUAL(shade::formatter::Constants::vec3, b.get_formatter_type()); 
    CHECK_EQUAL("uniform_vec3", b.get_uniq_id()); 
  }; 

  namespace
  {
    class TestShader : public shade::ShaderBase<TestShader>
    {
    private:
      SHADE_BASE_DECL(TestShader)
    };
    SHADE_CLASS_INIT(TestShader, "<undefined>", SHADE_NONE, SHADE_NONE)
  }

  TEST(ListAccess)
  {
    typedef shade::list<std::list< boost::shared_ptr<shade::Shader> > > shader_list;
    shader_list l;
    TestCallback tc(&l);

    CHECK(!tc.was_modified);
    l.register_notify(boost::bind(&TestCallback::modified, &tc, _1));
    CHECK(!tc.was_modified);
    CHECK_EQUAL(0u, l->size());
    CHECK(!tc.was_modified);

    {
      const shader_list::Accessor acc(l);
      CHECK(acc->empty());
    }

    CHECK(!tc.was_modified);

    {
      shader_list::Accessor acc(l);
      acc->push_back(boost::shared_ptr<shade::Shader>(new TestShader));
    }

    CHECK_EQUAL(1u, l->size());
    CHECK(tc.was_modified);
  }

  TEST(FunctionDefinitionState)
  {
    shade::vec3<> a;
    CHECK(!a.has_function());

    const std::type_info* ti = &typeid(a);
    a.invoke("Test", ti);
    CHECK(a.has_function());
  };

  TEST(WriteAttribute)
  {
    shade::StorageFormatter fmt(shade::vertex_shader);
    shade::PipelineState ps;

    shade::vec3<> a;
    a.output_attribute(a.create_state(ps), &fmt, "obj_0x0", "color");

    shade::sampler2D b;
    b.output_attribute(a.create_state(ps), &fmt, "obj_0x1", "tex");

    shade::float_<shade::raw_varying> c;
    c.output_attribute(a.create_state(ps), &fmt, "obj_0x2", "luminance");

    CHECK(fmt.has_attribute("obj_0x0", "color"));
    CHECK(fmt.has_attribute("obj_0x1", "tex"));
    CHECK(fmt.has_attribute("obj_0x2", "luminance"));

    CHECK_EQUAL(shade::formatter::Constants::vec3,
        fmt.get_attribute("obj_0x0", "color").type);
    CHECK_EQUAL(shade::formatter::Constants::none,
        fmt.get_attribute("obj_0x0", "color").qualifier);

    CHECK_EQUAL(shade::formatter::Constants::sampler2D,
        fmt.get_attribute("obj_0x1", "tex").type);
    CHECK_EQUAL(shade::formatter::Constants::uniform,
        fmt.get_attribute("obj_0x1", "tex").qualifier);

    CHECK_EQUAL(shade::formatter::Constants::float_,
        fmt.get_attribute("obj_0x2", "luminance").type);
    CHECK_EQUAL(shade::formatter::Constants::output,
        fmt.get_attribute("obj_0x2", "luminance").qualifier);
  }

  TEST(WriteAttributesWithInit)
  {
    shade::StorageFormatter fmt(shade::vertex_shader);
    shade::PipelineState ps;

    shade::vec3<shade::const_> position;
    position.set(1.1, 2.2, 3.3);
    position.output_attribute(position.create_state(ps), &fmt, "obj_0x3", "position");

    CHECK(fmt.has_attribute("obj_0x3", "position"));

    CHECK_EQUAL(shade::formatter::Constants::vec3,
        fmt.get_attribute("obj_0x3", "position").type);
    CHECK_EQUAL(shade::formatter::Constants::const_,
        fmt.get_attribute("obj_0x3", "position").qualifier);
    CHECK_EQUAL("<literal_vec3>1.1 2.2 3.3</literal_vec3>",
        fmt.get_attribute("obj_0x3", "position").initializer);
  }

  TEST(WriteAttributesAsArray)
  {
    shade::StorageFormatter fmt_geometry(shade::geometry_shader);
    shade::StorageFormatter fmt_fragment(shade::fragment_shader);
    shade::PipelineState ps;

    shade::vec3<shade::inout<shade::vertex_shader, shade::geometry_shader> > position_geometry;
    shade::vec3<shade::inout<shade::vertex_shader, shade::fragment_shader> > position_fragment;

    position_geometry.output_attribute(position_geometry.create_state(ps), &fmt_geometry, "obj_0x3", "position");
    position_fragment.output_attribute(position_fragment.create_state(ps), &fmt_fragment, "obj_0x3", "position");

    CHECK(fmt_geometry.has_attribute("obj_0x3", "position"));
    CHECK(fmt_geometry.get_attribute("obj_0x3", "position").is_array);

    CHECK(fmt_fragment.has_attribute("obj_0x3", "position"));
    CHECK(!fmt_fragment.get_attribute("obj_0x3", "position").is_array);
  }

  TEST(WriteAttributesInGeometryShader)
  {
    shade::StorageFormatter fmt(shade::geometry_shader);
    fmt.setup(10, shade::formatter::Constants::triangles);
    shade::PipelineState ps;

    shade::vec3<shade::uniform> position;

    position.output_attribute(position.create_state(ps), &fmt, "obj_0x3", "position");

    CHECK(fmt.has_attribute("obj_0x3", "position"));
    CHECK_EQUAL(1u, fmt.get_attribute("obj_0x3", "position").num_copies);
  }

  TEST(WriteLocalAttributesInGeometryShader)
  {
    shade::StorageFormatter fmt_geometry(shade::geometry_shader);
    fmt_geometry.setup(10, shade::formatter::Constants::triangles);
    shade::StorageFormatter fmt_fragment(shade::fragment_shader);
    shade::PipelineState ps;

    shade::vec3<shade::local> position_geometry;
    shade::vec3<shade::local> position_fragment;

    position_geometry.output_attribute(position_geometry.create_state(ps), &fmt_geometry, "obj_0x3", "position");
    position_fragment.output_attribute(position_fragment.create_state(ps), &fmt_fragment, "obj_0x3", "position");

    CHECK(fmt_geometry.has_attribute("obj_0x3", "position"));
    CHECK_EQUAL(3u, fmt_geometry.get_attribute("obj_0x3", "position").num_copies);

    CHECK(fmt_fragment.has_attribute("obj_0x3", "position"));
    CHECK_EQUAL(1u, fmt_fragment.get_attribute("obj_0x3", "position").num_copies);
  }

  TEST(WritePropertyDispatcher)
  {
    shade::StorageFormatter fmt(shade::vertex_shader);
    shade::PipelineState ps;

    shade::vec3<shade::inout<shade::vertex_shader, shade::geometry_shader> > position;
    shade::vec3<shade::uniform > uniform_position;

    position.output_begin_property_dispatcher(&fmt, "Class", "value");
    position.output_insert_property_dispatcher(position.create_state(ps), &fmt, 1, "obj_0x0");
    fmt.end_property_dispatcher();

    uniform_position.output_begin_property_dispatcher(&fmt, "Class", "uniform_value");
    uniform_position.output_insert_property_dispatcher(uniform_position.create_state(ps), &fmt, 1, "obj_0x0");
    fmt.end_property_dispatcher();

    CHECK_EQUAL(shade::StorageFormatter::normal, fmt.get_property_dispatch_type("Class", "value"));
    CHECK_EQUAL("obj_0x0", fmt.get_property_dispatch("Class", "value", 1));
    CHECK_EQUAL(shade::StorageFormatter::none, fmt.get_property_dispatch_type("Class", "uniform_value"));
  }

  TEST(WriteInputPropertyDispatcherInGeometryShader)
  {
    shade::StorageFormatter fmt(shade::geometry_shader);
    shade::PipelineState ps;

    shade::vec3<shade::inout<shade::vertex_shader, shade::geometry_shader> > position;
    shade::vec3<shade::uniform > uniform_position;

    position.output_begin_property_dispatcher(&fmt, "Class", "value");
    position.output_insert_property_dispatcher(position.create_state(ps), &fmt, 1, "obj_0x0");
    fmt.end_property_dispatcher();

    uniform_position.output_begin_property_dispatcher(&fmt, "Class", "uniform_value");
    uniform_position.output_insert_property_dispatcher(uniform_position.create_state(ps), &fmt, 1, "obj_0x0");
    fmt.end_property_dispatcher();

    CHECK_EQUAL(shade::StorageFormatter::array, fmt.get_property_dispatch_type("Class", "value"));
    CHECK_EQUAL(shade::StorageFormatter::none, fmt.get_property_dispatch_type("Class", "uniform_value"));
  }

  TEST(WriteLocalPropertyDispatcherInGeometryShader)
  {
    shade::StorageFormatter fmt_geometry(shade::geometry_shader);
    shade::StorageFormatter fmt_fragment(shade::fragment_shader);
    shade::PipelineState ps;

    shade::vec3<shade::local> position;

    position.output_begin_property_dispatcher(&fmt_geometry, "Class", "value");
    position.output_insert_property_dispatcher(position.create_state(ps), &fmt_geometry, 1, "obj_0x0");
    fmt_geometry.end_property_dispatcher();

    position.output_begin_property_dispatcher(&fmt_fragment, "Class", "value");
    position.output_insert_property_dispatcher(position.create_state(ps), &fmt_fragment, 1, "obj_0x0");
    fmt_geometry.end_property_dispatcher();

    CHECK_EQUAL(shade::StorageFormatter::multiple, fmt_geometry.get_property_dispatch_type("Class", "value"));
    CHECK_EQUAL(shade::StorageFormatter::normal, fmt_fragment.get_property_dispatch_type("Class", "value"));
  }

}
