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
#include <shade/ObjectMap.h>
#include <shade/types/vec3.h>
#include <shade/types/list.h>
#include <shade/types/void.h>
#include <shade/types/const.h>
#include "StorageFormatter.h"
#include "formatter/DumpGenerator.h"
#include "GLSLMockupWrapper.h"
#include <shade/utility.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>

namespace
{
class LightSource : public shade::ShaderBase<LightSource>
{
  public:
    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return shade::vec3<>(); };

  private:
    SHADE_BASE_DECL(LightSource)
};
SHADE_CLASS_INIT(LightSource, "empty.glsl", SHADE_DEFS((illuminate)), SHADE_NONE)

class PointLightSource : public shade::ShaderBase<PointLightSource, LightSource>
{
  public:
    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return invoke<shade::vec3<>>("PointLightSource_illuminate_impl"); };

    shade::vec3<> position;

  private:
    SHADE_DERIVED_DECL(PointLightSource, LightSource)
};
SHADE_CLASS_INIT(PointLightSource, "empty.glsl", SHADE_NONE, SHADE_DEFS((position)))

class DirectionalLightSource : public shade::ShaderBase<DirectionalLightSource, LightSource>
{
  public:
    virtual shade::vec3<> illuminate(shade::vec3<>, shade::vec3<>) { return invoke<shade::vec3<>>("DirectionalLightSource_illuminate_impl"); };

    shade::vec3<> direction;

  private:
    SHADE_DERIVED_DECL(DirectionalLightSource, LightSource)
};
SHADE_CLASS_INIT(DirectionalLightSource, "empty.glsl", SHADE_NONE, SHADE_DEFS((direction)))

class Material : public shade::ShaderBase<Material>
{
  public:
    virtual shade::void_<> shade(void) { return invoke<shade::void_<>>("Material_shade_impl"); };

    shade::vec3<> color;

    typedef shade::list<std::list<boost::shared_ptr<LightSource>>, shade::const_> LightList;
    LightList light;

  private:
    SHADE_BASE_DECL(Material)
};
SHADE_CLASS_INIT(Material, "empty.glsl", SHADE_DEFS((shade)), SHADE_DEFS((color)(light)))

} // namespace

SUITE(ListType)
{
    struct CompileFixture
    {
        struct EnvNotSet
        {
        };

        CompileFixture(void) : material(new Material), point_light_source(new PointLightSource), directional_light_source(new DirectionalLightSource), component(material), acc(resolver)
        {
            char* testdir = std::getenv("SHADE_TEST_DATA");
            if(testdir == 0)
                throw EnvNotSet();
            resolver.append_path(testdir);

            point_light_source_id = point_light_source->get_uniq_id();
            directional_light_source_id = directional_light_source->get_uniq_id();

            {
                Material::LightList::Accessor accessor(material->light);
                accessor->push_back(point_light_source);
                accessor->push_back(directional_light_source);
            };

            shade::PipelineState ps;
            component.setup(acc, ps);
            component.compile(fmt, acc, shade::fragment_shader);
            component.link(boost::shared_ptr<shade::GLSLWrapper>(new GLSLMockupWrapper));
        }

        boost::shared_ptr<Material> material;
        boost::shared_ptr<PointLightSource> point_light_source;
        boost::shared_ptr<DirectionalLightSource> directional_light_source;
        shade::Component component;

        shade::StorageFormatter fmt;
        shade::FileResolver resolver;
        shade::FileAccumulator acc;

        std::string point_light_source_id;
        std::string directional_light_source_id;
    };

    TEST_FIXTURE(CompileFixture, HasInstances)
    {
        CHECK(fmt.has_attribute(point_light_source_id, "position"));
        CHECK(fmt.has_attribute(directional_light_source_id, "direction"));
    }

    TEST(GenerateListConstructor)
    {
        shade::formatter::DumpGenerator generator;
        shade::list<std::list<boost::shared_ptr<LightSource>>, shade::const_> a;
        a.generate_constructor(generator);
        CHECK_EQUAL("<literal_list>0 0</literal_list>", generator.get_code());
    }

    TEST_FIXTURE(CompileFixture, GenerateListConstructorWithState)
    {
        shade::formatter::DumpGenerator generator;
        shade::PipelineState ps;
        shade::ObjectMap map;
        boost::shared_ptr<shade::Type::State> state(material->light.create_state(ps));
        material->light.insert_references(state, map);
        material->light.generate_constructor(generator, state);
        CHECK_EQUAL("<literal_list>0 2</literal_list>", generator.get_code());
    }
}
