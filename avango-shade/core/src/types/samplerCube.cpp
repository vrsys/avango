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

#include <shade/types/samplerCube.h>


shade::samplerCube::samplerCube(void) :
  TypeBase<samplerCube, shade::uniform>(formatter::Constants::samplerCube)
{
}

void shade::samplerCube::set(boost::shared_ptr<Texture> texture)
{
  m_texture = texture;
  this->touch();
}

boost::shared_ptr<shade::Texture> shade::samplerCube::get(void) const
{
  return m_texture;
}

void shade::samplerCube::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
  wrapper->UniformTexture(index, m_texture);
}

std::string shade::samplerCube::get_uniq_id(void) const
{
  return "samplerCube";
}
