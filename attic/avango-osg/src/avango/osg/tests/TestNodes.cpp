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

#include <vector>

#include <avango/osg/Init.h>

#include <avango/osg/ObjectLink.h>
#include <avango/osg/Group.h>
#include <avango/osg/MatrixTransform.h>
#include <avango/osg/StateSet.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/osg/network/NetMatrixTransform.h>
#endif

#include <avango/UnitTest++/UnitTest++.h>


namespace
{
  class InitNodeFixture
  {
  public:
    InitNodeFixture()
    {
      av::osg::Init::initClass();
    }
  };

  class TestOsgGroup : public ::osg::Group
  {
  public:
    TestOsgGroup()  {++sNumTestGroups;}
    ~TestOsgGroup() {--sNumTestGroups;}
    static int sNumTestGroups;
  };

  int TestOsgGroup::sNumTestGroups = 0;

  class TestAvGroup : public av::osg::Group
  {
  public:
    TestAvGroup() : av::osg::Group(new TestOsgGroup) {++sNumTestGroups;}
    ~TestAvGroup() {--sNumTestGroups;}
    static int sNumTestGroups;
  };

  int TestAvGroup::sNumTestGroups = 0;

  TEST_FIXTURE(InitNodeFixture, testRefCount)
  {
    // initial checks
    CHECK(::osg::Referenced::getDeleteHandler() == 0);
    CHECK_EQUAL(0, TestOsgGroup::sNumTestGroups);
    CHECK_EQUAL(0, TestAvGroup::sNumTestGroups);

    // check if avango and osg reference count is synchronous
    // and check for correct deletion after unreference from av::Link
    {
      av::Link<TestAvGroup> test_group = new TestAvGroup;
      CHECK_EQUAL(1, TestOsgGroup::sNumTestGroups);
      CHECK_EQUAL(1, TestAvGroup::sNumTestGroups);
      CHECK_EQUAL(1, test_group->referenceCount());
      CHECK_EQUAL(1, test_group->getOsgGroup()->referenceCount());
      test_group->reference();
      CHECK_EQUAL(2, test_group->referenceCount());
      CHECK_EQUAL(2, test_group->getOsgGroup()->referenceCount());
      test_group->getOsgGroup()->ref();
      CHECK_EQUAL(3, test_group->referenceCount());
      CHECK_EQUAL(3, test_group->getOsgGroup()->referenceCount());
      test_group->unreference();
      CHECK_EQUAL(2, test_group->referenceCount());
      CHECK_EQUAL(2, test_group->getOsgGroup()->referenceCount());
      test_group->getOsgGroup()->unref();
      CHECK_EQUAL(1, test_group->referenceCount());
      CHECK_EQUAL(1, test_group->getOsgGroup()->referenceCount());
    }
    CHECK_EQUAL(0, TestOsgGroup::sNumTestGroups);
    CHECK_EQUAL(0, TestAvGroup::sNumTestGroups);

    // check for correct deletion after unreference from osg
    av::Link<av::osg::Group> root = new av::osg::Group;
    {
      av::Link<TestAvGroup> test_group = new TestAvGroup;
      root->Children.add1Value(test_group);
      CHECK_EQUAL(1, TestOsgGroup::sNumTestGroups);
      CHECK_EQUAL(1, TestAvGroup::sNumTestGroups);
    }
    CHECK_EQUAL(1, TestOsgGroup::sNumTestGroups);
    CHECK_EQUAL(1, TestAvGroup::sNumTestGroups);
    CHECK_EQUAL(1, root->Children.getSize());
    root->Children.clear();
    CHECK_EQUAL(0, TestOsgGroup::sNumTestGroups);
    CHECK_EQUAL(0, TestAvGroup::sNumTestGroups);
  }

