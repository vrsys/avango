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

#include "GLSLMockupWrapper.h"
#include <shade/ShaderEnvironment.h>
#include <UnitTest++.h>

SUITE(TestGLSLWrapper)
{
    TEST(CleanStartState)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();

        CHECK(wrapper.has_program());
        for(int i = 0; i != shade::num_shader_environment; ++i)
        {
            shade::ShaderEnvironment env = static_cast<shade::ShaderEnvironment>(i);
            CHECK(!wrapper.has_shader(env));
        }
    }

    TEST(SetShader)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();
        std::string code("code");

        wrapper.set_shader(shade::fragment_shader, code);
        CHECK(wrapper.has_shader(shade::fragment_shader));
        CHECK(wrapper.was_attached());
        CHECK_EQUAL(code, wrapper.get_code());
        CHECK(wrapper.was_compiled());
    }

    TEST(Link)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();

        wrapper.link();
        CHECK(wrapper.was_linked());
    }

    TEST(MakeCurrent)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();

        wrapper.make_current();
        CHECK(wrapper.was_made_current());
    }

    TEST(SettingUniform)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();

        shade::GLSLWrapper::Handle link = wrapper.get_link_name("one");
        wrapper.Uniform1i(link, 42);

        CHECK_EQUAL(42, wrapper.get_uniform_int(link));
    }

    TEST(TextureAllocation)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();
        wrapper.link();

        boost::shared_ptr<MockupTexture> texture0(new MockupTexture(0));
        shade::GLSLWrapper::Handle link0 = wrapper.get_link_name("zero");
        boost::shared_ptr<MockupTexture> texture1(new MockupTexture(1));
        shade::GLSLWrapper::Handle link1 = wrapper.get_link_name("one");

        wrapper.make_current();
        wrapper.UniformTexture(link0, texture0);
        wrapper.UniformTexture(link1, texture1);

        CHECK_EQUAL(1u, wrapper.get_max_texture_unit());
        if(wrapper.get_max_texture_unit() == 1)
        {
            CHECK_EQUAL(texture0, wrapper.get_texture(0));
            CHECK_EQUAL(texture1, wrapper.get_texture(1));
            CHECK_EQUAL(0, wrapper.get_uniform_int(link0));
            CHECK_EQUAL(1, wrapper.get_uniform_int(link1));
        }
    }

    TEST(RepeatedTextureAllocation)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();
        wrapper.link();

        boost::shared_ptr<MockupTexture> texture(new MockupTexture(0));
        shade::GLSLWrapper::Handle link = wrapper.get_link_name("zero");

        wrapper.make_current();
        wrapper.UniformTexture(link, texture);
        wrapper.UniformTexture(link, texture);

        CHECK_EQUAL(0u, wrapper.get_max_texture_unit());
        if(wrapper.get_max_texture_unit() == 0)
        {
            CHECK_EQUAL(texture, wrapper.get_texture(0));
            CHECK_EQUAL(0, wrapper.get_uniform_int(link));
        }
    }

    TEST(CleanRestartInTextureAllocation)
    {
        GLSLMockupWrapper wrapper;
        wrapper.init();

        boost::shared_ptr<MockupTexture> texture0(new MockupTexture(0));
        shade::GLSLWrapper::Handle link0 = wrapper.get_link_name("zero");
        boost::shared_ptr<MockupTexture> texture1(new MockupTexture(1));
        shade::GLSLWrapper::Handle link1 = wrapper.get_link_name("one");

        wrapper.link();
        wrapper.make_current();
        wrapper.UniformTexture(link0, texture0);

        wrapper.link();
        wrapper.make_current();
        wrapper.UniformTexture(link1, texture1);

        CHECK_EQUAL(0u, wrapper.get_max_texture_unit());
        if(wrapper.get_max_texture_unit() == 0)
        {
            CHECK_EQUAL(texture1, wrapper.get_texture(0));
            CHECK_EQUAL(0, wrapper.get_uniform_int(link1));
        }
    }
}
