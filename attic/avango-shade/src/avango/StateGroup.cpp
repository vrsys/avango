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

#include <avango/shade/StateGroup.h>
#include <avango/osg/Object.h>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::shade::StateGroup"));
}

AV_FC_DEFINE(av::shade::StateGroup);

av::shade::StateGroup::StateGroup()
{
  AV_FC_ADD_FIELD(State, 0);

  UpdateCallback* uc = new UpdateCallback();
  getOsgNode()->setUpdateCallback(uc);
}

av::shade::StateGroup::~StateGroup()
{}

void
av::shade::StateGroup::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();
    av::shade::State::initClass();

    AV_FC_INIT(av::osg::Group, av::shade::StateGroup, true);
  }
}

/* virtual */ void
av::shade::StateGroup::evaluate()
{
  av::osg::Group::evaluate();

  if (!State.getValue().isValid())
    return;

  State.getValue()->applyState(getOsgNode());
}

/*virtual*/ void
av::shade::StateGroup::UpdateCallback::operator() (::osg::Node *node, ::osg::NodeVisitor *nv)
{
  callUpdate(node);
  traverse(node, nv);
}

/*virtual*/ void
av::shade::StateGroup::UpdateCallback::callUpdate(::osg::Node *node) const
{
  av::Link<av::shade::StateGroup> state_group(av::osg::get_from_osg_object<av::shade::StateGroup>(node));
  if (!state_group.isValid())
    return;

  av::Link<av::shade::State> state(state_group->State.getValue());
  if (!state.isValid())
    return;
  
  state->update();
}
