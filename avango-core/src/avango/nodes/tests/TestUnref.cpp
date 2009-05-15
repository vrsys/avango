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

// $Id$

// include i/f header

// includes, system

// includes, project

#include <avango/Link.h>
#include <avango/Object.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

// variables, exported

// internal unnamed namespace

namespace {
  using namespace av;

  class Lifetime : public av::Object {
    AV_FC_DECLARE();

  public:

    Lifetime();
    virtual ~Lifetime();

    static bool is_living;
  };

  bool Lifetime::is_living = false;

  AV_FC_DEFINE(Lifetime);

  Lifetime::Lifetime()
  {
    is_living = true;
  }

  Lifetime::~Lifetime()
  {
    is_living = false;
  }

  void
  Lifetime::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();

      AV_FC_INIT(av::Object, Lifetime, true);
    }
  }

  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {
      Lifetime::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, UsingLinks)
  {
    CHECK(!Lifetime::is_living);
    Link<Lifetime> node(dynamic_cast<Lifetime*>(av::Type::createInstanceOfType("Lifetime")));
    CHECK(node.isValid());
    CHECK(Lifetime::is_living);
  }

  TEST_FIXTURE(InitNodeFixture, ManualUnrefing)
  {
    CHECK(!Lifetime::is_living);
    Lifetime* node(dynamic_cast<Lifetime*>(av::Type::createInstanceOfType("Lifetime")));
    CHECK(0 != node);
    CHECK(Lifetime::is_living);
    node->reference();
    node->unreference();
    CHECK(!Lifetime::is_living);
  }

  TEST_FIXTURE(InitNodeFixture, ManualUnrefingWithoutDeletion)
  {
    CHECK(!Lifetime::is_living);
    Lifetime* node(dynamic_cast<Lifetime*>(av::Type::createInstanceOfType("Lifetime")));
    CHECK(0 != node);
    CHECK(Lifetime::is_living);
    node->reference();

    node->unreferenceWithoutDeletion();
    CHECK(Lifetime::is_living);

    node->reference();
    node->unreference();
    CHECK(!Lifetime::is_living);
  }

  TEST_FIXTURE(InitNodeFixture, ManualUnrefingWithFloatingRefs)
  {
    CHECK(!Lifetime::is_living);
    Lifetime* node(dynamic_cast<Lifetime*>(av::Type::createInstanceOfType("Lifetime")));
    CHECK(0 != node);
    CHECK(Lifetime::is_living);

    node->reference();
    node->setFloatingReference();
    node->unreference();
    CHECK(Lifetime::is_living);

    node->reference();
    node->unreference();
    CHECK(!Lifetime::is_living);
  }


} // namespace

// functions, exported

// $Id$
