// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

// include i/f header

// includes, system

#include <cstdlib>
#include <iostream>

// includes, project

#include <avango/Type.h>

#include <avango/UnitTest++/UnitTest++.h>

// variables, exported

// internal unnamed namespace

namespace
{

  class CleanTypeSystemFixture {
  public:
    CleanTypeSystemFixture() { av::Type::init(); }
    virtual ~CleanTypeSystemFixture() {}
  };

  TEST_FIXTURE(CleanTypeSystemFixture, checkBadType)
  {
    av::Type bad_type = av::Type::badType();
    CHECK_EQUAL(0U, bad_type.getId());
    CHECK_EQUAL("BAD_TYPE", bad_type.getName());
  }

  class HierarchyFixture : public CleanTypeSystemFixture {
  public:
    HierarchyFixture()
      : mParent(av::Type::createAbstractType(av::Type::badType(),
                                             "Parent",
                                             true)),
        mChild1(av::Type::createAbstractType( mParent, "Child1", true)),
        mChild2(av::Type::createAbstractType( mParent, "Child2", true))
    {}

  protected:
    av::Type mParent;
    av::Type mChild1;
    av::Type mChild2;
  };

  TEST_FIXTURE(HierarchyFixture, checkTypesValid)
  {
    CHECK(!mParent.isBad());
    CHECK(!mChild1.isBad());
    CHECK(!mChild2.isBad());
  }

  TEST_FIXTURE(HierarchyFixture, checkIsDerivedFrom)
  {
    CHECK(mChild1.isDerivedFrom(mParent));
    CHECK(mChild2.isDerivedFrom(mParent));
    CHECK(!mChild2.isDerivedFrom(mChild1));
    CHECK(!mChild2.isDerivedFrom(mChild2));
    CHECK(!mParent.isDerivedFrom(mChild1));
  }


  TEST_FIXTURE(HierarchyFixture, checkIsOfType)
  {
    CHECK(mParent.isOfType(mParent));
    CHECK(mChild2.isOfType(mParent));
    CHECK(!mChild2.isOfType(mChild1));
  }

  TEST_FIXTURE(HierarchyFixture, checkAllDerived)
  {
    CHECK_EQUAL(2U, mParent.getAllDerivedFrom().size());
  }

  TEST_FIXTURE(HierarchyFixture, checkIds)
  {
    av::Type a(mParent);
    av::Type b(mParent.getId());

    CHECK_EQUAL(mParent.getId(), a.getId());
    CHECK_EQUAL(mParent, b);
  }

} // namespace

// functions, exported
