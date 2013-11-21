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

#include <vector>

#include <avango/Link.h>
#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  using namespace av;

  class MultiFieldsNode : public av::FieldContainer
  {
    AV_FC_DECLARE();

  public:

    MultiFieldsNode();
    virtual ~MultiFieldsNode();

    MFFloat  FloatField;
    MFDouble DoubleField;
    MFString StringField;
    MFBool   BoolField;
    MFInt    IntField;
    MFUInt   UIntField;
    MFLong   LongField;
    MFULong  ULongField;

  private:

    std::vector<float> mFloats;
  };

  AV_FC_DEFINE(MultiFieldsNode);

  MultiFieldsNode::MultiFieldsNode()
  {
    AV_FC_ADD_FIELD(FloatField, MFFloat::ContainerType());
    FloatField.setMultiPush(true);
    AV_FC_ADD_FIELD(DoubleField, MFDouble::ContainerType());
    DoubleField.setMultiPush(true);
    AV_FC_ADD_FIELD(StringField, MFString::ContainerType());
    StringField.setMultiPush(true);
    AV_FC_ADD_FIELD(BoolField, MFBool::ContainerType());
    BoolField.setMultiPush(true);
    AV_FC_ADD_FIELD(IntField, MFInt::ContainerType());
    IntField.setMultiPush(true);
    AV_FC_ADD_FIELD(UIntField, MFUInt::ContainerType());
    UIntField.setMultiPush(true);
    AV_FC_ADD_FIELD(LongField, MFLong::ContainerType());
    LongField.setMultiPush(true);
    AV_FC_ADD_FIELD(ULongField, MFULong::ContainerType());
    ULongField.setMultiPush(true);
  }

  MultiFieldsNode::~MultiFieldsNode()
  {}

  void
  MultiFieldsNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, MultiFieldsNode, false);
    }
  }

  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {

      MultiFieldsNode::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, connectSameTypeFields)
  {
    Link<MultiFieldsNode> node1(new MultiFieldsNode);
    Link<MultiFieldsNode> node2(new MultiFieldsNode);

    std::vector<float>  values;
    values.push_back(2.0f);
    values.push_back(3.0f);

    node2->FloatField.connectFrom(&(node1->FloatField));
    node1->FloatField.setValue(values);

    CHECK_ARRAY_EQUAL(values, node2->FloatField.getValue(), values.size());
  }

  TEST_FIXTURE(InitNodeFixture, connectIncompatibleTypeFieldsThrow)
  {
    Link<MultiFieldsNode> node1(new MultiFieldsNode);
    Link<MultiFieldsNode> node2(new MultiFieldsNode);

    CHECK_THROW(node2->DoubleField.connectFrom(&(node1->StringField)), std::invalid_argument);
  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsFloatConvert)
  {
    Link<MultiFieldsNode> node1(new MultiFieldsNode);
    Link<MultiFieldsNode> node2(new MultiFieldsNode);

    node2->DoubleField.connectFrom(&(node1->FloatField));

    std::vector<float>  values;
    values.push_back(2.0f);
    values.push_back(3.0f);

    node1->FloatField.setValue(values);

    CHECK_ARRAY_EQUAL(values, node2->DoubleField.getValue(), values.size());
  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsBool)
  {
    Link<MultiFieldsNode> node1(new MultiFieldsNode);
    Link<MultiFieldsNode> node2(new MultiFieldsNode);

    std::vector<bool> boolVecTrue;
    boolVecTrue.push_back(true);
    boolVecTrue.push_back(true);

    std::vector<bool> boolVecFalse;
    boolVecFalse.push_back(false);
    boolVecFalse.push_back(false);


    // check float->bool
    std::vector<float> floatVec;
    floatVec.push_back(2.0f);
    floatVec.push_back(3.0f);
    node2->BoolField.connectFrom(&(node1->FloatField));
    node1->FloatField.setValue(floatVec);
    CHECK_ARRAY_EQUAL(boolVecTrue, node2->BoolField.getValue(), floatVec.size());
    node2->BoolField.disconnect();

    //check int->bool ( true )
    std::vector<int32_t> intVec;
    intVec.push_back(2);
    intVec.push_back(3);
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setValue(intVec);
    CHECK_ARRAY_EQUAL(boolVecTrue, node2->BoolField.getValue(), intVec.size());
    node2->BoolField.disconnect();

    //check int->bool ( false )
    std::vector<int32_t> intVec2;
    intVec2.push_back(0);
    intVec2.push_back(0);
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setValue(intVec2);
    CHECK_ARRAY_EQUAL(boolVecFalse, node2->BoolField.getValue(), intVec2.size());
    node2->BoolField.disconnect();

  }

} // namespace
