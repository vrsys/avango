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

#ifndef shade_GLSLWrapper_H
#define shade_GLSLWrapper_H shade_GLSLWrapper_H

#include "ShaderEnvironment.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <string>

namespace shade
{
  class Texture;

  class GLSLWrapper
  {
  public:
    GLSLWrapper(void);
    virtual ~GLSLWrapper(void) {};

    void init(void);
    bool has_shader(ShaderEnvironment env) const;
    void set_shader(ShaderEnvironment env, const std::string& code);
    void link(void);
    std::string get_link_log(void) const;
    void make_current(void);

    typedef unsigned int Enum;
    void set_geometry_paramters(Enum input, Enum output, int vertices);

    typedef unsigned int Handle;
    Handle get_link_name(const std::string& name);

    enum ShaderType
    {
      fragment_shader = 0x8B30,
      vertex_shader = 0x8B31,
      geometry_shader = 0x8DD9
    };
    enum GeometryParameter
    {
      geometry_vertices_out = 0x8DDA,
      geometry_input_type = 0x8DDB,
      geometry_output_type = 0x8DDC
    };

    virtual boost::shared_ptr<Texture> make_canvas_texture(void) const = 0;

  protected:
    virtual void set_texture(unsigned int unit, boost::shared_ptr<Texture> texture) = 0;

  protected:
    virtual Handle CreateShader(ShaderType type) = 0;
    virtual Handle CreateProgram(void) = 0;
    virtual void AttachShader(Handle program, Handle shader) = 0;
    virtual void ShaderSource(Handle shader, unsigned int count, const char** code, const int* length) = 0;
    virtual void CompileShader(Handle shader) = 0;
    virtual void LinkProgram(Handle program) = 0;
    virtual void UseProgram(Handle program) = 0;
    virtual std::string GetProgrammInfoLog(Handle program) const;

    virtual void ProgramParameter(Handle program, Enum name, int value) = 0;

    virtual Handle GetUniformLocation(Handle program, const char* name) = 0;

  public:
    virtual void Uniform1i(Handle index, int value) = 0;
    virtual void Uniform1f(Handle index, float x) = 0;
    virtual void Uniform2f(Handle index, float x, float y) = 0;
    virtual void Uniform3f(Handle index, float x, float y, float z) = 0;
    virtual void Uniform4f(Handle index, float x, float y, float z, float w) = 0;
    virtual void UniformMatrix4fv(Handle index, int count, bool transpose, const float* values) = 0;

    void UniformTexture(Handle index, boost::shared_ptr<Texture> texture);

  private:
    Handle m_gl_program;
    std::vector<Handle> m_gl_shader;
    typedef std::map<unsigned int, unsigned int> TextureIndexMap;
    TextureIndexMap m_textures;

    static ShaderType m_env_to_gl[shade::num_shader_environment];
  };
}

#endif /* shade_GLSLWrapper_H */
