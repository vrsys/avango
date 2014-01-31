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

namespace
{
  using namespace av;

  class FieldNodeMultiPush : public av::FieldContainer {
    AV_FC_DECLARE();

  public:

    FieldNodeMultiPush();
    virtual ~FieldNodeMultiPush();

    SFInt    IntField;
    SFInt    OutField;

    void fieldHasChanged(const av::Field& field);

  private:
    unsigned int mTriggered;

  };

  AV_FC_DEFINE(FieldNodeMultiPush);

  FieldNodeMultiPush::FieldNodeMultiPush()
    : mTriggered(0)
  {
    AV_FC_ADD_FIELD(IntField, 0);
    AV_FC_ADD_FIELD(OutField, 0);
  }

  FieldNodeMultiPush::~FieldNodeMultiPush()
  {}

  void
  FieldNodeMultiPush::initClass()
  {
    if (!isTypeInitialized())
    {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, FieldNodeMultiPush, false);
    }
  }

  void
  FieldNodeMultiPush::fieldHasChanged(const av::Field& field)
  {
    if (&field == &IntField)
    {
//       mTriggered++;
//       if (mTriggered < 2)
//       {
        OutField.setValue(3);
//       }
    }

  }


  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {
      FieldNodeMultiPush::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, defaultIsMultiPush)
  {
    Link<FieldNodeMultiPush> node1(new FieldNodeMultiPush);
    Link<FieldNodeMultiPush> node2(new FieldNodeMultiPush);

    node2->IntField.connectFrom(&(node1->IntField));

    node1->IntField.setValue(2);
    CHECK_EQUAL(2, node2->IntField.getValue());

    node1->IntField.setValue(3);
    CHECK_EQUAL(3, node2->IntField.getValue());

  }

  TEST_FIXTURE(InitNodeFixture, throwIfFieldIsTriggeredTwice)
  {
    Link<FieldNodeMultiPush> node1(new FieldNodeMultiPush);
    Link<FieldNodeMultiPush> node2(new FieldNodeMultiPush);

    node2->IntField.connectFrom(&(node1->OutField));
    CHECK_THROW(node1->IntField.connectFrom(&(node2->IntField)), std::runtime_error);

    // CHECK_THROW(node1->IntField.setValue(2), std::runtime_error);

  }

} // namespace

// functions, exported
