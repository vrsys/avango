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

template<class Qualifier> shade::float_<Qualifier>::float_(void) :
  TypeBase<float_<Qualifier>, Qualifier>(formatter::Constants::float_),
  m_value(0.)
{}

template<class Qualifier> shade::float_<Qualifier>::float_(float v) :
  TypeBase<float_<Qualifier>, Qualifier>(formatter::Constants::float_),
  m_value(v)
{}

template<class Qualifier> void shade::float_<Qualifier>::set(float value)
{
  m_value = value;
  this->touch();
}

template<class Qualifier> void shade::float_<Qualifier>::get(float& value) const
{
  value = m_value;
}

template<class Qualifier>
template<class Q> void shade::float_<Qualifier>::copy_value(const float_<Q>& source)
{
  source.get(m_value);
  this->touch();
}

template<class Qualifier> std::string shade::float_<Qualifier>::get_constructor_str(void) const
{
  return "0";
}

template<class Qualifier> std::string shade::float_<Qualifier>::get_constructor_str(boost::shared_ptr<Type::State>) const
{
  std::ostringstream out;
  out << m_value;
  return out.str();
}

template<class Qualifier> void shade::float_<Qualifier>::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
  wrapper->Uniform1f(index, m_value);
}

template<class Qualifier> std::string shade::float_<Qualifier>::get_uniq_id(void) const
{
  return join_names(Qualifier::get_uniq_id(), "float");
}

template<class Qualifier> void shade::float_<Qualifier>::generate_constructor(formatter::Generator& generator) const
{
  generator.handle_literal_float(m_value);
}
