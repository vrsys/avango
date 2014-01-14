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

#ifndef shade_types_ObjrefAccessor_H
#define shade_types_ObjrefAccessor_H shade_types_ObjrefAccessor_H

#include "TypeAccessor.h"
#include <boost/shared_ptr.hpp>
#include <exception>

namespace shade
{
  class Shader;

  namespace types
  {
    class ObjrefAccessor : public TypeAccessor
    {
    public:
      virtual void set_generic(boost::shared_ptr<Shader> v) = 0;
      virtual boost::shared_ptr<Shader> get_generic(void) const = 0;

      class BadCast : public std::bad_cast
      {
      public:
        /*virtual*/ const char* what(void) const throw();
      };

      /*virtual*/ HashType hash(void) const;
    private:
      static HashValue m_hash;
    };
  }
}


#endif /* shade_types_ObjrefAccessor_H */
