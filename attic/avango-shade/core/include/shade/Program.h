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

#ifndef shade_Program_H
#define shade_Program_H shade_Program_H


#include "Shader.h"
#include "ShaderEnvironment.h"
#include "FileResolver.h"
#include "GLSLWrapper.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace shade
{

  class Component;

  class Program
  {
  public:

    Program(boost::shared_ptr<Shader> shader, boost::shared_ptr<GLSLWrapper> wrapper);

    // Setup environment
    void append_search_path(const std::string& path);
    void prepend_search_path(const std::string& path);

    // Generate source code
    void compile(void);
    bool requires_compilation(void);

    // Transfer values to GPU
    void upload(void);
    bool requires_upload(void);

    void use(bool activate = true);

  private:
    boost::shared_ptr<Shader> m_shader;
    boost::shared_ptr<GLSLWrapper> m_wrapper;
    shade::FileResolver m_resolver;
    boost::shared_ptr<shade::Component> m_component;
  };

}

#endif /* shade_Program_H */
