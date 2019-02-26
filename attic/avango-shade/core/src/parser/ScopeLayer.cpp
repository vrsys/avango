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

#include "ScopeLayer.h"
#include "Value.h"

using namespace shade::parser;

void shade::parser::ScopeLayer::add_value(const std::string& var, boost::shared_ptr<const Value> value) { values_[var] = value; }

boost::shared_ptr<const Value> shade::parser::ScopeLayer::get_raw_value(const std::string& var) const
{
    std::map<std::string, boost::shared_ptr<const Value>>::const_iterator i(values_.find(var));
    if(i == values_.end())
        return boost::shared_ptr<const Value>();
    return i->second;
}

boost::shared_ptr<const Value> shade::parser::ScopeLayer::get_value(const std::string& var) const
{
    boost::shared_ptr<const Value> result(get_raw_value(var));
    if(!result)
    {
        result = boost::shared_ptr<const Value>(new Value());
    }
    return result;
}
