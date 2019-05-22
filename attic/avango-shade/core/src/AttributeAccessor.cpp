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

#include <shade/AttributeAccessor.h>

shade::AttributeAccessor::AttributeAccessor(void) : m_name("") {}

shade::AttributeAccessor::AttributeAccessor(boost::shared_ptr<AttributeType> impl, const std::string& name) : m_impl(impl), m_name(name) {}

const std::string& shade::AttributeAccessor::get_name(void) const { return m_name; }

const shade::Type& shade::AttributeAccessor::get_type(const Shader* obj) const { return m_impl->get_type(obj); }

shade::Type* shade::AttributeAccessor::get_public_type(Shader* obj) const { return m_impl->get_public_type(obj); }
