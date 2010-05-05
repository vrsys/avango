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

#ifndef shade_types_list_H
#define shade_types_list_H shade_types_list_H

#include "../TypeBase.h"
#include "ListAccessor.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace shade
{
  template<class Container, class Qualifier = Type> class list_base :
    public TypeBase<list_base<Container, Qualifier>, Qualifier>
  {
  public:
    list_base(void);

    class Accessor
    {
    public:
      Accessor(list_base<Container, Qualifier>& l);
      ~Accessor(void);
      Container& operator*(void);
      const Container& operator*(void) const;
      Container* operator->(void);
      const Container* operator->(void) const;
    private:
      list_base<Container, Qualifier>& m_list;
      bool m_touched;
    };

    const Container* operator->(void) const;

    /*virtual*/ boost::shared_ptr<Type::State> create_state(const PipelineState& ps) const;
    /*virtual*/ void collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)>) const;
    /*virtual*/ void insert_references(boost::shared_ptr<Type::State>, ObjectMap&) const;
    /*virtual*/ std::string get_constructor_str(void) const;
    /*virtual*/ std::string get_constructor_str(boost::shared_ptr<Type::State> state) const;
    /*virtual*/ std::string get_uniq_id(void) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const;
    /*virtual*/ void generate_constructor(formatter::Generator& generator) const;

    class State : public Type::State
    {
    public:
      ObjectMap::Index index;
    };

  protected:
    Container m_container;
  };

  template<class Container, class Qualifier = Type> class list :
    public list_base<Container, Qualifier>
  {
  public:
    using list_base<Container, Qualifier>::operator->;
  };

  template<class T, class Qualifier> class list<std::vector<boost::shared_ptr<T> >, Qualifier> :
    public list_base<std::vector<boost::shared_ptr<T> >, Qualifier>,
    public types::ListAccessor
  {
  public:
    using list_base<std::vector<boost::shared_ptr<T> >, Qualifier>::operator->;
    using list_base<std::vector<boost::shared_ptr<T> >, Qualifier>::m_container;

    /*virtual*/ OutIterator set(void);
    /*virtual*/ Range get(void) const;

  private:
    class ListInIteratorImpl : public InIteratorImpl
    {
    public:
      ListInIteratorImpl(typename std::vector<boost::shared_ptr<T> >::const_iterator iter);
      /*virtual*/ InIteratorImpl* clone(void);
      /*virtual*/ const boost::shared_ptr<Shader> dereference(void) const;
      /*virtual*/ void increment(void);
      /*virtual*/ bool equals(const InIteratorImpl& other) const;
    private:
      typename std::vector<boost::shared_ptr<T> >::const_iterator m_iter;
    };

    class ListOutIteratorImpl : public OutIteratorImpl
    {
    public:
      ListOutIteratorImpl(std::vector<boost::shared_ptr<T> >& container);
      /*virtual*/ OutIteratorImpl* clone(void);
      /*virtual*/ boost::shared_ptr<Shader>& dereference(void);
      /*virtual*/ void increment(void);
    private:
      boost::shared_ptr<Shader> m_value;
      std::vector<boost::shared_ptr<T> >& m_container;
    };
  };

}


#include "impl/list_impl.cpp"

#endif /* shade_types_list_H */

