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
#include <boost/shared_ptr.hpp>
#include <string>
#include <algorithm>

GLSLMockupWrapper::GLSLMockupWrapper(void) : m_has_program(false), m_was_attached(false), m_was_compiled(false), m_was_linked(false), m_was_made_current(false), m_max_texture_unit(-1) {}

void GLSLMockupWrapper::set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture)
{
    m_textures[unit] = texture;
    m_max_texture_unit = std::max(static_cast<int>(unit), m_max_texture_unit);
}

boost::shared_ptr<shade::Texture> GLSLMockupWrapper::make_canvas_texture(void) const { return boost::shared_ptr<shade::Texture>(new MockupTexture(0)); }

GLSLMockupWrapper::Handle GLSLMockupWrapper::GetUniformLocation(Handle program, const char* name)
{
    if(name == std::string("one"))
    {
        return 1;
    }
    return 0;
}

GLSLMockupWrapper::Handle GLSLMockupWrapper::CreateShader(ShaderType type) { return 1; }

GLSLMockupWrapper::Handle GLSLMockupWrapper::CreateProgram(void)
{
    m_has_program = true;
    return 1;
}

void GLSLMockupWrapper::AttachShader(Handle program, Handle shader) { m_was_attached = true; }

void GLSLMockupWrapper::ShaderSource(Handle shader, unsigned int count, const char** code, const int* length) { m_code = *code; }

void GLSLMockupWrapper::CompileShader(Handle shader) { m_was_compiled = true; }

void GLSLMockupWrapper::LinkProgram(Handle program)
{
    m_was_linked = true;
    m_textures.clear();
    m_max_texture_unit = -1;
}

void GLSLMockupWrapper::ProgramParameter(Handle program, Enum name, int value) {}

void GLSLMockupWrapper::UseProgram(Handle program) { m_was_made_current = true; }

void GLSLMockupWrapper::Uniform1i(Handle index, int value) { m_uniform_ints[index] = value; }

void GLSLMockupWrapper::Uniform1f(Handle index, float x) {}

void GLSLMockupWrapper::Uniform2f(Handle index, float x, float y) {}

void GLSLMockupWrapper::Uniform3f(Handle index, float x, float y, float z) {}

void GLSLMockupWrapper::Uniform4f(Handle index, float x, float y, float z, float w) {}

void GLSLMockupWrapper::UniformMatrix4fv(Handle index, int count, bool transpose, const float* values) {}

bool GLSLMockupWrapper::has_program(void) const { return m_has_program; }

bool GLSLMockupWrapper::was_attached(void) const { return m_was_attached; }

std::string GLSLMockupWrapper::get_code(void) const { return m_code; }

bool GLSLMockupWrapper::was_compiled(void) const { return m_was_compiled; }

bool GLSLMockupWrapper::was_linked(void) const { return m_was_linked; }

bool GLSLMockupWrapper::was_made_current(void) const { return m_was_made_current; }

unsigned int GLSLMockupWrapper::get_max_texture_unit(void) const { return m_max_texture_unit; }

boost::shared_ptr<MockupTexture> GLSLMockupWrapper::get_texture(unsigned int unit) const { return boost::dynamic_pointer_cast<MockupTexture>(m_textures.find(unit)->second); }

int GLSLMockupWrapper::get_uniform_int(Handle index) const
{
    if(m_uniform_ints.find(index) == m_uniform_ints.end())
        return -1;

    return m_uniform_ints.find(index)->second;
}
