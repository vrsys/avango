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

#include <shade/Shader.h>
#include <string>
#include <sstream>


boost::shared_ptr<shade::Type> shade::Shader::call(const MethodAccessor& method)
{
  return method.invoke(this);
}


std::string shade::Shader::get_class_name(void) const
{
  static std::string empty("empty");
  return empty;
}

const std::string& shade::Shader::get_source_name(void) const
{
  static std::string empty("");
  return empty;
}

std::string shade::Shader::get_uniq_id(void) const
{
  std::ostringstream out;
  out << "obj_" << this;
  return out.str();
}

