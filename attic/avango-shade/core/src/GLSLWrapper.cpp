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

#include <shade/GLSLWrapper.h>

shade::GLSLWrapper::ShaderType shade::GLSLWrapper::m_env_to_gl[shade::num_shader_environment] = {GLSLWrapper::vertex_shader, GLSLWrapper::fragment_shader, GLSLWrapper::geometry_shader};

shade::GLSLWrapper::GLSLWrapper(void) : m_gl_program(0), m_gl_shader(shade::num_shader_environment) {}

void shade::GLSLWrapper::init(void)
{
    if(!m_gl_program)
        m_gl_program = CreateProgram();
}

bool shade::GLSLWrapper::has_shader(ShaderEnvironment env) const { return m_gl_shader[env]; }

void shade::GLSLWrapper::set_shader(ShaderEnvironment env, const std::string& code)
{
    if(m_gl_shader[env] == 0)
    {
        m_gl_shader[env] = CreateShader(m_env_to_gl[env]);
        AttachShader(m_gl_program, m_gl_shader[env]);
    }

    const char* code_str = code.c_str();
    ShaderSource(m_gl_shader[env], 1, &code_str, 0);
    CompileShader(m_gl_shader[env]);
}

void shade::GLSLWrapper::link(void)
{
    m_textures.clear();

    LinkProgram(m_gl_program);
}

std::string shade::GLSLWrapper::get_link_log(void) const { return GetProgrammInfoLog(m_gl_program); }

void shade::GLSLWrapper::make_current(void) { UseProgram(m_gl_program); }

void shade::GLSLWrapper::set_geometry_paramters(Enum input, Enum output, int vertices)
{
    ProgramParameter(m_gl_program, geometry_input_type, input);
    ProgramParameter(m_gl_program, geometry_output_type, output);
    ProgramParameter(m_gl_program, geometry_vertices_out, vertices);

    // FIXME remember input type and propagate (somehow?) to Formatter
}

shade::GLSLWrapper::Handle shade::GLSLWrapper::get_link_name(const std::string& name) { return GetUniformLocation(m_gl_program, name.c_str()); }

void shade::GLSLWrapper::UniformTexture(Handle index, boost::shared_ptr<Texture> texture)
{
    unsigned int unit;
    TextureIndexMap::const_iterator iter = m_textures.find(index);
    if(iter == m_textures.end())
    {
        unit = m_textures.size();
        Uniform1i(index, unit);
    }
    else
    {
        unit = iter->second;
    }

    set_texture(unit, texture);
    m_textures[index] = unit;
}

std::string shade::GLSLWrapper::GetProgrammInfoLog(Handle program) const { return std::string(); }
