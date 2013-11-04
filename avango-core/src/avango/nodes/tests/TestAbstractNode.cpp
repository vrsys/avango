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

// include i/f header

// includes, system

// includes, project

#include <avango/Link.h>
#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

// variables, exported

// internal unnamed namespace

namespace {
  using namespace av;

  class AbstractNode : public av::FieldContainer {
    AV_FC_DECLARE_ABSTRACT();

  public:

    AbstractNode();
    virtual ~AbstractNode();

    MFFloat  FloatField;

    virtual void pureVirtualFunction() = 0;

  };

  AV_FC_DEFINE_ABSTRACT(AbstractNode);

  AbstractNode::AbstractNode()
  {
    AV_FC_ADD_FIELD(FloatField, MFFloat::ContainerType());
  }

  AbstractNode::~AbstractNode()
  {}

  void
  AbstractNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::FieldContainer::initClass();

      AV_FC_INIT_ABSTRACT(av::FieldContainer, AbstractNode, false);
    }
  }

  class ConcreteNode : public AbstractNode {
    AV_FC_DECLARE();

  public:

    ConcreteNode();
    virtual ~ConcreteNode();

    MFFloat  FloatField2;

    void pureVirtualFunction();


  };

  AV_FC_DEFINE(ConcreteNode);

  ConcreteNode::ConcreteNode()
  {
    AV_FC_ADD_FIELD(FloatField2, MFFloat::ContainerType());
  }

  ConcreteNode::~ConcreteNode()
  {}

  void
  ConcreteNode::pureVirtualFunction()
  {}

  void
  ConcreteNode::initClass()
  {
    if (!isTypeInitialized())
    {
      AbstractNode::initClass();

      AV_FC_INIT(AbstractNode, ConcreteNode, false);
    }
  }


  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {

      ConcreteNode::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, createConcreteNode)
  {
    Link<ConcreteNode> node1(dynamic_cast<ConcreteNode*>(av::Type::createInstanceOfType("ConcreteNode")));

    CHECK(node1.isValid());

  }

} // namespace

// functions, exported
