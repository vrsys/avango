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
shade::matrix4<Qualifier>::matrix4(void) : TypeBase<matrix4<Qualifier>, Qualifier>(formatter::Constants::matrix4)
{
}

template <class Qualifier>
shade::matrix4<Qualifier>::matrix4(const float* values) : TypeBase<matrix4<Qualifier>, Qualifier>(formatter::Constants::matrix4)
{
    this->set(values);
}

template <class Qualifier>
void shade::matrix4<Qualifier>::set(const float* values)
{
    for(int i = 0; i != 16; ++i)
    {
        m_values[i] = values[i];
    }
    this->touch();
}

template <class Qualifier>
void shade::matrix4<Qualifier>::get(float* values) const
{
    for(int i = 0; i != 16; ++i)
    {
        values[i] = m_values[i];
    }
}

template <class Qualifier>
template <class Q>
void shade::matrix4<Qualifier>::copy_value(const matrix4<Q>& source)
{
    source.get(m_values);
    this->touch();
}

template <class Qualifier>
std::string shade::matrix4<Qualifier>::get_constructor_str(void) const
{
    return "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0";
}

template <class Qualifier>
std::string shade::matrix4<Qualifier>::get_constructor_str(boost::shared_ptr<Type::State>) const
{
    std::ostringstream out;
    for(int i = 0; i != 15; ++i)
    {
        out << m_values[i] << ", ";
    }
    out << m_values[15];
    return out.str();
}

template <class Qualifier>
void shade::matrix4<Qualifier>::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
    wrapper->UniformMatrix4fv(index, 1, false, m_values);
}

template <class Qualifier>
std::string shade::matrix4<Qualifier>::get_uniq_id(void) const
{
    return join_names(Qualifier::get_uniq_id(), "mat4");
}
