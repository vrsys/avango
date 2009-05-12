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

// includes, project

#include <avango/Link.h>
#include <avango/Object.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

// variables, exported

// internal unnamed namespace

namespace
{
  using namespace av;

  class FieldsNode : public av::Object {
    AV_FC_DECLARE();

  public:

    FieldsNode();
    virtual ~FieldsNode();

    SFFloat  FloatField;
    SFDouble DoubleField;
    SFString StringField;
    SFBool   BoolField;
    SFInt    IntField;
    SFUInt   UIntField;
    SFLong   LongField;
    SFULong  ULongField;

  private:

    float mFloat;
  };

  AV_FC_DEFINE(FieldsNode);

  FieldsNode::FieldsNode()
    : mFloat(0.0f)
  {
    AV_FC_ADD_FIELD(FloatField, 0.0f);
    FloatField.setMultiPush(true);
    AV_FC_ADD_FIELD(DoubleField, 0.0);
    DoubleField.setMultiPush(true);
    AV_FC_ADD_FIELD(StringField, "defaultString");
    StringField.setMultiPush(true);
    AV_FC_ADD_FIELD(BoolField, false);
    BoolField.setMultiPush(true);
    AV_FC_ADD_FIELD(IntField, 0);
    IntField.setMultiPush(true);
    AV_FC_ADD_FIELD(UIntField, 0);
    UIntField.setMultiPush(true);
    AV_FC_ADD_FIELD(LongField, 0);
    LongField.setMultiPush(true);
    AV_FC_ADD_FIELD(ULongField, 0);
    ULongField.setMultiPush(true);
  }

  FieldsNode::~FieldsNode()
  {}

  void
  FieldsNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();

      AV_FC_INIT(av::Object, FieldsNode, false);
    }
  }

  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {

      FieldsNode::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, connectSameTypeFields)
  {
    Link<FieldsNode> node1(new FieldsNode);
    Link<FieldsNode> node2(new FieldsNode);

    node2->FloatField.connectFrom(&(node1->FloatField));

    node1->FloatField.setValue(2.0f);

    CHECK_EQUAL(node1->FloatField.getValue(), node2->FloatField.getValue());
  }

  TEST_FIXTURE(InitNodeFixture, connectIncompatibleTypeFieldsThrow)
  {
    Link<FieldsNode> node1(new FieldsNode);
    Link<FieldsNode> node2(new FieldsNode);

    CHECK_THROW(node2->DoubleField.connectFrom(&(node1->StringField)), std::invalid_argument);
  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsFloatConvert)
  {
    Link<FieldsNode> node1(new FieldsNode);
    Link<FieldsNode> node2(new FieldsNode);

    node2->DoubleField.connectFrom(&(node1->FloatField));

    node1->FloatField.setValue(2);

    CHECK_EQUAL(node1->FloatField.getValue(), node2->DoubleField.getValue());

  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsBool)
  {
    Link<FieldsNode> node1(new FieldsNode);
    Link<FieldsNode> node2(new FieldsNode);


    // check float->bool (true)
    node2->BoolField.connectFrom(&(node1->FloatField));
    node1->FloatField.setValue(2.0f);
    CHECK_EQUAL(true, node2->BoolField.getValue());
    node2->BoolField.disconnect(); node2->BoolField.setValue(false);

    // check float->bool (false)
    node2->BoolField.connectFrom(&(node1->FloatField));
    node1->FloatField.setValue(0.0f);
    CHECK_EQUAL(false, node2->BoolField.getValue());
    node2->BoolField.disconnect(); node2->BoolField.setValue(false);

    //check int->bool ( true )
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setValue(2);
    CHECK_EQUAL(true, node2->BoolField.getValue());
    node2->BoolField.disconnect(); node2->BoolField.setValue(false);

    //check int->bool ( false )
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setValue(0);
    CHECK_EQUAL(false, node2->BoolField.getValue());
    node2->BoolField.disconnect(); node2->BoolField.setValue(false);

    //check bool->int ( true )
    node2->IntField.connectFrom(&(node1->BoolField));
    node1->BoolField.setValue(true);
    CHECK_EQUAL(1, node2->IntField.getValue());
    node2->IntField.disconnect();

    //check bool->int ( false )
    node2->IntField.connectFrom(&(node1->BoolField));
    node1->BoolField.setValue(false);
    CHECK_EQUAL(0, node2->IntField.getValue());
    node2->IntField.disconnect();

    //check bool->float ( true )
    node2->FloatField.connectFrom(&(node1->BoolField));
    node1->BoolField.setValue(true);
    CHECK_EQUAL(1.0f, node2->FloatField.getValue());
    node2->FloatField.disconnect();

    //check bool->float ( false )
    node2->FloatField.connectFrom(&(node1->BoolField));
    node1->BoolField.setValue(false);
    CHECK_EQUAL(0.0f, node2->FloatField.getValue());
    node2->FloatField.disconnect();
  }

} // namespace

// functions, exported
