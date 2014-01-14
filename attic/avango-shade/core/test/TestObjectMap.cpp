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

#include <shade/ObjectMap.h>
#include <shade/Shader.h>
#include <shade/utility.h>
#include <boost/shared_ptr.hpp>
#include <UnitTest++.h>

namespace
{
  class Simple : public shade::ShaderBase<Simple>
  {
  private:
    SHADE_BASE_DECL(Simple)
  };

  SHADE_CLASS_INIT(Simple, "<undefined>", SHADE_NONE, SHADE_NONE)
}


SUITE(ObjectMap)
{
  TEST(EmptyMap)
  {
    shade::ObjectMap map;
    CHECK_EQUAL(0u, map.get_next_index());
  }

  TEST(InsertObjects)
  {
    shade::ObjectMap map;
    CHECK_EQUAL(0u, map.get_next_index());

    boost::shared_ptr<Simple> a = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(a));
    CHECK_EQUAL(1u, map.get_next_index());

    boost::shared_ptr<Simple> b = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(b));
    CHECK_EQUAL(2u, map.get_next_index());
  }

  TEST(GetObject)
  {
    shade::ObjectMap map;

    boost::shared_ptr<Simple> a = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(a));
    CHECK_EQUAL(get_pointer(a), map.get_object(0));

    boost::shared_ptr<Simple> b = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(b));
    CHECK_EQUAL(get_pointer(b), map.get_object(1));

    CHECK_EQUAL(get_pointer(a), map[0]);
    CHECK_EQUAL(get_pointer(b), map[1]);
  }

  TEST(GetIndex)
  {
    shade::ObjectMap map;

    boost::shared_ptr<Simple> a = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(a));
    CHECK_EQUAL(0u, map.get_index(get_pointer(a)));

    boost::shared_ptr<Simple> b = boost::shared_ptr<Simple>(new Simple);
    map.add_object(get_pointer(b));
    CHECK_EQUAL(1u, map.get_index(get_pointer(b)));
  }
}
