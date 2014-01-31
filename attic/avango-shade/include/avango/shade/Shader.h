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

#ifndef AVANGO_SHADE_SHADER_H
#define AVANGO_SHADE_SHADER_H

#include <avango/FieldContainer.h>
#include <boost/shared_ptr.hpp>
#include "windows_specific_shade.h"

namespace shade
{
  class Shader;
}

namespace av
{
  namespace shade
  {
    class AV_SHADE_DLL Shader : public av::FieldContainer
    {
    public:

      Shader(av::Type type, boost::shared_ptr< ::shade::Shader> shader);
      ~Shader(void);

      /*virtual*/ av::Type getTypeId() const;

      boost::shared_ptr< ::shade::Shader> getShader() const;
      static Shader* getWrapper(::shade::Shader* shader);

    private:
      av::Type mType;
      boost::shared_ptr< ::shade::Shader> mShader;
      typedef std::map< ::shade::Shader*, Shader*> Wrapper;
      static Wrapper* mWrapper;
    };
  }
}

#endif
