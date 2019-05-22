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

#ifndef shade_types_usampler2D_H
#define shade_types_usampler2D_H shade_types_usampler2D_H

#include "../TypeBase.h"
#include "TextureAccessor.h"
#include "uniform.h"
#include <boost/shared_ptr.hpp>

namespace shade
{
class Texture;

class usampler2D : public TypeBase<usampler2D, uniform>, public types::TextureAccessor
{
  public:
    usampler2D(void);

    /*virtual*/ void set(boost::shared_ptr<Texture> texture);
    /*virtual*/ boost::shared_ptr<Texture> get(void) const;

    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void upload_uniform(boost::shared_ptr<GLSLWrapper>, shade::Type::LinkIndex) const;

  private:
    boost::shared_ptr<Texture> m_texture;
};
} // namespace shade

#endif /* shade_types_usampler2D_H */
