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

#include <shade/GLSLARBWrapper.h>
#include <shade/GLSLTexture.h>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>


boost::shared_ptr<shade::Texture> shade::GLSLARBWrapper::make_canvas_texture(void) const
{
  return boost::shared_ptr<shade::Texture>();
}

void shade::GLSLARBWrapper::set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture)
{
  boost::shared_ptr<shade::GLSLTexture> glsl_texture(boost::dynamic_pointer_cast<shade::GLSLTexture>(texture));

  if (glsl_texture)
  {
    glActiveTexture(GL_TEXTURE0+unit);
    glsl_texture->enable();
  }
}

shade::GLSLARBWrapper::Handle shade::GLSLARBWrapper::CreateShader(ShaderType type)
{
  return glCreateShaderObjectARB(type);
}

shade::GLSLARBWrapper::Handle shade::GLSLARBWrapper::CreateProgram(void)
{
  return glCreateProgramObjectARB();
}

void shade::GLSLARBWrapper::AttachShader(Handle program, Handle shader)
{
  glAttachObjectARB(program, shader);
}

void shade::GLSLARBWrapper::ShaderSource(Handle shader, unsigned int count, const char** code, const int* length)
{
  glShaderSourceARB(shader, count, code, length);
}

void shade::GLSLARBWrapper::CompileShader(Handle shader)
{
  glCompileShaderARB(shader);
}

void shade::GLSLARBWrapper::LinkProgram(Handle program)
{
  glLinkProgramARB(program);
}

void shade::GLSLARBWrapper::UseProgram(Handle program)
{
  glUseProgramObjectARB(program);
}

void shade::GLSLARBWrapper::ProgramParameter(Handle program, Enum name, int value)
{
  glProgramParameteriEXT(program, name, value);
}

shade::GLSLARBWrapper::Handle shade::GLSLARBWrapper::GetUniformLocation(Handle program, const char* name)
{
  return glGetUniformLocationARB(program, name);
}

void shade::GLSLARBWrapper::Uniform1i(Handle index, int value)
{
  glUniform1iARB(index, value);
}

void shade::GLSLARBWrapper::Uniform1f(Handle index, float x)
{
  glUniform1fARB(index, x);
}

void shade::GLSLARBWrapper::Uniform2f(Handle index, float x, float y)
{
  glUniform2fARB(index, x, y);
}

void shade::GLSLARBWrapper::Uniform3f(Handle index, float x, float y, float z)
{
  glUniform3fARB(index, x, y, z);
}

void shade::GLSLARBWrapper::Uniform4f(Handle index, float x, float y, float z, float w)
{
  glUniform4fARB(index, x, y, z, w);
}

void shade::GLSLARBWrapper::UniformMatrix4fv(Handle index, int count, bool transpose, const float* values)
{
  glUniformMatrix4fvARB(index, count, transpose, values);
}
