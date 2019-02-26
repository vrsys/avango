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

#ifndef shade_parser_ScopeLayer_H
#define shade_parser_ScopeLayer_H shade_parser_ScopeLayer_H

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace shade
{
namespace parser
{
class Value;

class ScopeLayer
{
  public:
    void add_value(const std::string& var, boost::shared_ptr<const Value> value);

    boost::shared_ptr<const Value> get_value(const std::string& var) const;
    boost::shared_ptr<const Value> get_raw_value(const std::string& var) const;

  private:
    std::map<std::string, boost::shared_ptr<const Value>> values_;
};
} // namespace parser
} // namespace shade

#endif /* shade_parser_ScopeLayer_H */
