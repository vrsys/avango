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

#include <shade/types/usampler2D.h>


shade::usampler2D::usampler2D(void) :
  TypeBase<usampler2D, shade::uniform>(formatter::Constants::usampler2D)
{
}

void shade::usampler2D::set(boost::shared_ptr<Texture> texture)
{
  m_texture = texture;
  this->touch();
}

boost::shared_ptr<shade::Texture> shade::usampler2D::get(void) const
{
  return m_texture;
}

void shade::usampler2D::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
  wrapper->UniformTexture(index, m_texture);
}

std::string shade::usampler2D::get_uniq_id(void) const
{
  return "usampler2D";
}
