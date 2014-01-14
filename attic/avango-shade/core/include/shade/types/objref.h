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

#ifndef shade_types_objref_H
#define shade_types_objref_H shade_types_objref_H

#include "../TypeBase.h"
#include "../Shader.h"
#include "ObjrefAccessor.h"
#include "local.h"

namespace shade
{
  template<class P = Shader*, class Qualifier = Type> class objref_base : public TypeBase<objref_base<P, Qualifier>, Qualifier>
  {
  public:
    objref_base(void);

    P operator->(void);
    P get(void) const;
    P& operator=(const P& v);
    operator bool(void);

    /*virtual*/ boost::shared_ptr<Type::State> create_state(const PipelineState& ps) const;
    /*virtual*/ void collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)>) const;
    /*virtual*/ void insert_references(boost::shared_ptr<Type::State>, ObjectMap&) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_constructor_str(boost::shared_ptr<Type::State> state) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const;

    class State : public Type::State
    {
    public:
      ObjectMap::Index index;
    };

  private:
    P m_ref;
  };

  template<class P = Shader*, class Qualifier = Type> class objref : public objref_base<P, Qualifier>
  {
  public:
    using objref_base<P, Qualifier>::operator=;
  };

  template<class T, class Qualifier> class objref<boost::shared_ptr<T>, Qualifier> :
    public objref_base<boost::shared_ptr<T>, Qualifier>,
    public types::ObjrefAccessor
  {
  public:
    using objref_base<boost::shared_ptr<T>, Qualifier>::operator=;

    /*virtual*/ void set_generic(boost::shared_ptr<Shader> v);
    /*virtual*/ boost::shared_ptr<Shader> get_generic(void) const;
  };
}

#include "impl/objref_impl.cpp"

#endif /* shade_types_objref_H */
