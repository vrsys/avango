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

#include <avango/Application.h>
#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/UnitTest++/UnitTest++.h>

namespace
{
class CopyNode : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    CopyNode();

    av::SFInt FieldIn;
    av::SFInt FieldOut;

    /* virtual */ void evaluate();
};

AV_FC_DEFINE(CopyNode);

CopyNode::CopyNode()
{
    AV_FC_ADD_FIELD(FieldIn, 0);
    AV_FC_ADD_FIELD(FieldOut, 0);
}

void CopyNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();
        AV_FC_INIT(av::FieldContainer, CopyNode, false);
    }
}

/* virtual */ void CopyNode::evaluate() { FieldOut.setValue(FieldIn.getValue()); }

TEST(ConstantSizeOfEvaluationLoop)
{
    CopyNode::initClass();
    av::FieldContainer::unscheduleEvaluationForAllContainers();

    av::Link<CopyNode> node1(new CopyNode);
    node1->alwaysEvaluate(true);
    CHECK_EQUAL(1, av::FieldContainer::getNumberOfContainersToEvaluate());

    av::Link<CopyNode> node2(new CopyNode);
    node2->FieldOut.touch();
    CHECK_EQUAL(2, av::FieldContainer::getNumberOfContainersToEvaluate());

    node1->FieldIn.connectFrom(&node2->FieldOut);
    node2->FieldIn.connectFrom(&node1->FieldOut, false);

    av::ApplicationInstance::get().evaluate();

    // Node 1 will always be evaluated and Node 2 should be dirty from
    // weak field connection
    CHECK_EQUAL(2, av::FieldContainer::getNumberOfContainersToEvaluate());

    node1->FieldIn.disconnect();
    node2->FieldIn.disconnect();
}
} // namespace
