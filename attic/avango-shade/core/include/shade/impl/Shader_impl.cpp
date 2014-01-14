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

#include <boost/mpl/for_each.hpp>
#include <iostream>
#include "../formatter/Generator.h"

namespace shade
{
  namespace detail
  {

    class EmptyShader
    {};

    template<class Base1, class Base2, class Base3, class Base4> class BasesShader :
      public Base1, public Base2, public Base3, public Base4
    {
    public:
      // *Remove* ambiguity by declaring methods that will be redefined
      // later on anyway

      /*virtual*/ void traverse_hierarchy(boost::function<void (const std::type_info*)> cb) {}
      /*virtual*/ void iterate_methods(boost::function<void (const MethodAccessor&)> cb) {}
      /*virtual*/ void traverse_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void iterate_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void traverse_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ void iterate_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ boost::shared_ptr<Shader> convert(const std::type_info* ti)
      {
        return boost::shared_ptr<Shader>();
      }

      /*virtual*/ std::string get_class_name(void) const
      {
        return "";
      }

      /*virtual*/ const std::string& get_source_name(void) const
      {
        static std::string result;
        return result;
      }

    private:

      typedef typename boost::mpl::insert_range
      <
        typename Base1::bases,
        typename boost::mpl::end<typename Base1::bases>::type,
        typename Base2::bases
      >::type bases_1_2;

      typedef typename boost::mpl::insert_range
      <
        bases_1_2,
        typename boost::mpl::end<bases_1_2>::type,
        typename Base3::bases
      >::type bases_1_2_3;

    protected:

      typedef typename boost::mpl::insert_range
      <
        bases_1_2_3,
        typename boost::mpl::end<bases_1_2_3>::type,
        typename Base4::bases
      >::type bases;

    };

    template<class Base1, class Base2, class Base3> class BasesShader<Base1, Base2, Base3, EmptyShader> :
      public Base1, public Base2, public Base3
    {
    public:
      // *Remove* ambiguity by declaring methods that will be redefined
      // later on anyway

      /*virtual*/ void traverse_hierarchy(boost::function<void (const std::type_info*)> cb) {}
      /*virtual*/ void iterate_methods(boost::function<void (const MethodAccessor&)> cb) {}
      /*virtual*/ void traverse_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void iterate_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void traverse_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ void iterate_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ boost::shared_ptr<Shader> convert(const std::type_info* ti)
      {
        return boost::shared_ptr<Shader>();
      }

      /*virtual*/ std::string get_class_name(void) const
      {
        return "";
      }

      /*virtual*/ const std::string& get_source_name(void) const
      {
        static std::string result;
        return result;
      }

    private:

      typedef typename boost::mpl::insert_range
      <
        typename Base1::bases,
        typename boost::mpl::end<typename Base1::bases>::type,
        typename Base2::bases
      >::type bases_1_2;

    protected:

      typedef typename boost::mpl::insert_range
      <
        bases_1_2,
        typename boost::mpl::end<bases_1_2>::type,
        typename Base3::bases
      >::type bases;

    };

    template<class Base1, class Base2> class BasesShader<Base1, Base2, EmptyShader, EmptyShader> :
      public Base1, public Base2
    {
    public:
      // *Remove* ambiguity by declaring methods that will be redefined
      // later on anyway

      /*virtual*/ void traverse_hierarchy(boost::function<void (const std::type_info*)> cb) {}
      /*virtual*/ void iterate_methods(boost::function<void (const MethodAccessor&)> cb) {}
      /*virtual*/ void traverse_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void iterate_attributes(boost::function<void (const Type&, const std::string&)> cb) {}
      /*virtual*/ void traverse_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ void iterate_public_attributes(boost::function<void (Type*, const std::string&)> cb) {}
      /*virtual*/ boost::shared_ptr<Shader> convert(const std::type_info* ti)
      {
        return boost::shared_ptr<Shader>();
      }

      /*virtual*/ std::string get_class_name(void) const
      {
        return "";
      }

      /*virtual*/ const std::string& get_source_name(void) const
      {
        static std::string result;
        return result;
      }

    protected:

      typedef typename boost::mpl::insert_range
      <
        typename Base1::bases,
        typename boost::mpl::end<typename Base1::bases>::type,
        typename Base2::bases
      >::type bases;

    };

    template<class Base1> class BasesShader<Base1, EmptyShader, EmptyShader, EmptyShader> :
      public Base1
    {
    protected:
      typedef typename Base1::bases bases;
    };

    template<> class BasesShader<EmptyShader, EmptyShader, EmptyShader, EmptyShader> :
      public virtual Shader
    {
    protected:
      struct bases : boost::mpl::vector<>
      {};
    };


    template<class T> struct wrap {};

