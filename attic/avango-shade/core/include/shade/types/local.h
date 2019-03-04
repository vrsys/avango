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

#ifndef shade_types_local_H
#define shade_types_local_H shade_types_local_H

#include "../Type.h"

namespace shade
{
class local : public Type
{
  public:
    local(formatter::Constants::Type type);
    /*virtual*/ void output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const;
    /*virtual*/ bool output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const;
};
} // namespace shade

#endif /* shade_types_local_H */
