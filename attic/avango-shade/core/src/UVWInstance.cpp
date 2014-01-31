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

#include <shade/UVWInstance.h>


boost::shared_ptr<shade::shaders::Value<shade::vec3, shade::local> > shade::UVWInstance::m_value;

void shade::UVWInstance::init(void)
{
  m_value = boost::shared_ptr<shaders::Value<vec3, local> >(new shaders::Value<vec3, local>);
}

boost::shared_ptr<shade::shaders::Value<shade::vec3, shade::local> > shade::UVWInstance::get(void)
{
  if (m_value)
    return m_value;

  throw NotInitialized();
}
