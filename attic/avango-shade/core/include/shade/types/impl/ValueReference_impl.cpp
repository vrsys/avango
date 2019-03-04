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
#include "../../shaders/Value.h"

template <template <class> class T, class Q, class P>
shade::ValueReferenceBase<T, Q, P>::ValueReferenceBase(void) : TypeBase<ValueReferenceBase<T, Q, P>, Type>(T<Q>().get_formatter_type()), m_ref(new shaders::Value<T, Q>)
{
}

template <template <class> class T, class Q, class P>
boost::shared_ptr<shade::Type::State> shade::ValueReferenceBase<T, Q, P>::create_state(const PipelineState& ps) const
{
    return boost::shared_ptr<State>(new State);
}

template <template <class> class T, class Q, class P>
P shade::ValueReferenceBase<T, Q, P>::operator->(void)
{
    return m_ref;
}

template <template <class> class T, class Q, class P>
shade::Shader* shade::ValueReferenceBase<T, Q, P>::operator*(void)
{
    return get_pointer(m_ref);
}

template <template <class> class T, class Q, class P>
P shade::ValueReferenceBase<T, Q, P>::get(void) const
{
    return m_ref;
}

template <template <class> class T, class Q, class P>
P& shade::ValueReferenceBase<T, Q, P>::operator=(const P& v)
{
    m_ref = v;
    TypeBase<ValueReferenceBase<T, Q, P>, Type>::touch();
    return m_ref;
}

template <template <class> class T, class Q, class P>
T<shade::Type> shade::ValueReferenceBase<T, Q, P>::get_value(void)
{
    shaders::Value<T, Q>* value_holder = dynamic_cast<shaders::Value<T, Q>*>(**this);
    if(!value_holder)
        throw NoValueAsReference();

    T<shade::Type> retval;
    retval.copy_value(value_holder->value);
    return retval;
}

template <template <class> class T, class Q, class P>
template <class Q2>
const T<Q2>& shade::ValueReferenceBase<T, Q, P>::set_value(const T<Q2>& arg)
{
    shaders::Value<T, Q>* value_holder = dynamic_cast<shaders::Value<T, Q>*>(**this);
    if(!value_holder)
        throw NoValueAsReference();
    value_holder->value.copy_value(arg);
    return arg;
}

template <template <class> class T, class Q, class P>
void shade::ValueReferenceBase<T, Q, P>::collect_references(boost::shared_ptr<Type::State> state, boost::function<void(Shader*)> cb) const
{
    Shader* p = get_pointer(m_ref);
    if(p)
    {
        cb(get_pointer(p));
    }
}

template <template <class> class T, class Q, class P>
void shade::ValueReferenceBase<T, Q, P>::insert_references(boost::shared_ptr<Type::State> state, ObjectMap& objects) const
{
    Shader* p = get_pointer(m_ref);
    if(p)
    {
        State* st = dynamic_cast<State*>(get_pointer(state));
        st->index = objects.get_next_index();
        objects.add_object(get_pointer(p));
    }
}

template <template <class> class T, class Q, class P>
std::string shade::ValueReferenceBase<T, Q, P>::get_constructor_str(void) const
{
    return T<Q>().get_constructor_str();
}

template <template <class> class T, class Q, class P>
void shade::ValueReferenceBase<T, Q, P>::output_attribute(boost::shared_ptr<Type::State>, Formatter* fmt, const std::string& obj, const std::string& name) const
{
    /* Do nothing */
}

template <template <class> class T, class Q, class P>
bool shade::ValueReferenceBase<T, Q, P>::output_begin_property_dispatcher(Formatter* fmt, const std::string& obj, const std::string name) const
{
    fmt->begin_deferred_property_dispatcher(obj, name, this->get_formatter_type());
    return true;
}

template <template <class> class T, class Q, class P>
void shade::ValueReferenceBase<T, Q, P>::output_insert_property_dispatcher(boost::shared_ptr<Type::State> state, Formatter* fmt, ObjectMap::Index index, const std::string& obj) const
{
    std::string getter_class;
    std::string getter_method;
    shade::shaders::Gettable<T>* getter = dynamic_cast<shade::shaders::Gettable<T>*>(get_pointer(m_ref));
    if(getter != 0)
    {
        getter_class = getter->shade::shaders::Gettable<T>::get_class_name();
        getter_method = "get";
    }

    std::string setter_class;
    std::string setter_method;
    shade::shaders::Settable<T>* setter = dynamic_cast<shade::shaders::Settable<T>*>(get_pointer(m_ref));
    if(setter != 0)
    {
        setter_class = setter->shade::shaders::Settable<T>::get_class_name();
        setter_method = "set";
    }

    State* st = dynamic_cast<State*>(get_pointer(state));
    fmt->insert_deferred_property_dispatcher(index, st->index, getter_class, getter_method, setter_class, setter_method);
}

template <template <class> class T, class Q, class P>
std::string shade::ValueReferenceBase<T, Q, P>::get_uniq_id(void) const
{
    T<Q> type;
    return join_names("ValueReferenceBase", type.get_uniq_id());
}

template <template <class> class T, class Q, class P>
void shade::ValueReferenceBase<T, Q, P>::generate_constructor(formatter::Generator& generator) const
{
    T<Q>().generate_constructor(generator);
}

template <template <class> class T, class Q>
void shade::ValueReference<T, Q, boost::shared_ptr<shade::Shader>>::set_generic(boost::shared_ptr<Shader> v)
{
    *this = v;
}

template <template <class> class T, class Q>
boost::shared_ptr<shade::Shader> shade::ValueReference<T, Q, boost::shared_ptr<shade::Shader>>::get_generic(void) const
{
    return this->get();
}
