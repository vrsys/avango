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

#include "../src/Component.h"

#include <shade/Type.h>
#include <shade/Shader.h>
#include <shade/PipelineState.h>
#include <shade/shaders/Initializeable.h>
#include <shade/shaders/Enterable.h>
#include <shade/types/vec3.h>
#include <shade/types/vec2.h>
#include <shade/types/objref.h>
#include <shade/types/const.h>
#include <shade/types/local.h>
#include "StorageFormatter.h"
#include "GLSLMockupWrapper.h"
#include <string>
#include <sstream>
#include <set>
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>

namespace
{
class uniform_mockup : public shade::Type
{
  public:
    uniform_mockup(shade::formatter::Constants::Type type) : shade::Type(type){};

    /*virtual*/ const std::string& get_qualifier_str(void) const
    {
        static std::string qualifier("uniform");
        return qualifier;
    }

    /*virtual*/ bool is_writable(const shade::ShaderEnvironment& se) const { return false; }

    /*virtual*/ void upload(boost::shared_ptr<shade::GLSLWrapper> wrapper, LinkIndex index) const { upload_uniform(wrapper, index); }

    /*virtual*/ void output_attribute(boost::shared_ptr<Type::State>, shade::Formatter* fmt, const std::string& obj, const std::string& name) const
    {
        fmt->insert_attribute(m_type, obj, name, shade::formatter::Constants::uniform);
    }
};

template <class Qualifier = shade::local>
class vec3_mockup : public shade::TypeBase<vec3_mockup<Qualifier>, Qualifier>
{
  public:
    vec3_mockup(void) : shade::TypeBase<vec3_mockup<Qualifier>, Qualifier>(shade::formatter::Constants::vec3), m_dirty(true), m_upload(false), m_upload_status(false)
    {
        union {
            shade::Type::LinkIndex index;
            vec3_mockup* pointer;
        } conversion;
        conversion.pointer = this;
        m_link_index = conversion.index;
    }

    void touch(bool dirty = true)
    {
        m_dirty = dirty;
        if(dirty)
            shade::TypeBase<vec3_mockup<Qualifier>, Qualifier>::touch();
    }
    bool is_dirty(void) const { return m_dirty; }
    bool was_uploaded(void) const { return m_upload; }
    void reset_upload_status(void) const { m_upload = false; }

    /*virtual*/ shade::Type::LinkIndex get_link_index(boost::shared_ptr<shade::GLSLWrapper>, const std::string& name) const
    {
        m_link_name = name;
        return m_link_index;
    }

    /*virtual*/ void upload_uniform(boost::shared_ptr<shade::GLSLWrapper>, shade::Type::LinkIndex index) const
    {
        m_dirty = false;
        m_upload = true;
        m_upload_status = (m_link_index == index);
    }

    /*virtual*/ void generate_constructor(shade::formatter::Generator& generator) const { generator.handle_verbatim("mockup"); }

    bool link_valid(void) const { return m_upload_status; }
    std::string get_link_name(void) const { return m_link_name; }

  private:
    mutable bool m_dirty;
    mutable bool m_upload;
    mutable bool m_upload_status;
    mutable std::string m_link_name;
    shade::Type::LinkIndex m_link_index;
};

class LightSource : public shade::ShaderBase<LightSource, shade::shaders::Initializeable>
{
  public:
    /*virtual*/ shade::void_<> init_fragment(void) { return invoke<shade::void_<>>("LightSource_initialize_impl"); }

    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return shade::vec3<>(); };
    virtual shade::vec3<> get_intensity(shade::vec2<>) { return invoke_inline<shade::vec3<>>(std::string("return value;\n")); };

    virtual shade::vec3<> illuminate_vertex(shade::vec3<>, shade::vec3<>) { return shade::vec3<>(); }

    vec3_mockup<uniform_mockup> intensity;
    vec3_mockup<> light_sum;

  private:
    friend class shade::ShaderBase<LightSource, shade::shaders::Initializeable>;
    static std::string class_name;
    static const std::string source_name;
    static shade::MethodAccessor methods[];
    static shade::AttributeAccessor attributes[];
};