    class traverse_hierarchy_callback
    {
    public:
      traverse_hierarchy_callback(boost::function<void (const std::type_info*)> cb) :
        m_cb(cb)
      {}
      template<class T> void operator()(wrap<T> wt) const
      {
        m_cb(&typeid(wt));
      }
    private:
      boost::function<void (const std::type_info*)> m_cb;
    };

    class traverse_attributes_callback
    {
    public:
      traverse_attributes_callback(Shader* sb, boost::function<void (const Type&, const std::string&)> cb) :
        m_sb(sb), m_cb(cb)
      {}
      template<class T> void operator()(wrap<T> wt) const
      {
        dynamic_cast<T*>(m_sb)->T::iterate_attributes(m_cb);
      }
    private:
      Shader* m_sb;
      boost::function<void (const Type&, const std::string&)> m_cb;
    };

    class traverse_public_attributes_callback
    {
    public:
      traverse_public_attributes_callback(Shader* sb, boost::function<void (Type*, const std::string&)> cb) :
        m_sb(sb), m_cb(cb)
      {}
      template<class T> void operator()(wrap<T> wt) const
      {
        dynamic_cast<T*>(m_sb)->T::iterate_public_attributes(m_cb);
      }
    private:
      Shader* m_sb;
      boost::function<void (Type*, const std::string&)> m_cb;
    };


    template<class T> class ShaderWrapper : public shade::Shader
    {
    public:
      ShaderWrapper(Shader* obj) :
        m_obj(obj)
      {
      }

      /*virtual*/ void traverse_hierarchy(boost::function<void (const std::type_info*)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::traverse_hierarchy(cb);
      }

      /*virtual*/ void iterate_methods(boost::function<void (const shade::MethodAccessor&)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::iterate_methods(cb);
      }

      /*virtual*/ void traverse_attributes(boost::function<void (const shade::Type&, const std::string&)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::traverse_attributes(cb);
      }

      /*virtual*/ void iterate_attributes(boost::function<void (const shade::Type&, const std::string&)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::iterate_attributes(cb);
      }

      /*virtual*/ void traverse_public_attributes(boost::function<void (Type*, const std::string&)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::traverse_public_attributes(cb);
      }

      /*virtual*/ void iterate_public_attributes(boost::function<void (Type*, const std::string&)> cb)
      {
        dynamic_cast<T*>(m_obj)->T::iterate_public_attributes(cb);
      }

      /*virtual*/ std::string get_class_name(void) const
      {
        return dynamic_cast<T*>(m_obj)->T::get_class_name();
      }

      /*virtual*/ const std::string& get_source_name(void) const
      {
        return dynamic_cast<T*>(m_obj)->T::get_source_name();
      }

      /*virtual*/ boost::shared_ptr<Shader> convert(const std::type_info* ti)
      {
        return dynamic_cast<T*>(m_obj)->T::convert(ti);
      }

      /*virtual*/ boost::shared_ptr<Type> call(const MethodAccessor& method)
      {
        return method.invoke(dynamic_cast<T*>(m_obj));
      }

    private:
      Shader* m_obj;
    };


    class convert_callback
    {
    public:
      convert_callback(Shader* sb, const std::type_info* ti) :
        m_sb(sb), m_ti(ti)
      {}
      template<class T> void operator()(wrap<T> wt)
      {
        if (typeid(wt) == *m_ti)
          m_newobj = boost::shared_ptr<Shader>(new ShaderWrapper<T>(m_sb));
      }
      boost::shared_ptr<Shader> get_object(void)
      {
        return m_newobj;
      }
    private:
      Shader* m_sb;
      boost::shared_ptr<Shader> m_newobj;
      const std::type_info* m_ti;
    };

    class GenerateVerbatim
    {
    public:
      GenerateVerbatim(const std::string& content) :
        m_content(content)
      {
      }

      void operator()(formatter::Generator& generator)
      {
        generator.handle_verbatim(m_content);
      }

    private:
      std::string m_content;
    };

  }
}


template<class Derived, class Base1, class Base2, class Base3, class Base4>
shade::MethodAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::methods[] =
{
  shade::MethodAccessor()
};

template<class Derived, class Base1, class Base2, class Base3, class Base4>
shade::AttributeAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::attributes[] =
{
  shade::AttributeAccessor()
};


