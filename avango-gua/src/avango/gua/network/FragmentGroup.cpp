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

#include <avango/gua/network/FragmentGroup.h>

#include <avango/gua/network/NetMatrixTransform.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::FragmentGroup"));
}

AV_FC_DEFINE(av::gua::FragmentGroup);

AV_FIELD_DEFINE(av::gua::SFFragmentGroup);
AV_FIELD_DEFINE(av::gua::MFFragmentGroup);

av::gua::FragmentGroup::FragmentGroup() :
  mNetMatrixTransform(0),
  mChildrenChanged(false)
{}

/* virtual */
av::gua::FragmentGroup::~FragmentGroup()
{}

/* static */ void
av::gua::FragmentGroup::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::Node::initClass();

    AV_FC_INIT(av::gua::Node, av::gua::FragmentGroup, false);

    SFFragmentGroup::initClass("av::gua::SFFragmentGroup", "av::Field");
    MFFragmentGroup::initClass("av::gua::MFFragmentGroup", "av::Field");
  }
}

/* virtual */ void
av::gua::FragmentGroup::fieldHasChangedLocalSideEffect(const Field& field)
{
  Node::fieldHasChangedLocalSideEffect(field);

  mChildrenChanged = (&field == &Children);
}

void
av::gua::FragmentGroup::evaluateLocalSideEffect()
{
  Node::evaluateLocalSideEffect();

  if (mChildrenChanged && mNetMatrixTransform) {
    mNetMatrixTransform->fragmentChildrenChanged();
  }
}

void
av::gua::FragmentGroup::registerNetMatrixTransform(NetMatrixTransform* netMatrixTransform)
{
  mNetMatrixTransform = netMatrixTransform;
}


