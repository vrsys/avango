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

#ifndef shade_parser_ValueList_H
#define shade_parser_ValueList_H shade_parser_ValueList_H

#include <vector>
#include <boost/shared_ptr.hpp>


namespace shade
{
  namespace parser
  {

    class Value;

    class ValueList
    {
    private:

      typedef std::vector<boost::shared_ptr<const Value> > Parameters;
      typedef Parameters::size_type size_type;
      Parameters m_parameters;

    public:

      void add(boost::shared_ptr<const Value> parameter);

      bool operator<(const ValueList& other) const;

      bool has_self_parameter(void) const;
      const Value& get_self_parameter(void) const;

      typedef std::vector<boost::shared_ptr<const Value> >::iterator iterator;
      typedef std::vector<boost::shared_ptr<const Value> >::const_iterator const_iterator;

      const_iterator begin(void) const { return m_parameters.begin(); }
      const_iterator end(void) const { return m_parameters.end(); }
      size_type size(void) const { return m_parameters.size(); }
    };

  }
}


#endif /* shade_parser_ValueList_H */
