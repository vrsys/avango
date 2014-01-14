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

#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>

namespace
{
  class Base
  {
  public:
    explicit Base(int i) {}
    virtual ~Base(void) {};
  private:
    Base(const Base&);
  };

  class Derived : public Base
  {
  public:
    explicit Derived(int i) : Base(i) {}
  };
}


TEST(UnequalTypeid)
{
  Base b(1);
  Derived d(1);
  CHECK( typeid(b)!=typeid(d) );
}

TEST(UnequalDereferencedTypeid)
{
  boost::shared_ptr<Base> b(new Base(1));
  boost::shared_ptr<Derived> d(new Derived(1));
  CHECK( typeid(*b)!=typeid(*d) );
}

TEST(UnequalPointerTypeid)
{
  Base b(1);
  Derived d(1);
  CHECK( typeid(&b)!=typeid(&d) );
}

TEST(BaseClassTypeid)
{
  boost::shared_ptr<Base> b(new Base(1));
  boost::shared_ptr<Derived> d(new Derived(1));
  CHECK( typeid(&*b) == typeid(static_cast<Base*>(&*d)) );
}


namespace
{
  template<class T> T* get_pointer(T* v)
  {
    return v;
  }
  Base* get_pointer(Base& v)
  {
    return &v;
  }
}

TEST(get_pointer)
{
  Base* b = new Base(1);
  Base& rb(*b);
  boost::shared_ptr<Base> sb(b);
  CHECK_EQUAL(b, get_pointer(b));  // Template function
  CHECK_EQUAL(b, get_pointer(*b)); // Specific function
  CHECK_EQUAL(b, get_pointer(rb)); // Specific function
  CHECK_EQUAL(b, get_pointer(sb)); // ADL
}
