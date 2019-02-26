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

#ifndef shade_Shader_H
#define shade_Shader_H shade_Shader_H

#include "Type.h"
#include "MethodAccessor.h"
#include "AttributeAccessor.h"
#include "Formatter.h"
#include "ShaderEnvironment.h"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/function.hpp>
#include <typeinfo>
#include <string>

namespace shade
{
namespace formatter
{
class Generator;
};

class Shader
{
  public:
    virtual ~Shader() {}

    virtual void traverse_hierarchy(boost::function<void(const std::type_info*)> cb) = 0;
    virtual void iterate_methods(boost::function<void(const MethodAccessor&)>) = 0;
    virtual void traverse_attributes(boost::function<void(const Type&, const std::string&)> cb) = 0;
    virtual void iterate_attributes(boost::function<void(const Type&, const std::string&)> cb) = 0;
    virtual void traverse_public_attributes(boost::function<void(Type*, const std::string&)> cb) = 0;
    virtual void iterate_public_attributes(boost::function<void(Type*, const std::string&)> cb) = 0;
    virtual boost::shared_ptr<Shader> convert(const std::type_info* ti) = 0;

    virtual std::string get_class_name(void) const;
    virtual const std::string& get_source_name(void) const;

    std::string get_uniq_id(void) const;

    virtual boost::shared_ptr<Type> call(const MethodAccessor& method);
};

namespace detail
{
class EmptyShader;
template <class Base1, class Base2, class Base3, class Base4>
class BasesShader;
} // namespace detail

template <class Derived, class Base1 = detail::EmptyShader, class Base2 = detail::EmptyShader, class Base3 = detail::EmptyShader, class Base4 = detail::EmptyShader>
class ShaderBase : public detail::BasesShader<Base1, Base2, Base3, Base4>
{
  public:
    /*virtual*/ void traverse_hierarchy(boost::function<void(const std::type_info*)> cb);
    /*virtual*/ void iterate_methods(boost::function<void(const MethodAccessor&)> cb);
    /*virtual*/ void traverse_attributes(boost::function<void(const Type&, const std::string&)> cb);
    /*virtual*/ void iterate_attributes(boost::function<void(const Type&, const std::string&)> cb);
    /*virtual*/ void traverse_public_attributes(boost::function<void(Type*, const std::string&)> cb);
    /*virtual*/ void iterate_public_attributes(boost::function<void(Type*, const std::string&)> cb);
    /*virtual*/ boost::shared_ptr<Shader> convert(const std::type_info* ti);

    /*virtual*/ std::string get_class_name(void) const;
    /*virtual*/ const std::string& get_source_name(void) const;

    template <class T>
    T invoke(const std::string& name);
    template <class T>
    T invoke_inline(const std::string& content);
    template <class T>
    T invoke_inline(boost::function<void(formatter::Generator&)> generator);

  protected:
    struct bases : boost::mpl::push_back<typename detail::BasesShader<Base1, Base2, Base3, Base4>::bases, Derived>::type
    {
    };

    template <class A>
    static MethodAccessor def(A (Derived::*v)(void), std::string name, ShaderEnvironment env = invariant_shader);
    template <class A, class B>
    static MethodAccessor def(A (Derived::*v)(B), std::string name, ShaderEnvironment env = invariant_shader);
    template <class A, class B, class C>
    static MethodAccessor def(A (Derived::*v)(B, C), std::string name, ShaderEnvironment env = invariant_shader);
    template <class A, class B, class C, class D>
    static MethodAccessor def(A (Derived::*v)(B, C, D), std::string name, ShaderEnvironment env = invariant_shader);

    template <class T>
    static AttributeAccessor def(T Derived::*type, std::string name, ShaderEnvironment env = invariant_shader);

    static shade::MethodAccessor methods[];
    static shade::AttributeAccessor attributes[];
    static const std::string source_name;
};

} // namespace shade

#include "impl/Shader_impl.cpp"

#endif /* shade_Shader_H */
