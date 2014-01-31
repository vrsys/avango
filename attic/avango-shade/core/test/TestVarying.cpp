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
#include <shade/ShaderEnvironment.h>
#include <shade/shaders/Surface.h>
#include <shade/shaders/Geometry.h>
#include <shade/types/vec3.h>
#include <shade/types/Varying.h>
#include <shade/PipelineState.h>
#include <shade/UVWInstance.h>
#include <shade/utility.h>
#include "StorageFormatter.h"
#include "../src/Component.h"
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>


SUITE(Varying)
{
  class ClassWithVarying : public shade::ShaderBase<ClassWithVarying>
  {
  public:
    shade::Varying<shade::vec3> color;

    ClassWithVarying(void)
    {
      shade::UVWInstance::init();
    }

  private:
    SHADE_BASE_DECL(ClassWithVarying)
  };

  SHADE_CLASS_INIT(ClassWithVarying, "", SHADE_NONE, SHADE_DEFS((color)))

    struct CompilationFixture
    {
      CompilationFixture(void) :
        value(new ClassWithVarying),
        value_id(value->get_uniq_id()),
        component(value),
        acc(resolver),
        fmt_vertex(shade::vertex_shader),
        fmt_geometry(shade::geometry_shader),
        fmt_fragment(shade::fragment_shader)
      {
        char* shaderdir = std::getenv("SHADE_SHADER_PATH");
        if (shaderdir != 0)
          append_env_path(resolver, shaderdir);
      }

      void compile(bool with_geometry_shader = false)
      {
        shade::PipelineState ps;
        if (with_geometry_shader)
        {
          boost::shared_ptr<shade::shaders::Surface> surface(new shade::shaders::Surface);
          boost::shared_ptr<shade::shaders::Geometry> geometry(new shade::shaders::Geometry);
          surface->geometry = geometry;
          ps.set_root_shader(surface);
        }
        component.setup(acc, ps);

        component.compile(fmt_vertex, acc, shade::vertex_shader);
        component.compile(fmt_geometry, acc, shade::geometry_shader);
        component.compile(fmt_fragment, acc, shade::fragment_shader);
      }

      boost::shared_ptr<shade::Shader> value;
      std::string value_id;
      shade::Component component;
      shade::FileResolver resolver;
      shade::FileAccumulator acc;
      shade::StorageFormatter fmt_vertex;
      shade::StorageFormatter fmt_geometry;
      shade::StorageFormatter fmt_fragment;
    };

  TEST_FIXTURE(CompilationFixture, NoGeometryShader)
  {
    compile();
    std::string class_("ClassWithVarying");

    CHECK(!fmt_vertex.has_attribute(value_id, "color"));
    CHECK_EQUAL(shade::StorageFormatter::deferred, fmt_vertex.get_property_dispatch_type(class_, "color"));
    CHECK_EQUAL(1u, fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).object);
    CHECK_EQUAL("get", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).getter_method);
    CHECK_EQUAL("Gettable_vec3", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).getter_class);
    CHECK_EQUAL("set", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).setter_method);
    CHECK_EQUAL("Settable_vec3", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).setter_class);
    CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt_vertex.get_property_dispatch_return(class_, "color"));

    CHECK(!fmt_fragment.has_attribute(value_id, "color"));
    CHECK_EQUAL(shade::StorageFormatter::deferred, fmt_fragment.get_property_dispatch_type(class_, "color"));
    CHECK_EQUAL(1u, fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).object);
    CHECK_EQUAL("get", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).getter_method);
    CHECK_EQUAL("Gettable_vec3", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).getter_class);
    CHECK_EQUAL("set", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).setter_method);
    CHECK_EQUAL("Settable_vec3", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).setter_class);
    CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt_fragment.get_property_dispatch_return(class_, "color"));
  }

  TEST_FIXTURE(CompilationFixture, WithGeometryShader)
  {
    compile(true);
    std::string class_("ClassWithVarying");

    CHECK(!fmt_vertex.has_attribute(value_id, "color"));
    CHECK_EQUAL(shade::StorageFormatter::deferred, fmt_vertex.get_property_dispatch_type(class_, "color"));
    CHECK_EQUAL(1u, fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).object);
    CHECK_EQUAL("get", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).getter_method);
    CHECK_EQUAL("Gettable_vec3", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).getter_class);
    CHECK_EQUAL("set", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).setter_method);
    CHECK_EQUAL("Settable_vec3", fmt_vertex.get_deferred_property_dispatch(class_, "color", 0).setter_class);
    CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt_vertex.get_property_dispatch_return(class_, "color"));

    CHECK(fmt_geometry.has_function("Initializeable", "init_geometry"));
    CHECK(fmt_geometry.has_function("Initializeable", "init_post_geometry"));

    CHECK(!fmt_fragment.has_attribute(value_id, "color"));
    CHECK_EQUAL(shade::StorageFormatter::deferred, fmt_fragment.get_property_dispatch_type(class_, "color"));
    CHECK_EQUAL(2u, fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).object);
    CHECK_EQUAL("get", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).getter_method);
    CHECK_EQUAL("Gettable_vec3", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).getter_class);
    CHECK_EQUAL("set", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).setter_method);
    CHECK_EQUAL("Settable_vec3", fmt_fragment.get_deferred_property_dispatch(class_, "color", 0).setter_class);
    CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt_fragment.get_property_dispatch_return(class_, "color"));
  }
}
