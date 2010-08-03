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
#include <avango/UnitTest++/UnitTest++.h>
#include <avango/utils/MultiValueField.h>

namespace
{
  using namespace av;
  using namespace av::utils;

  class MultiValueFieldsNode : public av::FieldContainer
  {
    AV_FC_DECLARE();

  public:

    MultiValueFieldsNode();
    virtual ~MultiValueFieldsNode();

    MVFFloat  FloatField;
    MVFDouble DoubleField;
    MVFString StringField;
    MVFBool   BoolField;
    MVFInt    IntField;
    MVFUInt   UIntField;
    MVFLong   LongField;
    MVFULong  ULongField;
    MVFInt    ChangedField;

    /* virtual */ void fieldHasChanged(const Field& field);

    void resetFieldChanged();

    uint getFieldChanged() { return mFieldChanged; }

  private:

    std::vector<float> mFloats;

    uint mFieldChanged;
  };

  AV_FC_DEFINE(MultiValueFieldsNode);

  MultiValueFieldsNode::MultiValueFieldsNode() : mFieldChanged(0)
  {
    AV_FC_ADD_FIELD(FloatField, MVFFloat::ContainerType());
    FloatField.setMultiPush(true);
    AV_FC_ADD_FIELD(DoubleField, MVFDouble::ContainerType());
    DoubleField.setMultiPush(true);
    AV_FC_ADD_FIELD(StringField, MVFString::ContainerType());
    StringField.setMultiPush(true);
    AV_FC_ADD_FIELD(BoolField, MVFBool::ContainerType());
    BoolField.setMultiPush(true);
    AV_FC_ADD_FIELD(IntField, MVFInt::ContainerType());
    IntField.setMultiPush(true);
    AV_FC_ADD_FIELD(UIntField, MVFUInt::ContainerType());
    UIntField.setMultiPush(true);
    AV_FC_ADD_FIELD(LongField, MVFLong::ContainerType());
    LongField.setMultiPush(true);
    AV_FC_ADD_FIELD(ULongField, MVFULong::ContainerType());
    ULongField.setMultiPush(true);
    AV_FC_ADD_FIELD(ChangedField, MVFInt::ContainerType());
    ChangedField.setMultiPush(true);
  }

  MultiValueFieldsNode::~MultiValueFieldsNode()
  {}

  void
  MultiValueFieldsNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::FieldContainer::initClass();

