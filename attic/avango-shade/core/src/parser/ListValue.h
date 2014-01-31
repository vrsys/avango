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

#ifndef shade_parser_ListValue
#define shade_parser_ListValue shade_parser_ListValue

#include "Value.h"
#include <shade/ObjectMap.h>
#include <boost/shared_ptr.hpp>


namespace shade
{
  namespace parser
  {

    class ListValue : public Value
    {
    public:

      typedef ObjectMap::Index Index;

      ListValue(Index begin, Index end);

      Index begin(void) const { return m_begin; }
      Index end(void) const { return m_end; }

      /*virtual*/ bool is_constant(void) const;

    private:

      /*virtual*/ bool less_than(const Value& other) const;

      Index m_begin;
      Index m_end;
    };

  }
}


#endif /* shade_parser_ListValue */