template<class Derived, class Base1, class Base2, class Base3, class Base4>
const std::string shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::source_name;

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::traverse_hierarchy(boost::function<void (const std::type_info*)> cb)
{
  detail::traverse_hierarchy_callback thc(cb);
  boost::mpl::for_each<bases, detail::wrap<boost::mpl::_1> >(thc);
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::iterate_methods(boost::function<void (const MethodAccessor&)> cb)
{
  Derived* self = static_cast<Derived*>(this);
  for (int i = 0; !self->methods[i].get_name().empty(); ++i)
    cb(self->methods[i]);
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::traverse_attributes(boost::function<void (const Type&, const std::string&)> cb)
{
  detail::traverse_attributes_callback tac(this, cb);
  boost::mpl::for_each<bases, detail::wrap<boost::mpl::_1> >(tac);
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::iterate_attributes(boost::function<void (const Type&, const std::string&)> cb)
{
  Derived* self = static_cast<Derived*>(this);
  for (int i = 0; !self->attributes[i].get_name().empty(); ++i)
    cb(self->attributes[i].get_type(this), self->attributes[i].get_name());
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::traverse_public_attributes(boost::function<void (Type*, const std::string&)> cb)
{
  detail::traverse_public_attributes_callback tpac(this, cb);
  boost::mpl::for_each<bases, detail::wrap<boost::mpl::_1> >(tpac);
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
void shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::iterate_public_attributes(boost::function<void (Type*, const std::string&)> cb)
{
  Derived* self = static_cast<Derived*>(this);
  for (int i = 0; !self->attributes[i].get_name().empty(); ++i)
  {
    Type* t = self->attributes[i].get_public_type(this);
    if (!t)
      continue;

    cb(t, self->attributes[i].get_name());
  }
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
std::string shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::get_class_name(void) const
{
  return Derived::class_name;
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
const std::string& shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::get_source_name(void) const
{
  return Derived::source_name;
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template<class T>
T shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::invoke(const std::string& name)
{
  T v;
  v.invoke(name, &typeid(detail::wrap<Derived>));
  return v;
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template<class T>
T shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::invoke_inline(boost::function<void (formatter::Generator&)> generator)
{
  T v;
  v.invoke_inline(generator, &typeid(detail::wrap<Derived>));
  return v;
}

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template<class T>
T shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::invoke_inline(const std::string& content)
{
  T v;
  v.invoke_inline(detail::GenerateVerbatim(content), &typeid(detail::wrap<Derived>));
  return v;
}


template<class Derived, class Base1, class Base2, class Base3, class Base4>
boost::shared_ptr<shade::Shader> shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::convert(const std::type_info* ti)
{
  detail::convert_callback cc(this, ti);
  boost::mpl::for_each<bases, detail::wrap<boost::mpl::_1> >(boost::ref(cc));
  return cc.get_object();
}



template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template <class A>
shade::MethodAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::def(A (Derived::*v)(void), std::string name, ShaderEnvironment env)
{
  boost::shared_ptr<MethodArgumentList> args = 
    boost::shared_ptr<MethodArgumentList>(new MethodArgumentList_impl< boost::mpl::vector1<A> >);
  boost::shared_ptr<MethodCaller> caller = 
    boost::shared_ptr<MethodCaller>(new MethodCaller_impl0<Derived, A>(v));
  return MethodAccessor(args, caller, name, env);
};

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template <class A, class B>
shade::MethodAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::def(A (Derived::*v)(B), std::string name, ShaderEnvironment env)
{
  boost::shared_ptr<MethodArgumentList> args = 
    boost::shared_ptr<MethodArgumentList>(new MethodArgumentList_impl< boost::mpl::vector2<A, B> >);
  boost::shared_ptr<MethodCaller> caller = 
    boost::shared_ptr<MethodCaller>(new MethodCaller_impl1<Derived, A, B>(v));
  return MethodAccessor(args, caller, name, env);
};

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template <class A, class B, class C>
shade::MethodAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::def(A (Derived::*v)(B, C), std::string name, ShaderEnvironment env)
{
  boost::shared_ptr<MethodArgumentList> args = 
    boost::shared_ptr<MethodArgumentList>(new MethodArgumentList_impl< boost::mpl::vector3<A, B, C> >);
  boost::shared_ptr<MethodCaller> caller = 
    boost::shared_ptr<MethodCaller>(new MethodCaller_impl2<Derived, A, B, C>(v));
  return MethodAccessor(args, caller, name, env);
};

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template <class A, class B, class C, class D>
shade::MethodAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::def(A (Derived::*v)(B, C, D), std::string name, ShaderEnvironment env)
{
  boost::shared_ptr<MethodArgumentList> args = 
    boost::shared_ptr<MethodArgumentList>(new MethodArgumentList_impl< boost::mpl::vector4<A, B, C, D> >);
  boost::shared_ptr<MethodCaller> caller = 
    boost::shared_ptr<MethodCaller>(new MethodCaller_impl3<Derived, A, B, C, D>(v));
  return MethodAccessor(args, caller, name, env);
};

template<class Derived, class Base1, class Base2, class Base3, class Base4>
  template <class T>
shade::AttributeAccessor shade::ShaderBase<Derived, Base1, Base2, Base3, Base4>::def(T Derived::* type, std::string name, ShaderEnvironment env)
{
  boost::shared_ptr<AttributeType> att_type = 
    boost::shared_ptr<AttributeType>(new AttributeType_impl<Derived, T>(type, env == application_stage));
  return AttributeAccessor(att_type, name);
}
