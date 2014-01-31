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

#ifndef shade_GLSLMockupWrapper_H
#define shade_GLSLMockupWrapper_H shade_GLSLMockupWrapper_H

#include <shade/Texture.h>
#include <shade/GLSLWrapper.h>
#include <string>
#include <map>

struct MockupTexture : public shade::Texture
{
  MockupTexture(int v) :
    value(v)
  {
  }

  int value;
};

class GLSLMockupWrapper : public shade::GLSLWrapper
{
public:
  GLSLMockupWrapper(void);

  /*virtual*/ void set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture);
  /*virtual*/ boost::shared_ptr<shade::Texture> make_canvas_texture(void) const;

  /*virtual*/ Handle CreateShader(ShaderType type);
  /*virtual*/ Handle CreateProgram(void);
  /*virtual*/ void AttachShader(Handle program, Handle shader);
  /*virtual*/ void ShaderSource(Handle shader, unsigned int count, const char** code, const int* length);
  /*virtual*/ void CompileShader(Handle shader);
  /*virtual*/ void LinkProgram(Handle program);
  /*virtual*/ void UseProgram(Handle program);

  /*virtual*/ void ProgramParameter(Handle program, Enum name, int value);

  /*virtual*/ Handle GetUniformLocation(Handle program, const char* name);
  /*virtual*/ void Uniform1i(Handle index, int value);
  /*virtual*/ void Uniform1f(Handle index, float x);
  /*virtual*/ void Uniform2f(Handle index, float x, float y);
  /*virtual*/ void Uniform3f(Handle index, float x, float y, float z);
  /*virtual*/ void Uniform4f(Handle index, float x, float y, float z, float w);
  /*virtual*/ void UniformMatrix4fv(Handle index, int count, bool transpose, const float* values);

  bool has_program(void) const;
  bool was_attached(void) const;
  std::string get_code(void) const;
  bool was_compiled(void) const;
  bool was_linked(void) const;
  bool was_made_current(void) const;

  unsigned int get_max_texture_unit(void) const;
  boost::shared_ptr<MockupTexture> get_texture(unsigned int unit) const;

  int get_uniform_int(Handle index) const;

private:
  bool m_has_program;
  bool m_was_attached;
  std::string m_code;
  bool m_was_compiled;
  bool m_was_linked;
  bool m_was_made_current;

  int m_max_texture_unit;
  std::map<unsigned int, boost::shared_ptr<shade::Texture> > m_textures;

  std::map<unsigned int, int> m_uniform_ints;
};

#endif /* shade_GLSLMockupWrapper_H */
