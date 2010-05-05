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

#include "../Shader.h"
#include <avango/Create.h>
#include <avango/FieldContainer.h>
#include <boost/shared_ptr.hpp>

namespace av
{
  namespace shade
  {
    namespace detail
    {
      template<class T, class W>
      class ShaderCreate : public Create
      {
      public:

        ShaderCreate(const std::string& name)
        {
          mType = av::Type::createType(FieldContainer::getClassTypeId(), name, this, true);
        }

        /*virtual*/ av::Typed* makeInstance() const
        {
          boost::shared_ptr<T> shade_shader(new T());
          W* shader = new W(mType, shade_shader);
          return shader;
        }

      private:
        av::Type mType;
      };
    }
  }
}

template<class Shader>
/*static*/ void
av::shade::Init::registerShader(const std::string& name)
{
  registerWrappedShader<Shader, ::av::shade::Shader>(name);
}

template<class Shader, class Wrapper>
/*static*/ void
av::shade::Init::registerWrappedShader(const std::string& name)
{
  FieldContainer::initClass();
  // Object registers itself and is thus not leaked
  new detail::ShaderCreate<Shader, Wrapper>(name);
}
