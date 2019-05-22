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

#include "ValueList.h"
#include "Value.h"

using namespace shade::parser;

void ValueList::add(boost::shared_ptr<const Value> parameter) { m_parameters.push_back(parameter); }

bool ValueList::operator<(const ValueList& other) const
{
    if(m_parameters.size() != other.m_parameters.size())
        return m_parameters.size() < other.m_parameters.size();

    for(size_type i = 0; i != m_parameters.size(); ++i)
    {
        const Value& a(*m_parameters[i]);
        const Value& b(*other.m_parameters[i]);

        if(!(!(a < b) && !(b < a)))
            return a < b;
    }

    return false;
}

bool ValueList::has_self_parameter(void) const { return !m_parameters.empty(); }

const Value& ValueList::get_self_parameter(void) const { return *m_parameters.front(); }
