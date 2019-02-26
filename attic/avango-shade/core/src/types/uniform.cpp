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

#include <shade/types/uniform.h>
#include <shade/GLSLInstance.h>

shade::uniform::uniform(formatter::Constants::Type type) : Type(type) {}

bool shade::uniform::is_writable(const ShaderEnvironment& se) const { return false; }

shade::Type::LinkIndex shade::uniform::get_link_index(boost::shared_ptr<GLSLWrapper> wrapper, const std::string& name) const { return wrapper->get_link_name(name); }

void shade::uniform::upload(boost::shared_ptr<GLSLWrapper> wrapper, LinkIndex index) const { upload_uniform(wrapper, index); }

void shade::uniform::output_attribute(boost::shared_ptr<Type::State> state, Formatter* fmt, const std::string& obj, const std::string& name) const
{
    fmt->insert_attribute(m_type, obj, name, formatter::Constants::uniform);
}

std::string shade::uniform::get_uniq_id(void) const { return "uniform"; }