      AV_FC_INIT(av::FieldContainer, MultiValueFieldsNode, false);
    }
  }

  void
  MultiValueFieldsNode::fieldHasChanged(const Field& field)
  {
    av::FieldContainer::fieldHasChanged(field);

    if (&field == &ChangedField)
    {
      mFieldChanged++;
    }
  }

  void
  MultiValueFieldsNode::resetFieldChanged()
  {
    mFieldChanged = false;
  }

  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {

      MultiValueFieldsNode::initClass();
    }
  };

  TEST_FIXTURE(InitNodeFixture, connectSameTypeFields)
  {
    Link<MultiValueFieldsNode> node1(new MultiValueFieldsNode);
    Link<MultiValueFieldsNode> node2(new MultiValueFieldsNode);

    std::vector<float>  values;
    values.push_back(2.0f);
    values.push_back(3.0f);

    node2->FloatField.connectFrom(&(node1->FloatField));
    node1->FloatField.setAllValues(values);

    CHECK_ARRAY_EQUAL(values, node2->FloatField.getAllValues(), values.size());
  }

  TEST_FIXTURE(InitNodeFixture, connectIncompatibleTypeFieldsThrow)
  {
    Link<MultiValueFieldsNode> node1(new MultiValueFieldsNode);
    Link<MultiValueFieldsNode> node2(new MultiValueFieldsNode);

    CHECK_THROW(node2->DoubleField.connectFrom(&(node1->StringField)), std::invalid_argument);
  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsFloatConvert)
  {
    Link<MultiValueFieldsNode> node1(new MultiValueFieldsNode);
    Link<MultiValueFieldsNode> node2(new MultiValueFieldsNode);

    node2->DoubleField.connectFrom(&(node1->FloatField));

    std::vector<float>  values;
    values.push_back(2.0f);
    values.push_back(3.0f);

    node1->FloatField.setAllValues(values);

    CHECK_ARRAY_EQUAL(values, node2->DoubleField.getAllValues(), values.size());
  }

  TEST_FIXTURE(InitNodeFixture, connectDifferentTypeFieldsBool)
  {
    Link<MultiValueFieldsNode> node1(new MultiValueFieldsNode);
    Link<MultiValueFieldsNode> node2(new MultiValueFieldsNode);

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
    node1->FloatField.setAllValues(floatVec);
    CHECK_ARRAY_EQUAL(boolVecTrue, node2->BoolField.getAllValues(), floatVec.size());
    node2->BoolField.disconnect();

    //check int->bool ( true )
    std::vector<int32_t> intVec;
    intVec.push_back(2);
    intVec.push_back(3);
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setAllValues(intVec);
    CHECK_ARRAY_EQUAL(boolVecTrue, node2->BoolField.getAllValues(), intVec.size());
    node2->BoolField.disconnect();

    //check int->bool ( false )
    std::vector<int32_t> intVec2;
    intVec2.push_back(0);
    intVec2.push_back(0);
    node2->BoolField.connectFrom(&(node1->IntField));
    node1->IntField.setAllValues(intVec2);
    CHECK_ARRAY_EQUAL(boolVecFalse, node2->BoolField.getAllValues(), intVec2.size());
    node2->BoolField.disconnect();

  }

  TEST_FIXTURE(InitNodeFixture, setAndGetIndividualValues)
  {
    Link<MultiValueFieldsNode> node(new MultiValueFieldsNode);
    std::vector<int> intVec(4,10);
    node->IntField.setAllValues(intVec);
    CHECK_ARRAY_EQUAL(intVec, node->IntField.getAllValues(), intVec.size());

    int ints[] = {0, -1, -2, -3};
    std::vector<int> intVec2 (ints, ints + sizeof(ints) / sizeof(int));
    node->IntField.set1Value(0,0);
    node->IntField.set1Value(-1,1);
    node->IntField.set1Value(-2,2);
    node->IntField.set1Value(-3,3);
    CHECK_ARRAY_EQUAL(intVec2, node->IntField.getAllValues(), intVec2.size());

    node->IntField.set1Value(-4,2);
    node->IntField.set1Value(-8,3);
    CHECK_EQUAL(0, node->IntField.get1Value(0));
    CHECK_EQUAL(-1, node->IntField.get1Value(1));
    CHECK_EQUAL(-4, node->IntField.get1Value(2));
    CHECK_EQUAL(-8, node->IntField.get1Value(3));

    node->IntField.insert1Value(-2, 2);
    CHECK_EQUAL(0, node->IntField.get1Value(0));
    CHECK_EQUAL(-1, node->IntField.get1Value(1));
    CHECK_EQUAL(-2, node->IntField.get1Value(2));
    CHECK_EQUAL(-4, node->IntField.get1Value(3));
    CHECK_EQUAL(-8, node->IntField.get1Value(4));

    CHECK_THROW(node->IntField.set1Value(0,5), std::out_of_range);
    CHECK_THROW(node->IntField.set1Value(0,100), std::out_of_range);
    CHECK_THROW(node->IntField.set1Value(0,-1), std::out_of_range);

    CHECK_THROW(node->IntField.get1Value(5), std::out_of_range);
    CHECK_THROW(node->IntField.get1Value(77), std::out_of_range);
    CHECK_THROW(node->IntField.get1Value(-4), std::out_of_range);

    CHECK_THROW(node->FloatField.set1Value(0.0,0), std::out_of_range);
    CHECK_THROW(node->DoubleField.get1Value(0), std::out_of_range);

    CHECK_THROW(node->IntField.insert1Value(0,64), std::out_of_range);
    CHECK_THROW(node->FloatField.insert1Value(0.0,1), std::out_of_range);
  }

  TEST_FIXTURE(InitNodeFixture, fieldChangedTriggered)
  {
    Link<MultiValueFieldsNode> node(new MultiValueFieldsNode);
    std::vector<int> intVec(8,0);
    node->ChangedField.setAllValues(intVec);
    CHECK(node->getFieldChanged());

    node->resetFieldChanged();
    CHECK(!node->getFieldChanged());
    node->ChangedField.set1Value(-1, 4);
    CHECK(node->getFieldChanged());

    node->resetFieldChanged();
    CHECK(!node->getFieldChanged());
    node->ChangedField.set1Value(-1, 0);
    CHECK(node->getFieldChanged());
    node->resetFieldChanged();
    node->ChangedField.set1Value(-1, 3);
    CHECK(node->getFieldChanged());

    node->resetFieldChanged();
    CHECK(!node->getFieldChanged());
    node->ChangedField.set1Value(-1, 5, false);
    CHECK(!node->getFieldChanged());
    node->ChangedField.set1Value(-1, 6, false);
    node->ChangedField.set1Value(-1, 7, false);
    node->ChangedField.set1Value(-1, 2, false);
    CHECK(!node->getFieldChanged());
    node->ChangedField.set1Value(-1, 1, true);
    CHECK(node->getFieldChanged());

  }

  TEST_FIXTURE(InitNodeFixture, insertValues)
  {
    Link<MultiValueFieldsNode> node(new MultiValueFieldsNode);
    node->IntField.insert1Value(0,0);
    node->IntField.insert1Value(-1,1);
    node->IntField.insert1Value(-2,2);

    CHECK_EQUAL(0, node->IntField.get1Value(0));
    CHECK_EQUAL(-1, node->IntField.get1Value(1));
    CHECK_EQUAL(-2, node->IntField.get1Value(2));

    CHECK_THROW(node->IntField.insert1Value(-4,4), std::out_of_range);
  }

  TEST_FIXTURE(InitNodeFixture, fieldChangedConnectedFields)
  {
    Link<MultiValueFieldsNode> node0(new MultiValueFieldsNode);
    Link<MultiValueFieldsNode> node1(new MultiValueFieldsNode);

    node1->ChangedField.connectFrom(&(node0->ChangedField));
    std::vector<int> intVec(16, 0);
    node0->ChangedField.setAllValues(intVec);
    CHECK(node0->getFieldChanged());
    CHECK(node1->getFieldChanged());

    node0->resetFieldChanged();
    node1->resetFieldChanged();
    node0->ChangedField.resetChangedFlags();
    node1->ChangedField.resetChangedFlags();

    // a few field values can be set this way, or...
    node0->ChangedField.set1Value(-1, 0, false);
    node0->ChangedField.set1Value(-2, 2, false);
    node0->ChangedField.set1Value(-3, 4, false);
    node0->ChangedField.set1Value(-4, 8);
    CHECK_EQUAL(1u, node0->getFieldChanged());
    CHECK_EQUAL(1u, node1->getFieldChanged());

    node0->resetFieldChanged();
    node1->resetFieldChanged();
    node0->ChangedField.resetChangedFlags();
    node1->ChangedField.resetChangedFlags();

    // ...this way
    std::vector<uint> v_inds;
    v_inds.push_back(0);
    v_inds.push_back(2);
    v_inds.push_back(4);
    v_inds.push_back(8);

    std::vector<int> v;
    v.push_back(-1);
    v.push_back(-2);
    v.push_back(-3);
    v.push_back(-4);

    node0->ChangedField.setSomeValues(v, v_inds);
    CHECK_ARRAY_EQUAL(node0->ChangedField.getAllValues(), node1->ChangedField.getAllValues(), 16);
    CHECK_EQUAL(1u, node0->getFieldChanged());
    CHECK_EQUAL(1u, node1->getFieldChanged());

  }

  TEST_FIXTURE(InitNodeFixture, eraseValues)
  {
    Link<MultiValueFieldsNode> node(new MultiValueFieldsNode);
    CHECK_THROW(node->IntField.erase1Value(0), std::out_of_range);

    std::vector<int> intVec(8, 0);
    node->IntField.setAllValues(intVec);
    node->IntField.resetChangedFlags();
    node->IntField.erase1Value(4);
    for (uint u = 0; u <= 3; u++)
      CHECK(!(node->IntField.valueHasChanged(u)));
    for (uint u = 4; u < 7; u++)
      CHECK(node->IntField.valueHasChanged(u));

  }


} // namespace
