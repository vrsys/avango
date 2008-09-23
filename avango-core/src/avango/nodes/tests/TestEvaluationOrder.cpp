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

#include <avango/Application.h>
#include <avango/Object.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  class EvaluateNode : public av::Object
  {
    AV_FC_DECLARE();

  public:

    EvaluateNode();

    av::SFInt IntFieldIn;
    av::SFInt IntFieldOut;

    /* virtual */ void evaluate();

    int mEvaluateIndex;
    static int sEvaluateIndex;
  };

  AV_FC_DEFINE(EvaluateNode);

  int EvaluateNode::sEvaluateIndex = 0;

  EvaluateNode::EvaluateNode() :
    mEvaluateIndex(0)
  {
    AV_FC_ADD_FIELD(IntFieldIn, 0);
    AV_FC_ADD_FIELD(IntFieldOut, 0);
  }

  void
  EvaluateNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();
      AV_FC_INIT(av::Object, EvaluateNode, false);
    }
  }

  /* virtual */ void
  EvaluateNode::evaluate()
  {
    mEvaluateIndex = sEvaluateIndex++;
    if (IntFieldIn.getValue() < 10)
      IntFieldOut.setValue(IntFieldIn.getValue() + 1);
  }

  class DependantNode : public av::Object
  {
    AV_FC_DECLARE();

  public:

    DependantNode();

    av::SFContainer Dependency;

    /* virtual */ void evaluate();
  };

  AV_FC_DEFINE(DependantNode);

  DependantNode::DependantNode()
  {
    AV_FC_ADD_FIELD(Dependency, 0);
  }

  void
  DependantNode::initClass()
  {
    if (!isTypeInitialized())
    {
      av::Object::initClass();
      AV_FC_INIT(av::Object, DependantNode, false);
    }
  }

  /* virtual */ void
  DependantNode::evaluate()
  {
    if (Dependency.getValue().isValid())
      evaluateDependency(*Dependency.getValue());
  }


  TEST(endlessEvaluateLoop)
  {
    EvaluateNode::initClass();

    av::Link<EvaluateNode> node1(new EvaluateNode);
    av::Link<EvaluateNode> node2(new EvaluateNode);

    node1->IntFieldIn.connectFrom(&node2->IntFieldOut);
    node2->IntFieldIn.connectFrom(&node1->IntFieldOut);

    av::ApplicationInstance::get().evaluate();

    CHECK(node1->IntFieldIn.getValue() < 3 && node2->IntFieldIn.getValue() < 3);

    node1->IntFieldIn.disconnect();
    node2->IntFieldIn.disconnect();
  }

  TEST(evaluationOrder)
  {
    EvaluateNode::initClass();
    EvaluateNode::sEvaluateIndex = 0;

    av::Link<EvaluateNode> node1(new EvaluateNode);
    av::Link<EvaluateNode> node2(new EvaluateNode);
    av::Link<EvaluateNode> node3(new EvaluateNode);

    node2->IntFieldIn.connectFrom(&node1->IntFieldOut);
    node3->IntFieldIn.connectFrom(&node2->IntFieldOut);
    av::ApplicationInstance::get().evaluate();

    EvaluateNode::sEvaluateIndex = 1;
    node3->touch();
    node1->IntFieldIn.setValue(1);
    av::ApplicationInstance::get().evaluate();

    CHECK_EQUAL(1, node1->mEvaluateIndex);
    CHECK_EQUAL(2, node2->mEvaluateIndex);
    CHECK_EQUAL(3, node3->mEvaluateIndex);

    CHECK_EQUAL(1, node1->IntFieldIn.getValue());
    CHECK_EQUAL(2, node1->IntFieldOut.getValue());
    CHECK_EQUAL(2, node2->IntFieldIn.getValue());
    CHECK_EQUAL(3, node2->IntFieldOut.getValue());
    CHECK_EQUAL(3, node3->IntFieldIn.getValue());
    CHECK_EQUAL(4, node3->IntFieldOut.getValue());

    node2->IntFieldIn.disconnect();
    node3->IntFieldIn.disconnect();
  }

  TEST(deactivateDependency)
  {
    EvaluateNode::initClass();
    EvaluateNode::sEvaluateIndex = 1;

    av::Link<EvaluateNode> node1(new EvaluateNode);
    av::Link<EvaluateNode> node2(new EvaluateNode);

    node1->IntFieldIn.enableDependency(false);
    node1->IntFieldIn.connectFrom(&node2->IntFieldOut);
    node2->IntFieldIn.connectFrom(&node1->IntFieldOut);

    node1->IntFieldIn.setValue(1);
    av::ApplicationInstance::get().evaluate();

    CHECK_EQUAL(1, node1->mEvaluateIndex);
    CHECK_EQUAL(2, node2->mEvaluateIndex);

    node1->IntFieldIn.disconnect();
    node2->IntFieldIn.disconnect();
  }

  TEST(weakConnectionDependency)
  {
    EvaluateNode::initClass();
    EvaluateNode::sEvaluateIndex = 1;

    av::Link<EvaluateNode> node1(new EvaluateNode);
    av::Link<EvaluateNode> node2(new EvaluateNode);

    node1->IntFieldIn.connectFrom(&node2->IntFieldOut, false);
    node2->IntFieldIn.connectFrom(&node1->IntFieldOut);

    node1->IntFieldIn.setValue(1);
    av::ApplicationInstance::get().evaluate();

    CHECK_EQUAL(1, node1->mEvaluateIndex);
    CHECK_EQUAL(2, node2->mEvaluateIndex);

    node1->IntFieldIn.disconnect();
    node2->IntFieldIn.disconnect();
  }

  TEST(disableAllowScheduling)
  {
    EvaluateNode::initClass();
    EvaluateNode::sEvaluateIndex = 1;

    av::Link<EvaluateNode> node(new EvaluateNode);
    node->allowScheduling(false);

    node->IntFieldIn.setValue(1);
    av::ApplicationInstance::get().evaluate();

    CHECK_EQUAL(0, node->mEvaluateIndex);
  }

  TEST(disableAllowSchedulingAndPull)
  {
    EvaluateNode::initClass();
    EvaluateNode::sEvaluateIndex = 1;
    DependantNode::initClass();

    av::Link<EvaluateNode> node(new EvaluateNode);
    node->allowScheduling(false);

    av::Link<DependantNode> puller(new DependantNode);
    puller->Dependency.setValue(node);

    node->IntFieldIn.setValue(1);
    av::ApplicationInstance::get().evaluate();

    CHECK_EQUAL(1, node->mEvaluateIndex);
  }
}