  TEST_FIXTURE(InitNodeFixture, testGroupNodes)
  {
    // Avango group node with two group nodes as children
    av::Link<av::osg::Group> av_g_root   = new av::osg::Group;
    av::Link<av::osg::Group> av_g_child1 = new av::osg::Group;
    av::Link<av::osg::Group> av_g_child2 = new av::osg::Group;

    CHECK(av_g_root.isValid());
    CHECK(av_g_child1.isValid());
    CHECK(av_g_child2.isValid());

    av_g_root->Children.add1Value(av_g_child1.getPtr());
    av_g_root->Children.add1Value(av_g_child2.getPtr());

    // check Avango structure
    std::vector<av::Link<av::osg::Node> > av_g_root_children = av_g_root->Children.getValue();

    CHECK_EQUAL(2u, av_g_root_children.size());
    CHECK(av_g_root_children[0].isValid());
    CHECK(av_g_root_children[1].isValid());

    av::osg::Group* av_n_check1 = dynamic_cast<av::osg::Group*>(av_g_root_children[0].getPtr());
    av::osg::Group* av_n_check2 = dynamic_cast<av::osg::Group*>(av_g_root_children[1].getPtr());

    CHECK_EQUAL(av_n_check1, av_g_child1.getPtr());
    CHECK_EQUAL(av_n_check2, av_g_child2.getPtr());

    // check OSG structure
    ::osg::ref_ptr< ::osg::Group> osg_g_root_ref_ptr = av_g_root->getOsgGroup();

    CHECK(osg_g_root_ref_ptr.valid());
    CHECK_EQUAL(2u, osg_g_root_ref_ptr->getNumChildren());

    ::osg::Group* osg_g_child1 = dynamic_cast< ::osg::Group*>(osg_g_root_ref_ptr->getChild(0));
    ::osg::Group* osg_g_child2 = dynamic_cast< ::osg::Group*>(osg_g_root_ref_ptr->getChild(1));

    CHECK(osg_g_child1);
    CHECK(osg_g_child2);

    // check way back from OSG to Avango
    av::osg::ObjectLink* av_g_root_objectlink   =
      dynamic_cast<av::osg::ObjectLink*>(osg_g_root_ref_ptr->getUserData());
    av::osg::ObjectLink* av_g_child1_objectlink =
      dynamic_cast<av::osg::ObjectLink*>(osg_g_child1->getUserData());
    av::osg::ObjectLink* av_g_child2_objectlink =
      dynamic_cast<av::osg::ObjectLink*>(osg_g_child2->getUserData());

    CHECK(av_g_root_objectlink);
    CHECK(av_g_child1_objectlink);
    CHECK(av_g_child2_objectlink);

    CHECK(av_g_root_objectlink->getObject() != 0);
    CHECK(av_g_child1_objectlink->getObject() != 0);
    CHECK(av_g_child2_objectlink->getObject() != 0);

    CHECK_EQUAL(dynamic_cast<av::osg::Group*>(av_g_root_objectlink->getObject()),   av_g_root.getPtr());
    CHECK_EQUAL(dynamic_cast<av::osg::Group*>(av_g_child1_objectlink->getObject()), av_g_child1.getPtr());
    CHECK_EQUAL(dynamic_cast<av::osg::Group*>(av_g_child2_objectlink->getObject()), av_g_child2.getPtr());
  }

  TEST_FIXTURE(InitNodeFixture, testMatrixTransform)
  {
    av::Link<av::osg::MatrixTransform> av_g_root = new av::osg::MatrixTransform;

    CHECK(av_g_root.isValid());

    ::osg::Matrix osg_m(0.0, 0.1, 0.2, 0.3,
                        1.0, 1.1, 1.2, 1.3,
                        2.0, 2.1, 2.2, 2.3,
                        3.0, 3.1, 2.2, 3.3);

    av_g_root->Matrix.setValue(osg_m);

    // TODO: finish test
  }

  TEST_FIXTURE(InitNodeFixture, testWireframeMode)
  {
    av::Link<av::osg::StateSet> av_state = new av::osg::StateSet;
    CHECK(av_state.isValid());

    av_state->WireframeMode.setValue(1);
    CHECK(av_state->WireframeMode.getValue() == 1);
  }

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  TEST_FIXTURE(InitNodeFixture, testNetMatrixTransform)
  {
    av::Link<av::osg::NetMatrixTransform> av_net_transform = new av::osg::NetMatrixTransform;

    CHECK(av_net_transform.isValid());

  }
#endif

} // namespace
