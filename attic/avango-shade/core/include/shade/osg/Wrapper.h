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

#ifndef shade_osg_Wrapper_H
#define shade_osg_Wrapper_H shade_osg_Wrapper_H

#include "../GLSLWrapper.h"
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <vector>

namespace shade
{
  namespace osg
  {  
    class Wrapper : public shade::GLSLWrapper
    {
    public:

      Wrapper(::osg::StateSet* state);

    protected:

      /*virtual*/ void set_texture(unsigned int unit, boost::shared_ptr<shade::Texture> texture);

      /*virtual*/ Handle CreateShader(ShaderType type);
      /*virtual*/ Handle CreateProgram(void);
      /*virtual*/ void AttachShader(Handle program, Handle shader);
      /*virtual*/ void ShaderSource(Handle shader, unsigned int count, const char** code, const int* length);
      /*virtual*/ void CompileShader(Handle shader);
      /*virtual*/ void LinkProgram(Handle program);
      /*virtual*/ void UseProgram(Handle program);
      /*virtual*/ void ProgramParameter(Handle program, Enum name, int value);
      /*virtual*/ Handle GetUniformLocation(Handle program, const char* name);

    public:

      /*virtual*/ boost::shared_ptr<shade::Texture> make_canvas_texture(void) const;

      /*virtual*/ void Uniform1i(Handle index, int value);
      /*virtual*/ void Uniform1f(Handle index, float x);
      /*virtual*/ void Uniform2f(Handle index, float x, float y);
      /*virtual*/ void Uniform3f(Handle index, float x, float y, float z);
      /*virtual*/ void Uniform4f(Handle index, float x, float y, float z, float w);
      /*virtual*/ void UniformMatrix4fv(Handle index, int count, bool transpose, const float* values);

    private:
      ::osg::ref_ptr< ::osg::StateSet > m_state;
      std::vector< ::osg::ref_ptr< ::osg::Program > > m_programs;
      std::vector< ::osg::ref_ptr< ::osg::Shader > > m_shaders;
      std::vector< ::osg::ref_ptr< ::osg::Uniform > > m_uniforms;
    };
  }
}

#endif /* shade_osg_Wrapper_H */
