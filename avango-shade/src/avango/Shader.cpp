// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include "../include/avango/shade/Shader.h"
#include "../include/avango/shade/Init.h"
#include "../include/avango/shade/FieldAdapter.h"
#include "../../core/include/shade/Shader.h"
#include "../../core/include/shade/types/TypeAccessor.h"
#include <avango/StandardFields.h>
#include <avango/osg/Fields.h>

namespace
{
  class RegisterFields
   {
   public:

     RegisterFields(av::shade::Shader* shader) :
       mShader(shader)
     {
     }

     void operator()(shade::Type* type, const std::string& name)
     {
       shade::types::TypeAccessor* accessor(dynamic_cast<shade::types::TypeAccessor*>(type));
       if (accessor == 0)
         return;

       boost::shared_ptr<av::shade::FieldAdapter> adapter(av::shade::Init::getFieldAdapter(accessor->hash()));
       if (!adapter)
         return;

       adapter->bindField(type, name, mShader);
     }
    
   private:
    av::shade::Shader* mShader;
   };
}

av::shade::Shader::Shader(av::Type type, boost::shared_ptr< ::shade::Shader> shader) :
  mType(type),
  mShader(shader)
{
  if (!mWrapper)
  {
    mWrapper = new Wrapper;
  }

  shader->traverse_public_attributes(RegisterFields(this));
  (*mWrapper)[mShader.get()] = this;
}

av::shade::Shader::~Shader(void)
{
  mWrapper->erase(mShader.get());

  if (mWrapper->empty())
  {
    delete mWrapper;
    mWrapper = 0;
  }
}

av::Type
av::shade::Shader::getTypeId() const
{
  return mType;
}

boost::shared_ptr< ::shade::Shader>
av::shade::Shader::getShader() const
{
  return mShader;
}

/*static*/ av::shade::Shader*
av::shade::Shader::getWrapper(::shade::Shader* shader)
{
  Shader* result(0);
  Wrapper::const_iterator av_shader(mWrapper->find(shader));
  if (av_shader != mWrapper->end())
    result = av_shader->second;
  return result;
}

/*static*/ av::shade::Shader::Wrapper* av::shade::Shader::mWrapper = 0;
