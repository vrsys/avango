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

template<class Container, class Qualifier> shade::list_base<Container, Qualifier>::list_base(void) :
  TypeBase<list_base<Container, Qualifier>, Qualifier>(formatter::Constants::list)
{
}

template<class Container, class Qualifier>
boost::shared_ptr<shade::Type::State> shade::list_base<Container, Qualifier>::create_state(const PipelineState& ps) const
{
  return boost::shared_ptr<State>(new State);
}

template<class Container, class Qualifier>
const Container* shade::list_base<Container, Qualifier>::operator->(void) const
{
  return &m_container;
}

template<class Container, class Qualifier>
void shade::list_base<Container, Qualifier>::collect_references(boost::shared_ptr<Type::State> state, boost::function<void (Shader*)> cb) const
{
  for (typename Container::const_iterator i = m_container.begin(); i != m_container.end(); ++i)
  {
    cb(get_pointer(*i));
  }
}

template<class Container, class Qualifier>
void shade::list_base<Container, Qualifier>::insert_references(boost::shared_ptr<Type::State> state, ObjectMap& objects) const
{
  State* objref_state = dynamic_cast<State*>(get_pointer(state));
  objref_state->index = objects.get_next_index();

  for (typename Container::const_iterator i = m_container.begin(); i != m_container.end(); ++i)
  {
    objects.add_object(get_pointer(*i));
  }
}

template<class Container, class Qualifier>
std::string shade::list_base<Container, Qualifier>::get_constructor_str(void) const
{
  return "0, 0";
}

template<class Container, class Qualifier>
std::string shade::list_base<Container, Qualifier>::get_constructor_str(boost::shared_ptr<Type::State> state) const
{
  State* objref_state = dynamic_cast<State*>(get_pointer(state));

  std::ostringstream out;
  if (m_container.empty())
    out << "0, 0";
  else
    out << objref_state->index << ", " << objref_state->index+m_container.size();
  return out.str();
}


template<class Container, class Qualifier>
shade::list_base<Container, Qualifier>::Accessor::Accessor(list_base<Container, Qualifier>& l) :
  m_list(l),
  m_touched(false)
{
}

template<class Container, class Qualifier>
shade::list_base<Container, Qualifier>::Accessor::~Accessor(void)
{
  if (m_touched)
    m_list.TypeBase<list_base<Container, Qualifier>, Qualifier>::touch();
}

template<class Container, class Qualifier>
Container& shade::list_base<Container, Qualifier>::Accessor::operator*(void)
{
  m_touched = true;
  return m_list.m_container;
}

template<class Container, class Qualifier>
const Container& shade::list_base<Container, Qualifier>::Accessor::operator*(void) const
{
  return m_list.m_container;
}

  template<class Container, class Qualifier>
Container* shade::list_base<Container, Qualifier>::Accessor::operator->(void)
{
  m_touched = true;
  return &m_list.m_container;
}

template<class Container, class Qualifier>
const Container* shade::list_base<Container, Qualifier>::Accessor::operator->(void) const
{
  return &m_list.m_container;
}


template<class Container, class Qualifier>
std::string shade::list_base<Container, Qualifier>::get_uniq_id(void) const
{
  return join_names(Qualifier::get_uniq_id(), "LIST");
}

template<class Container, class Qualifier>
void shade::list_base<Container, Qualifier>::generate_constructor(formatter::Generator& generator) const
{
  generator.handle_literal_list(0, 0);
}

template<class Container, class Qualifier>
void shade::list_base<Container, Qualifier>::generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const
{
  State* list_state = dynamic_cast<State*>(get_pointer(state));
  if (!list_state)
    return;

  generator.handle_literal_list(list_state->index, list_state->index+m_container.size());
}


template<class T, class Qualifier>
shade::types::ListAccessor::OutIterator shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::set(void)
{
  return OutIterator(new ListOutIteratorImpl(m_container));
}

template<class T, class Qualifier>
shade::types::ListAccessor::Range shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::get(void) const
{
  return std::make_pair(new ListInIteratorImpl(m_container.begin()), new ListInIteratorImpl(m_container.end()));
}


template<class T, class Qualifier>
shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListInIteratorImpl::ListInIteratorImpl(typename std::vector<boost::shared_ptr<T> >::const_iterator iter) :
  m_iter(iter)
{
}

template<class T, class Qualifier>
shade::types::ListAccessor::InIteratorImpl* shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListInIteratorImpl::clone(void)
{
  return new ListInIteratorImpl(m_iter);
}

template<class T, class Qualifier>
const boost::shared_ptr<shade::Shader> shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListInIteratorImpl::dereference(void) const
{
  return *m_iter;
}

template<class T, class Qualifier>
void shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListInIteratorImpl::increment(void)
{
  ++m_iter;
}

template<class T, class Qualifier>
bool shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListInIteratorImpl::equals(const InIteratorImpl& other) const
{
  const ListInIteratorImpl* other_list_iter(dynamic_cast<const ListInIteratorImpl*>(&other));
  if (!other_list_iter)
    return false;

  return m_iter == other_list_iter->m_iter;
}


template<class T, class Qualifier>
shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListOutIteratorImpl::ListOutIteratorImpl(std::vector<boost::shared_ptr<T> >& container) :
  m_container(container)
{
}

template<class T, class Qualifier>
shade::types::ListAccessor::OutIteratorImpl* shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListOutIteratorImpl::clone(void)
{
  return new ListOutIteratorImpl(m_container);
}

template<class T, class Qualifier>
boost::shared_ptr<shade::Shader>& shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListOutIteratorImpl::dereference(void)
{
  return m_value;
}

template<class T, class Qualifier>
void shade::list<std::vector<boost::shared_ptr<T> >, Qualifier>::ListOutIteratorImpl::increment(void)
{
  boost::shared_ptr<T> T_value(boost::dynamic_pointer_cast<T>(m_value));
  if (!T_value)
    throw BadCast();

  m_container.push_back(T_value);
}
