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

#include <shade/types/sampler2DArray.h>


shade::sampler2DArray::sampler2DArray(void) :
  TypeBase<sampler2DArray, shade::uniform>(formatter::Constants::sampler2DArray)
{
}

void shade::sampler2DArray::set(boost::shared_ptr<Texture> texture)
{
  m_texture = texture;
  this->touch();
}

boost::shared_ptr<shade::Texture> shade::sampler2DArray::get(void) const
{
  return m_texture;
}

void shade::sampler2DArray::upload_uniform(boost::shared_ptr<GLSLWrapper> wrapper, shade::Type::LinkIndex index) const
{
  wrapper->UniformTexture(index, m_texture);
}

std::string shade::sampler2DArray::get_uniq_id(void) const
{
  return "sampler2DArray";
}
