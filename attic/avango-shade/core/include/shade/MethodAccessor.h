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

#ifndef shade_MethodAccessor_H
#define shade_MethodAccessor_H shade_MethodAccessor_H

#include "Type.h"
#include "ShaderEnvironment.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/vector.hpp>

namespace shade
{
class Shader;
class MethodArgumentList;
class MethodArgumentIterator;
class MethodCaller;

class MethodAccessor
{
  public:
    MethodAccessor(void);
    MethodAccessor(boost::shared_ptr<MethodArgumentList> args, boost::shared_ptr<MethodCaller> caller, std::string name, ShaderEnvironment env);

    const std::string& get_name(void) const;
    ShaderEnvironment get_env(void) const;

    MethodArgumentIterator type_begin(void) const;
    MethodArgumentIterator type_end(void) const;

    boost::shared_ptr<Type> invoke(Shader* obj) const;

  private:
    boost::shared_ptr<MethodArgumentList> m_args;
    boost::shared_ptr<MethodCaller> m_caller;
    std::string m_name;
    ShaderEnvironment m_env;
};

class MethodArgumentList
{
  public:
    virtual ~MethodArgumentList(void) {}
    virtual int size(void) const = 0;
    virtual Type* create_type(int pos) const = 0;
};

template <class Args>
class MethodArgumentList_impl : public MethodArgumentList
{
  public:
    int size(void) const { return boost::mpl::size<Args>::value; }

    template <int pos, bool>
    struct create_type_impl
    {
        typedef typename boost::mpl::at<Args, boost::mpl::int_<pos>>::type rettype;
        operator Type*(void) { return new rettype; }
    };

    template <int pos>
    struct create_type_impl<pos, false>
    {
        operator Type*(void) { return 0; }
    };

    Type* create_type(int pos) const
    {
        const int size = boost::mpl::size<Args>::value;
        if(pos == 0)
            return create_type_impl<0, (0 < size)>();
        else if(pos == 1)
            return create_type_impl<1, (1 < size)>();
        else if(pos == 2)
            return create_type_impl<2, (2 < size)>();
        else if(pos == 3)
            return create_type_impl<3, (3 < size)>();
        else if(pos == 4)
            return create_type_impl<4, (4 < size)>();
        return 0;
    }
};

class MethodCaller
{
  public:
    virtual ~MethodCaller(void) {}
    virtual Type* invoke(Shader* obj) const = 0;
};

template <class Base, class RetVal>
class MethodCaller_impl0 : public MethodCaller
{
  public:
    MethodCaller_impl0(RetVal (Base::*mem)(void)) : m_mem(mem) {}
    Type* invoke(Shader* obj) const
    {
        RetVal* retval = new RetVal;
        *retval = (dynamic_cast<Base*>(obj)->*m_mem)();
        return retval;
    }

  private:
    RetVal (Base::*m_mem)();
};

template <class Base, class RetVal, class Arg0>
class MethodCaller_impl1 : public MethodCaller
{
  public:
    MethodCaller_impl1(RetVal (Base::*mem)(Arg0)) : m_mem(mem) {}
    Type* invoke(Shader* obj) const
    {
        RetVal* retval = new RetVal;
        *retval = (dynamic_cast<Base*>(obj)->*m_mem)(Arg0());
        return retval;
    }

  private:
    RetVal (Base::*m_mem)(Arg0);
};

template <class Base, class RetVal, class Arg0, class Arg1>
class MethodCaller_impl2 : public MethodCaller
{
  public:
    MethodCaller_impl2(RetVal (Base::*mem)(Arg0, Arg1)) : m_mem(mem) {}
    Type* invoke(Shader* obj) const
    {
        RetVal* retval = new RetVal;
        Base* base = dynamic_cast<Base*>(obj);
        if(base)
            *retval = (base->*m_mem)(Arg0(), Arg1());
        return retval;
    }

  private:
    RetVal (Base::*m_mem)(Arg0, Arg1);
};

template <class Base, class RetVal, class Arg0, class Arg1, class Arg2>
class MethodCaller_impl3 : public MethodCaller
{
  public:
    MethodCaller_impl3(RetVal (Base::*mem)(Arg0, Arg1, Arg2)) : m_mem(mem) {}
    Type* invoke(Shader* obj) const
    {
        RetVal* retval = new RetVal;
        *retval = (dynamic_cast<Base*>(obj)->*m_mem)(Arg0(), Arg1(), Arg2());
        return retval;
    }

  private:
    RetVal (Base::*m_mem)(Arg0, Arg1, Arg2);
};

class MethodArgumentIterator
{
  public:
    MethodArgumentIterator(const MethodArgumentList& args);

    void seek_end(void);
    void check(void);

    MethodArgumentIterator& operator++(void);
    const Type& operator*(void)const;
    Type& operator*(void);
    bool operator==(const MethodArgumentIterator& a);
    bool operator!=(const MethodArgumentIterator& a);

  private:
    const MethodArgumentList& m_args;
    int m_pos;
    boost::shared_ptr<Type> m_type;
};

} // namespace shade

#endif /* shade_MethodAccessor_H */