std::string LightSource::class_name("LightSource");
const std::string LightSource::source_name("LightSource.glsl");
shade::MethodAccessor LightSource::methods[] = {LightSource::def(&LightSource::get_intensity, "get_intensity"),
                                                LightSource::def(&LightSource::illuminate, "illuminate"),
                                                LightSource::def(&LightSource::illuminate_vertex, "illuminate_vertex", shade::vertex_shader),
                                                shade::MethodAccessor()};
shade::AttributeAccessor LightSource::attributes[] = {LightSource::def(&LightSource::intensity, "intensity"), LightSource::def(&LightSource::light_sum, "light_sum"), shade::AttributeAccessor()};

class PointLightSource : public shade::ShaderBase<PointLightSource, LightSource>
{
  public:
    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return invoke<shade::vec3<>>("PointLightSource_illuminate_impl"); };

    virtual shade::vec3<> illuminate_vertex(shade::vec3<>, shade::vec3<>) { return shade::vec3<>(); }

    vec3_mockup<uniform_mockup> position;
    vec3_mockup<shade::const_> quality;

  private:
    friend class shade::ShaderBase<PointLightSource, LightSource>;
    static std::string class_name;
    static const std::string source_name;
    static shade::AttributeAccessor attributes[];
};

std::string PointLightSource::class_name("PointLightSource");
const std::string PointLightSource::source_name("PointLightSource.glsl");
shade::AttributeAccessor PointLightSource::attributes[] = {
    PointLightSource::def(&PointLightSource::position, "position"), PointLightSource::def(&PointLightSource::quality, "quality"), shade::AttributeAccessor()};

class Material : public shade::ShaderBase<Material, shade::shaders::Enterable>
{
  public:
    /*virtual*/ shade::void_<> enter_fragment(void) { return invoke<shade::void_<>>("Material_fragment_impl"); };

    virtual shade::void_<> shade(void) { return invoke<shade::void_<>>("Material_shade_impl"); };

    virtual shade::void_<> transform(void) { return invoke<shade::void_<>>("Material_transform_impl"); };

    virtual shade::void_<> noop(void) { return shade::void_<>(); }

    vec3_mockup<uniform_mockup> color;
    shade::objref<LightSource*, shade::const_> light;
    vec3_mockup<> color_sum;

  private:
    friend class shade::ShaderBase<Material, shade::shaders::Enterable>;
    static std::string class_name;
    static const std::string source_name;
    static shade::MethodAccessor methods[];
    static shade::AttributeAccessor attributes[];
};

std::string Material::class_name("Material");
const std::string Material::source_name("Material.glsl");
shade::MethodAccessor Material::methods[] = {Material::def(&Material::shade, "shade", shade::fragment_shader),
                                             Material::def(&Material::transform, "transform", shade::vertex_shader),
                                             Material::def(&Material::noop, "noop", shade::fragment_shader),
                                             shade::MethodAccessor()};
shade::AttributeAccessor Material::attributes[] = {
    Material::def(&Material::color, "color"), Material::def(&Material::light, "light"), Material::def(&Material::color_sum, "color_sum"), shade::AttributeAccessor()};
} // namespace

