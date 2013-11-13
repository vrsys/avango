// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include "MockFieldContainer.h"
#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  TEST(TypeIdOfFields)
  {
    MockFieldContainer::initClass();

    av::Link<av::FieldContainer> fc =
      dynamic_cast<av::FieldContainer*>(av::Type::createInstanceOfType("MockFieldContainer"));
    CHECK(&*fc != 0 );
    av::Field& self_p = *fc->getField("AnIntField");
    CHECK(&self_p != 0 );
  }

  using namespace av;

  class TestNode : public av::FieldContainer {
    AV_FC_DECLARE();

  public:

    TestNode();
    virtual ~TestNode();

    SFFloat  FloatField;
  };

  AV_FC_DEFINE(TestNode);

  TestNode::TestNode()
  {
    AV_FC_ADD_FIELD(FloatField, 0.0f);
  }

  TestNode::~TestNode()
  {}

  void
  TestNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, TestNode, false);
    }
  }

  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {

      TestNode::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, getFieldWithInvalidArguments)
  {
    Link<TestNode> node(new TestNode);

    CHECK(node->getField("NonExistingField") == 0);
    CHECK(node->getField(15u) == 0);
  }

  TEST_FIXTURE(InitNodeFixture, disconnectAllFieldsWorks)
  {
    Link<TestNode> node1(new TestNode);
    Link<TestNode> node2(new TestNode);

    node2->FloatField.connectFrom(&(node1->FloatField));

    CHECK_EQUAL(1u, node2->FloatField.getNumberOfConnectedFields());
    node2->disconnectAllFields();
    CHECK_EQUAL(0u, node2->FloatField.getNumberOfConnectedFields());
  }
}
