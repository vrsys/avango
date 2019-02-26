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

#include "get_pointer.h"
#include <boost/lexical_cast.hpp>

template <class P, class Qualifier>
boost::shared_ptr<shade::Type::State> shade::objref_base<P, Qualifier>::create_state(const PipelineState& ps) const
{
    boost::shared_ptr<State> result(new State);
    result->index = 0;
    return result;
}

template <class P, class Qualifier>
shade::objref_base<P, Qualifier>::objref_base(void) : TypeBase<objref_base<P, Qualifier>, Qualifier>(formatter::Constants::objref)
{
}

template <class P, class Qualifier>
P shade::objref_base<P, Qualifier>::operator->(void)
{
    return m_ref;
}

template <class P, class Qualifier>
P shade::objref_base<P, Qualifier>::get(void) const
{
    return m_ref;
}

template <class P, class Qualifier>
P& shade::objref_base<P, Qualifier>::operator=(const P& v)
{
    m_ref = v;
    TypeBase<objref_base<P, Qualifier>, Qualifier>::touch();
    return m_ref;
}

template <class P, class Qualifier>
shade::objref_base<P, Qualifier>::operator bool(void)
{
    return get_pointer(m_ref);
}

template <class P, class Qualifier>
void shade::objref_base<P, Qualifier>::collect_references(boost::shared_ptr<Type::State> state, boost::function<void(Shader*)> cb) const
{
    Shader* p = get_pointer(m_ref);
    if(p)
    {
        cb(get_pointer(p));
    }
}

template <class P, class Qualifier>
void shade::objref_base<P, Qualifier>::insert_references(boost::shared_ptr<Type::State> state, ObjectMap& objects) const
{
    Shader* p = get_pointer(m_ref);
    if(p)
    {
        State* objref_state = dynamic_cast<State*>(get_pointer(state));
        objref_state->index = objects.get_next_index();
        objects.add_object(get_pointer(p));
    }
}

template <class P, class Qualifier>
std::string shade::objref_base<P, Qualifier>::get_constructor_str(void) const
{
    return "0";
}

template <class P, class Qualifier>
std::string shade::objref_base<P, Qualifier>::get_constructor_str(boost::shared_ptr<Type::State> state) const
{
    State* objref_state = dynamic_cast<State*>(get_pointer(state));
    return boost::lexical_cast<std::string>(objref_state->index);
}

template <class P, class Qualifier>
std::string shade::objref_base<P, Qualifier>::get_uniq_id(void) const
{
    return join_names(Qualifier::get_uniq_id(), "OBJREF");
}

template <class P, class Qualifier>
void shade::objref_base<P, Qualifier>::generate_constructor(formatter::Generator& generator) const
{
    generator.handle_literal_objref(0);
}

template <class P, class Qualifier>
void shade::objref_base<P, Qualifier>::generate_constructor(formatter::Generator& generator, boost::shared_ptr<Type::State> state) const
{
    State* objref_state = dynamic_cast<State*>(get_pointer(state));
    generator.handle_literal_objref(objref_state->index);
}

template <class T, class Qualifier>
void shade::objref<boost::shared_ptr<T>, Qualifier>::set_generic(boost::shared_ptr<Shader> v)
{
    boost::shared_ptr<T> specific_shader(boost::dynamic_pointer_cast<T>(v));
    if(v && (!specific_shader))
        throw BadCast();

    *this = specific_shader;
}

template <class T, class Qualifier>
boost::shared_ptr<shade::Shader> shade::objref<boost::shared_ptr<T>, Qualifier>::get_generic(void) const
{
    return this->get();
}