SUITE(Component)
{
    struct CompileFixture
    {
        struct EnvNotSet
        {
        };

        CompileFixture(void) : wrapper(new GLSLMockupWrapper), material(new Material), component(material), acc(resolver)
        {
            char* testdir = std::getenv("SHADE_TEST_DATA");
            if(testdir == 0)
                throw EnvNotSet();
            resolver.append_path(testdir);

            point_light_source_id = point_light_source.get_uniq_id();
            point_light_source_name = point_light_source.get_class_name();
            material_id = material->get_uniq_id();
            material_name = material->get_class_name();

            material->light = &point_light_source;
            shade::PipelineState ps;
            component.setup(acc, ps);
            component.compile(fmt, acc, shade::fragment_shader);
            component.link(wrapper);
        }

        boost::shared_ptr<shade::GLSLWrapper> wrapper;
        boost::shared_ptr<Material> material;
        PointLightSource point_light_source;
        shade::Component component;

        shade::StorageFormatter fmt;
        shade::FileResolver resolver;
        shade::FileAccumulator acc;

        std::string point_light_source_id;
        std::string point_light_source_name;
        std::string material_id;
        std::string material_name;
    };

    TEST_FIXTURE(CompileFixture, FunctionCompile)
    {
        CHECK(fmt.has_function("LightSource", "illuminate"));
        CHECK_EQUAL(3u, fmt.get_function_args("LightSource", "illuminate").size());
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_function_args("LightSource", "illuminate")[0]);
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_function_args("LightSource", "illuminate")[1]);
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_function_args("LightSource", "illuminate")[2]);
        CHECK_EQUAL(point_light_source_id, fmt.get_function_dispatch("LightSource", "illuminate", 1).first);
        CHECK_EQUAL(2u, fmt.get_function_dispatch_args("LightSource", "illuminate", 1).size());
        CHECK_EQUAL("position", fmt.get_function_dispatch_args("LightSource", "illuminate", 1)[0]);
        CHECK_EQUAL("quality", fmt.get_function_dispatch_args("LightSource", "illuminate", 1)[1]);
        CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt.get_function_return("LightSource", "illuminate"));

        CHECK(fmt.has_function("LightSource", "get_intensity"));
        CHECK_EQUAL(1u, fmt.get_function_dispatch_args("LightSource", "get_intensity", 1).size());
        CHECK_EQUAL("intensity", fmt.get_function_dispatch_args("LightSource", "get_intensity", 1)[0]);
        CHECK_EQUAL("<literal_vec3>0 0 0</literal_vec3>", fmt.get_function_return("LightSource", "get_intensity"));

        CHECK(fmt.has_function("Material", "shade"));
        CHECK_EQUAL("Material_shade_impl", fmt.get_function_dispatch("Material", "shade", 0).second);
        CHECK_EQUAL(2u, fmt.get_function_dispatch_args("Material", "shade", 0).size());
        CHECK_EQUAL("color", fmt.get_function_dispatch_args("Material", "shade", 0)[0]);
        CHECK_EQUAL("light", fmt.get_function_dispatch_args("Material", "shade", 0)[1]);
        CHECK_EQUAL("", fmt.get_function_return("Material", "shade"));

        CHECK(fmt.has_function("Material", "noop"));
        CHECK(!fmt.has_function_dispatch("Material", "noop", 0));
        CHECK_EQUAL("", fmt.get_function_return("Material", "noop"));
    }

    TEST_FIXTURE(CompileFixture, FunctionEnvCompile)
    {
        CHECK(fmt.has_function("Material", "shade"));
        CHECK(!fmt.has_function("Material", "transform"));
        CHECK(fmt.has_function("LightSource", "illuminate"));
        CHECK(!fmt.has_function("LightSource", "illuminate_vertex"));
    }

    TEST_FIXTURE(CompileFixture, PropertyCompile)
    {
        CHECK("" == fmt.get_property_dispatch("LightSource", "intensity", 1));
        CHECK("" != fmt.get_property_dispatch("LightSource", "light_sum", 1));
        CHECK_EQUAL("<verbatim>mockup</verbatim>", fmt.get_property_dispatch_return("LightSource", "light_sum"));
        CHECK("" == fmt.get_property_dispatch("PointLightSource", "position", 1));
        CHECK("" == fmt.get_property_dispatch("PointLightSource", "quality", 1));
        CHECK("" == fmt.get_property_dispatch("Material", "color", 0));
        CHECK("" == fmt.get_property_dispatch("Material", "light", 0));
        CHECK("" != fmt.get_property_dispatch("Material", "color_sum", 0));
        CHECK_EQUAL("<verbatim>mockup</verbatim>", fmt.get_property_dispatch_return("Material", "color_sum"));
    }

    TEST_FIXTURE(CompileFixture, InstanceCompile)
    {
        CHECK(fmt.has_attribute(point_light_source_id, "intensity"));
        CHECK(fmt.has_attribute(point_light_source_id, "position"));
        CHECK(fmt.has_attribute(material_id, "color"));
        CHECK(fmt.has_attribute(material_id, "light"));
        CHECK_EQUAL(shade::formatter::Constants::uniform, fmt.get_attribute(material_id, "color").qualifier);
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_attribute(material_id, "color").type);
    }

    TEST_FIXTURE(CompileFixture, SelfArgumentsCompile)
    {
        CHECK_EQUAL(1u, fmt.get_self_argument("LightSource").size());
        CHECK_EQUAL("intensity", fmt.get_self_argument("LightSource")[0].second);
        CHECK_EQUAL(2u, fmt.get_self_argument(point_light_source_name).size());
        CHECK_EQUAL("position", fmt.get_self_argument(point_light_source_name)[0].second);
        CHECK_EQUAL("quality", fmt.get_self_argument(point_light_source_name)[1].second);
        CHECK_EQUAL(2u, fmt.get_self_argument(material_name).size());
        CHECK_EQUAL("color", fmt.get_self_argument(material_name)[0].second);
        CHECK_EQUAL("light", fmt.get_self_argument(material_name)[1].second);
    }

    TEST_FIXTURE(CompileFixture, DoUpload)
    {
        CHECK(component.requires_upload());
        material->color.touch();
        component.upload(wrapper);
        CHECK(!component.requires_upload());
        CHECK(!material->color.is_dirty());
    }

    TEST_FIXTURE(CompileFixture, OnlyMinimalUpload)
    {
        component.upload(wrapper);
        point_light_source.position.reset_upload_status();
        material->color.reset_upload_status();

        point_light_source.position.touch();
        CHECK(component.requires_upload());
        component.upload(wrapper);
        CHECK(!component.requires_upload());

        CHECK(!point_light_source.position.is_dirty());
        CHECK(!material->color.is_dirty());
        CHECK(point_light_source.position.was_uploaded());
        CHECK(!material->color.was_uploaded());
    }

    TEST_FIXTURE(CompileFixture, ConstantUpload)
    {
        component.upload(wrapper);
        point_light_source.position.touch();
        CHECK(!component.requires_compilation());

        component.upload(wrapper);
        point_light_source.quality.touch();
        CHECK(component.requires_compilation());
        CHECK(!component.requires_upload());

        component.compile(fmt, acc, shade::fragment_shader);
        CHECK(!component.requires_compilation());
    }

    TEST_FIXTURE(CompileFixture, LinkNames)
    {
        std::string link_name = point_light_source.get_uniq_id() + "_position";
        CHECK_EQUAL(link_name, point_light_source.position.get_link_name());
    }

    TEST_FIXTURE(CompileFixture, LinkValid)
    {
        component.upload(wrapper);
        CHECK(point_light_source.position.link_valid());
    }

    TEST_FIXTURE(CompileFixture, SourceImport)
    {
        std::istringstream hin(acc.get_str());
        std::set<std::string> lines;
        while(hin.good())
        {
            std::string line;
            std::getline(hin, line);
            if(line.size())
                lines.insert(line);
        }

        CHECK_EQUAL(4u, lines.size());
        CHECK(lines.end() != lines.find("// Material implementation"));
        CHECK(lines.end() != lines.find("// SHADE glsl_version:110"));
        CHECK(lines.end() != lines.find("// LightSource implementation"));
        CHECK(lines.end() != lines.find("// PointLightSource implementation"));
    }

    TEST_FIXTURE(CompileFixture, GenerateSource)
    {
        std::string lines(fmt.get_imported_sources());
        CHECK(lines.npos != lines.find("// Material implementation"));
        CHECK(lines.npos != lines.find("// SHADE glsl_version:110"));
        CHECK(lines.npos != lines.find("// LightSource implementation"));
        CHECK(lines.npos != lines.find("// PointLightSource implementation"));
    }

    TEST_FIXTURE(CompileFixture, MainEntry)
    {
        std::string entry_name("enter_fragment");

        CHECK_EQUAL("Enterable_" + entry_name, fmt.get_invoke());

        CHECK_EQUAL(1u, fmt.get_function_args("Enterable", entry_name).size());
        CHECK_EQUAL(shade::formatter::Constants::void_, fmt.get_function_args("Enterable", entry_name)[0]);

        CHECK_EQUAL(2u, fmt.get_function_dispatch_args("Enterable", entry_name, 0).size());
        CHECK_EQUAL("color", fmt.get_function_dispatch_args("Enterable", entry_name, 0)[0]);
        CHECK_EQUAL("light", fmt.get_function_dispatch_args("Enterable", entry_name, 0)[1]);

        CHECK_EQUAL(material_id, fmt.get_function_dispatch("Enterable", entry_name, 0).first);
        CHECK_EQUAL("Material_fragment_impl", fmt.get_function_dispatch("Enterable", entry_name, 0).second);
    }

    TEST(RequiresCompilationAtStartup)
    {
        boost::shared_ptr<Material> material(new Material);
        shade::Component component(material);
        CHECK(component.requires_compilation());
    }

    TEST_FIXTURE(CompileFixture, OBJREFInitialization)
    {
        CHECK(fmt.has_attribute(material_id, "light"));
        CHECK_EQUAL("<literal_objref>1</literal_objref>", fmt.get_attribute(material_id, "light").initializer);
    }

    TEST_FIXTURE(CompileFixture, Initializers)
    {
        std::string initializer_name("init_fragment");

        CHECK_EQUAL(1u, fmt.get_initializers().size());
        CHECK_EQUAL(1u, fmt.get_function_args("Initializeable", initializer_name).size());
        CHECK_EQUAL(shade::formatter::Constants::void_, fmt.get_function_args("Initializeable", initializer_name)[0]);

        CHECK_EQUAL(fmt.get_method_name("shade_initialize", fmt.get_method_name("Initializeable", initializer_name)), fmt.get_initializers()[0].first);
        CHECK_EQUAL(1u, fmt.get_initializers()[0].second);

        CHECK_EQUAL(1u, fmt.get_function_dispatch_args("Initializeable", initializer_name, 1).size());
        CHECK_EQUAL("intensity", fmt.get_function_dispatch_args("Initializeable", initializer_name, 1)[0]);

        CHECK_EQUAL(point_light_source_id, fmt.get_function_dispatch("Initializeable", initializer_name, 1).first);
        CHECK_EQUAL("LightSource_initialize_impl", fmt.get_function_dispatch("Initializeable", initializer_name, 1).second);
    }

    TEST_FIXTURE(CompileFixture, InlineMethod)
    {
        CHECK(fmt.has_function("LightSource", "get_intensity"));
        CHECK_EQUAL(2u, fmt.get_function_args("LightSource", "get_intensity").size());
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_function_args("LightSource", "get_intensity")[0]);
        CHECK_EQUAL(shade::formatter::Constants::vec2, fmt.get_function_args("LightSource", "get_intensity")[1]);
        CHECK_EQUAL(point_light_source_id, fmt.get_function_dispatch("LightSource", "get_intensity", 1).first);
        CHECK_EQUAL("get_intensity", fmt.get_function_dispatch("LightSource", "get_intensity", 1).second);

        CHECK(fmt.has_inline_method("LightSource", "get_intensity"));
        CHECK_EQUAL(shade::formatter::Constants::vec3, fmt.get_inline_method("LightSource", "get_intensity").first);
        CHECK_EQUAL("<verbatim>return value;\n</verbatim>", fmt.get_inline_method("LightSource", "get_intensity").second);
        CHECK_EQUAL(1u, fmt.get_inline_method_args("LightSource", "get_intensity").size());
        CHECK_EQUAL(shade::formatter::Constants::vec2, fmt.get_inline_method_args("LightSource", "get_intensity")[0]);
    }
}
