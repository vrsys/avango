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

#include "shader_lib_path.h"
#include <shade/Program.h>
#include <shade/FileAccumulator.h>
#include <shade/GLSLInstance.h>
#include <shade/PipelineState.h>
#include <shade/transform.h>
#include "Component.h"
#include "formatter/GLSLFormatter.h"
#include "formatter/TraversalGenerator.h"
#include "formatter/GL2Generator.h"
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include <sys/time.h>
#include <time.h>

namespace
{
class ScopeTimer
{
  public:
    ScopeTimer(double& time) : time_(time) { gettimeofday(&start_, 0); }

    ~ScopeTimer(void)
    {
        struct timeval stop;
        gettimeofday(&stop, 0);
        double timediff = get_time(stop) - get_time(start_);
        time_ += timediff;
    }

  private:
    double get_time(const struct timeval& tv) { return static_cast<double>(tv.tv_sec) + static_cast<double>(tv.tv_usec) / 1000000.; }

    struct timeval start_;
    double& time_;
};
} // namespace

shade::Program::Program(boost::shared_ptr<Shader> shader, boost::shared_ptr<GLSLWrapper> wrapper) : m_shader(shader), m_wrapper(wrapper), m_component(new Component(shader))
{
    char* shaderdir = std::getenv("SHADE_SHADER_PATH");
    if(shaderdir != 0)
        append_env_path(m_resolver, shaderdir);
    m_resolver.append_path(shader_lib_path);
}

void shade::Program::append_search_path(const std::string& path) { m_resolver.append_path(path); }

void shade::Program::prepend_search_path(const std::string& path) { m_resolver.prepend_path(path); }

void shade::Program::compile(void)
{
    double GLSL_compile_time = 0.;
    double SHADE_build_time = 0.;
    double SHADE_transform_time = 0.;
    std::ostringstream error_log;

    m_wrapper->init();

    shade::FileAccumulator acc(m_resolver);
    PipelineState ps;
    ps.set_geometry_shader(m_component->has_entry(geometry_shader));
    ps.set_root_shader(m_shader);
    m_component->setup(acc, ps);

    for(int i = 0; i != shade::num_shader_environment; ++i)
    {
        ShaderEnvironment env = static_cast<ShaderEnvironment>(i);
        if(!m_component->has_entry(env))
            continue;

        error_log << "Compiling " << get_shader_env_define(env) << ":" << std::endl;

        formatter::GL2Generator code_generator(env);
        int sl_version = acc.get_glsl_version();
        if(env == geometry_shader)
            sl_version = std::max(120, sl_version);
        if(sl_version != 0)
            code_generator.insert_sl_version(sl_version);
        if(env == geometry_shader)
            code_generator.insert_extension("GL_EXT_geometry_shader4", formatter::GL2Generator::Extension::require);
        if(acc.requires_gpu_shader4_ext())
            code_generator.insert_extension("GL_EXT_gpu_shader4", formatter::GL2Generator::Extension::require);
        code_generator.insert_define(get_shader_env_define(env), "1");
        code_generator.insert_type_definitions();

        formatter::TraversalGenerator optimizing_generator;
        shade::GLSLFormatter fmt(env, optimizing_generator);
        {
            ScopeTimer timer(SHADE_build_time);
            m_component->compile(fmt, acc, env, formatter::Constants::triangles);
        }
        {
            ScopeTimer timer(SHADE_transform_time);
            fmt.generate(optimizing_generator);
        }
        optimizing_generator.traverse(code_generator, error_log);
        std::string code(code_generator.get_code());

        if(std::getenv("SHADE_DEBUG_SHADER_OUTPUT"))
        {
            std::string output_name("shade-shader-" + boost::lexical_cast<std::string>(this) + "-" + boost::lexical_cast<std::string>(env) + ".glsl");
            std::ofstream hout(output_name.c_str());
            hout << code;
        }
        if(std::getenv("SHADE_DEBUG_SHADER_INPUT"))
        {
            std::string input_name("shade-shader-" + boost::lexical_cast<std::string>(env) + ".glsl");
            std::ifstream hin(input_name.c_str());
            std::ostringstream contents;
            contents << hin.rdbuf();
            code = contents.str();
        }

        {
            ScopeTimer timer(GLSL_compile_time);
            m_wrapper->set_shader(env, code);
        }

        error_log << std::endl;
    }

    {
        ScopeTimer timer(GLSL_compile_time);
        m_wrapper->link();
    }
    m_component->link(m_wrapper);

    if(std::getenv("SHADE_DEBUG"))
    {
        std::cout << error_log.str();
        std::cout << "GL info log:" << std::endl;
        std::cout << m_wrapper->get_link_log() << std::endl;
    }

    if(std::getenv("SHADE_PROFILING"))
        std::cout << GLSL_compile_time << "  " << SHADE_build_time << "  " << SHADE_transform_time << std::endl;
}

bool shade::Program::requires_compilation(void) { return m_component->requires_compilation(); }

void shade::Program::upload(void) { m_component->upload(m_wrapper); }

bool shade::Program::requires_upload(void) { return m_component->requires_upload(); }

void shade::Program::use(bool activate) {}
