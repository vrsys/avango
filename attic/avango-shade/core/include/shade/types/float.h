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

#ifndef shade_types_float_H
#define shade_types_float_H shade_types_float_H

#include "../TypeBase.h"
#include "FloatAccessor.h"

namespace shade
{
template <class Qualifier = Type>
class float_ : public TypeBase<float_<Qualifier>, Qualifier>, public types::FloatAccessor
{
  public:
    float_(void);
    float_(float v);
    template <class Q>
    void copy_value(const float_<Q>& source);

    /*virtual*/ void set(float v);
    /*virtual*/ void get(float& v) const;

    /*virtual*/ void upload_uniform(boost::shared_ptr<GLSLWrapper>, shade::Type::LinkIndex) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_constructor_str(boost::shared_ptr<Type::State> state) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;

  private:
    float m_value;
};
} // namespace shade

#include "impl/float_impl.cpp"

#endif /* shade_types_float_H */
