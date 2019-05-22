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

template <class Qualifier>
shade::vec4<Qualifier>::vec4(void) : TypeBase<vec4<Qualifier>, Qualifier>(formatter::Constants::vec4), m_x(0.), m_y(0.), m_z(0.), m_w(0.)
{
}

template <class Qualifier>
shade::vec4<Qualifier>::vec4(float x, float y, float z, float w) : TypeBase<vec4<Qualifier>, Qualifier>(formatter::Constants::vec4), m_x(x), m_y(y), m_z(z), m_w(w)
{
}

template <class Qualifier>
void shade::vec4<Qualifier>::set(float x, float y, float z, float w)
{
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
    this->touch();
}

template <class Qualifier>
void shade::vec4<Qualifier>::get(float& x, float& y, float& z, float& w) const
{
    x = m_x;
    y = m_y;
    z = m_z;
    w = m_w;
}

template <class Qualifier>
template <class Q>
void shade::vec4<Qualifier>::copy_value(const vec4<Q>& source)
{
    source.get(m_x, m_y, m_z, m_w);
    this->touch();
}

template <class Qualifier>
std::string shade::vec4<Qualifier>::get_constructor_str(void) const
{
    return "0, 0, 0, 0";
}

template <class Qualifier>
std::string shade::vec4<Qualifier>::get_constructor_str(boost::shared_ptr<Type::State>) const
{
    std::ostringstream out;
    out << m_x << ", " << m_y << ", " << m_z << ", " << m_w;
    return out.str();
}

template <class Qualifier>
void shade::vec4<Qualifier>::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
    wrapper->Uniform4f(index, m_x, m_y, m_z, m_w);
}

template <class Qualifier>
std::string shade::vec4<Qualifier>::get_uniq_id(void) const
{
    return join_names(Qualifier::get_uniq_id(), "vec4");
}

template <class Qualifier>
void shade::vec4<Qualifier>::generate_constructor(formatter::Generator& generator) const
{
    generator.handle_literal_vec4(m_x, m_y, m_z, m_w);
}
