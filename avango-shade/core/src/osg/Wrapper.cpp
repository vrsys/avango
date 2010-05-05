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

#include <shade/osg/Wrapper.h>
#include <shade/osg/Texture.h>
#include <sstream>

shade::osg::Wrapper::Wrapper(::osg::StateSet* state) :
  m_state(state)
{
}

void shade::osg::Wrapper::set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture)
{
  boost::shared_ptr<shade::osg::Texture> osg_texture(boost::dynamic_pointer_cast<shade::osg::Texture>(texture));

  if (osg_texture)
  {
    osg_texture->bind(m_state.get(), unit);
  }
}

shade::osg::Wrapper::Handle shade::osg::Wrapper::CreateShader(ShaderType type)
{
  ::osg::Shader::Type osg_type(::osg::Shader::UNDEFINED);
  switch (type)
  {
  case vertex_shader:
    osg_type = ::osg::Shader::VERTEX;
    break;
  case fragment_shader:
    osg_type = ::osg::Shader::FRAGMENT;
    break;
  case geometry_shader:
    osg_type = ::osg::Shader::GEOMETRY;
    break;
  }

  ::osg::ref_ptr< ::osg::Shader > shader(new ::osg::Shader(osg_type));
  m_shaders.push_back(shader);
  return m_shaders.size();
}

shade::osg::Wrapper::Handle shade::osg::Wrapper::CreateProgram(void)
{
  ::osg::ref_ptr< ::osg::Program > program(new ::osg::Program);
  m_programs.push_back(program);
  return m_programs.size();
}

void shade::osg::Wrapper::AttachShader(Handle program, Handle shader)
{
  if ((program > m_programs.size()) || (shader > m_shaders.size()) )
    return;

  m_programs[program-1]->addShader(m_shaders[shader-1].get());
}

void shade::osg::Wrapper::ShaderSource(Handle shader, unsigned int count, const char** code, const int* length)
{
  std::ostringstream result;
  for (unsigned int i = 0; i != count; ++i)
  {
    if (length)
    {
      result.write(code[i], length[i]);
    }
    else
    {
      result << code[i];
    }
  }

  m_shaders[shader-1]->setShaderSource(result.str());
}

void shade::osg::Wrapper::CompileShader(Handle shader)
{
  /* noop */
}

void shade::osg::Wrapper::LinkProgram(Handle program)
{
  /* noop */
}

void shade::osg::Wrapper::UseProgram(Handle program)
{
  if (program > m_programs.size())
    return;

  m_state->setAttributeAndModes(m_programs[program-1].get(), ::osg::StateAttribute::ON);
}


void shade::osg::Wrapper::ProgramParameter(Handle program, Enum name, int value)
{
  if (program > m_programs.size())
    return;

  // Note: Our enums have GL values
  m_programs[program-1]->setParameter(name, value);
}


shade::osg::Wrapper::Handle shade::osg::Wrapper::GetUniformLocation(Handle program, const char* name)
{
  ::osg::ref_ptr< ::osg::Uniform > uniform(new ::osg::Uniform);
  uniform->setName(name);
  m_state->addUniform(uniform.get());
  m_uniforms.push_back(uniform);
  return m_uniforms.size();
}


boost::shared_ptr<shade::Texture> shade::osg::Wrapper::make_canvas_texture(void) const
{
  return boost::shared_ptr<shade::Texture>();
}

void shade::osg::Wrapper::Uniform1i(Handle index, int value)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::INT);
  m_uniforms[index-1]->set(value);
}

void shade::osg::Wrapper::Uniform1f(Handle index, float x)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::FLOAT);
  m_uniforms[index-1]->set(x);
}

void shade::osg::Wrapper::Uniform2f(Handle index, float x, float y)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::FLOAT_VEC2);
  m_uniforms[index-1]->set(::osg::Vec2(x, y));
}

void shade::osg::Wrapper::Uniform3f(Handle index, float x, float y, float z)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::FLOAT_VEC3);
  m_uniforms[index-1]->set(::osg::Vec3(x, y, z));
}

void shade::osg::Wrapper::Uniform4f(Handle index, float x, float y, float z, float w)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::FLOAT_VEC4);
  m_uniforms[index-1]->set(::osg::Vec4(x, y, z, w));
}

void shade::osg::Wrapper::UniformMatrix4fv(Handle index, int count, bool transpose, const float* values)
{
  if (index > m_uniforms.size())
    return;

  m_uniforms[index-1]->setType(::osg::Uniform::FLOAT_MAT4);
  // FIXME does OSG already transpose the matrix to conform with GL?
  m_uniforms[index-1]->set(::osg::Matrixf(values));
}

