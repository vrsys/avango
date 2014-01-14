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

#include "../../GLSLInstance.h"
#include <sstream>


template<class Qualifier> shade::vec3<Qualifier>::vec3(void) :
  TypeBase<vec3<Qualifier>, Qualifier>(formatter::Constants::vec3),
  m_x(0.), m_y(0.), m_z(0.)
{}

template<class Qualifier> shade::vec3<Qualifier>::vec3(float x, float y, float z) :
  TypeBase<vec3<Qualifier>, Qualifier>(formatter::Constants::vec3),
  m_x(x), m_y(y), m_z(z)
{} 

template<class Qualifier> void shade::vec3<Qualifier>::set(float x, float y, float z)
{
  m_x = x;
  m_y = y;
  m_z = z;
  this->touch();
}

template<class Qualifier> void shade::vec3<Qualifier>::get(float& x, float& y, float& z) const
{
  x = m_x;
  y = m_y;
  z = m_z;
}

template<class Qualifier>
template<class Q> void shade::vec3<Qualifier>::copy_value(const vec3<Q>& source)
{
  source.get(m_x, m_y, m_z);
  this->touch();
}

template<class Qualifier> std::string shade::vec3<Qualifier>::get_constructor_str(void) const
{
  return "0, 0, 0";
}

template<class Qualifier> std::string shade::vec3<Qualifier>::get_constructor_str(boost::shared_ptr<Type::State>) const
{
  std::ostringstream out;
  out << m_x << ", " << m_y << ", " << m_z;
  return out.str();
}

template<class Qualifier> void shade::vec3<Qualifier>::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
  wrapper->Uniform3f(index, m_x, m_y, m_z);
}

template<class Qualifier> std::string shade::vec3<Qualifier>::get_uniq_id(void) const
{
  return join_names(Qualifier::get_uniq_id(), "vec3");
}

template<class Qualifier> void shade::vec3<Qualifier>::generate_constructor(formatter::Generator& generator) const
{
  generator.handle_literal_vec3(m_x, m_y, m_z);
}
