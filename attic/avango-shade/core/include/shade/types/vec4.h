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

#ifndef shade_types_vec4_H
#define shade_types_vec4_H shade_types_vec4_H

#include "../TypeBase.h"
#include "Vec4Accessor.h"
#include "local.h"

namespace shade
{
  template<class Qualifier = Type> class vec4 :
    public TypeBase<vec4<Qualifier>, Qualifier>,
    public types::Vec4Accessor
  {
  public:
    vec4(void);
    vec4(float x, float y, float z, float w);
    template<class Q> void copy_value(const vec4<Q>& source);

    /*virtual*/ void set(float x, float y, float z, float w);
    /*virtual*/ void get(float& x, float& y, float& z, float& w) const;

    /*virtual*/ void upload_uniform(boost::shared_ptr<GLSLWrapper>, shade::Type::LinkIndex) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_constructor_str(boost::shared_ptr<Type::State> state) const;

  private:
    float m_x, m_y, m_z, m_w;
  };
}

#include "impl/vec4_impl.cpp"

#endif /* shade_types_vec4_H */
