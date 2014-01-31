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

#ifndef AVANGO_SHADE_INIT_H
#define AVANGO_SHADE_INIT_H

/**
 * \file
 * \ingroup av_shade
 */

#include "core/types/TypeAccessor.h"
#include "FieldAdapter.h"
#include <avango/Typed.h>
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>
#include "windows_specific_shade.h"

namespace shade
{
  class Program;
}

namespace av
{
  namespace shade
  {
    /**
     * Initializes the AVANGO SHADE Integration library.
     *
     * \ingroup av_shade
     */
    class AV_SHADE_DLL Init
    {
      AV_TYPED_DECLARE_ABSTRACT();

    public:

      template<class Shader> static void registerShader(const std::string& name);
      template<class Shader, class Wrapper> static void registerWrappedShader(const std::string& name);

      static void registerFieldAdapter(boost::shared_ptr<FieldAdapter> adapter);
      static boost::shared_ptr<FieldAdapter> getFieldAdapter(::shade::types::TypeAccessor::HashType hash);

      static void initProgram(boost::shared_ptr< ::shade::Program> program);
      static void appendSearchPath(const std::string& path);

    private:
      static std::map< ::shade::types::TypeAccessor::HashType, boost::shared_ptr<FieldAdapter> > mFieldAdapters;
      static std::vector<std::string> mSearchPaths;
    };
  }
}

#include "impl/Init_impl.cpp"

#endif
