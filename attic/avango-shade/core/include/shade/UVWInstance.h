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

#ifndef shade_UVWInstance_H
#define shade_UVWInstance_H shade_UVWInstance_H

#include "shaders/Value.h"
#include "types/vec3.h"
#include "types/local.h"
#include <boost/shared_ptr.hpp>

namespace shade
{
  class UVWInstance
  {
  public:
    static void init(void);
    static boost::shared_ptr<shaders::Value<vec3, local> > get(void);

    class NotInitialized {};

  private:
    static boost::shared_ptr<shaders::Value<vec3, local> > m_value;
  };
}

#endif /* shade_UVWInstance_H */
