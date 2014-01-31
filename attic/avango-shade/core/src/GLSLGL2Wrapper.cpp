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

#include <shade/GLSLGL2Wrapper.h>
#include <shade/GLSLTexture.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>


boost::shared_ptr<shade::Texture> shade::GLSLGL2Wrapper::make_canvas_texture(void) const
{
  return boost::shared_ptr<shade::Texture>();
}

void shade::GLSLGL2Wrapper::set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture)
{
  boost::shared_ptr<shade::GLSLTexture> glsl_texture(boost::dynamic_pointer_cast<shade::GLSLTexture>(texture));

  if (glsl_texture)
  {
    glActiveTexture(GL_TEXTURE0+unit);
    glsl_texture->enable();
  }
}

shade::GLSLGL2Wrapper::Handle shade::GLSLGL2Wrapper::CreateShader(ShaderType type)
{
  return glCreateShader(type);
}

shade::GLSLGL2Wrapper::Handle shade::GLSLGL2Wrapper::CreateProgram(void)
{
  return glCreateProgram();
}

void shade::GLSLGL2Wrapper::AttachShader(Handle program, Handle shader)
{
  glAttachShader(program, shader);
}

void shade::GLSLGL2Wrapper::ShaderSource(Handle shader, unsigned int count, const char** code, const int* length)
{
  glShaderSource(shader, count, code, length);
}

void shade::GLSLGL2Wrapper::CompileShader(Handle shader)
{
  glCompileShader(shader);
}

void shade::GLSLGL2Wrapper::LinkProgram(Handle program)
{
  glLinkProgram(program);
}

void shade::GLSLGL2Wrapper::UseProgram(Handle program)
{
  glUseProgram(program);
}

std::string shade::GLSLGL2Wrapper::GetProgrammInfoLog(Handle program) const
{
  GLint length;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

  GLchar* log = new GLchar[length];
  glGetProgramInfoLog(program, length, 0, log);

  std::string result(log);
  delete log;

  return result;
}

void shade::GLSLGL2Wrapper::ProgramParameter(Handle program, Enum name, int value)
{
  glProgramParameteriEXT(program, name, value);
}

shade::GLSLGL2Wrapper::Handle shade::GLSLGL2Wrapper::GetUniformLocation(Handle program, const char* name)
{
  return glGetUniformLocation(program, name);
}

void shade::GLSLGL2Wrapper::Uniform1i(Handle index, int value)
{
  glUniform1i(index, value);
}

void shade::GLSLGL2Wrapper::Uniform1f(Handle index, float x)
{
  glUniform1f(index, x);
}

void shade::GLSLGL2Wrapper::Uniform2f(Handle index, float x, float y)
{
  glUniform2f(index, x, y);
}

void shade::GLSLGL2Wrapper::Uniform3f(Handle index, float x, float y, float z)
{
  glUniform3f(index, x, y, z);
}

void shade::GLSLGL2Wrapper::Uniform4f(Handle index, float x, float y, float z, float w)
{
  glUniform4f(index, x, y, z, w);
}

void shade::GLSLGL2Wrapper::UniformMatrix4fv(Handle index, int count, bool transpose, const float* values)
{
  glUniformMatrix4fv(index, count, transpose, values);
}
