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

#ifndef shade_AttributeAccessor_H
#define shade_AttributeAccessor_H shade_AttributeAccessor_H

#include "Type.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace shade
{

  class Shader;
  class AttributeType;

  class AttributeAccessor
  {
  public:
    AttributeAccessor(void);
    AttributeAccessor(boost::shared_ptr<AttributeType> impl, const std::string& name);

    const std::string& get_name(void) const;
    const Type& get_type(const Shader* obj) const;
    Type* get_public_type(Shader* obj) const;

  private:
    boost::shared_ptr<AttributeType> m_impl;
    std::string m_name;
  };


  class AttributeType
  {
  public:
    virtual ~AttributeType(void) {}
    virtual const Type& get_type(const Shader* obj) const = 0;
    virtual Type* get_public_type(Shader* obj) const = 0;
  };

  template<class Base, class T>
    class AttributeType_impl : public AttributeType
  {
  public:
    AttributeType_impl(T Base::* mem, bool public_ = false) :
      m_mem(mem),
      m_public(public_)
    {
    }

    /*virtual*/ const Type& get_type(const Shader* obj) const
    {
      return dynamic_cast<const Base*>(obj)->*m_mem;
    }

    /*virtual*/ Type* get_public_type(Shader* obj) const
    {
      if (!m_public)
        return 0;
      return &(dynamic_cast<Base*>(obj)->*m_mem);
    }

  private:
    T Base::* m_mem;
    bool m_public;
  };

}

#endif /* shade_AttributeAccessor_H */
