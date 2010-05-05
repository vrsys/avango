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

#ifndef shade_osg_Texture_H
#define shade_osg_Texture_H shade_osg_Texture_H

#include "../Texture.h"
#include <osg/ref_ptr>

namespace osg
{
  class Texture;
  class StateSet;
}

namespace shade
{
  namespace osg
  {
    class Texture : public ::shade::Texture
    {
    public:
      Texture(::osg::Texture* texture);

      bool operator==(const Texture& other) const;
      bool operator!=(const Texture& other) const;

      void bind(::osg::StateSet* state, unsigned int unit) const;

      ::osg::Texture* get(void) const;

    private:
      ::osg::ref_ptr< ::osg::Texture> m_texture;
    };
  }
}

#endif /* shade_osg_Texture_H */
