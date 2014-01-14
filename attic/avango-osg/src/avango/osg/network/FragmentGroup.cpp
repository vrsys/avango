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

#include <avango/osg/network/FragmentGroup.h>

#include <avango/osg/network/NetMatrixTransform.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::FragmentGroup"));
}

AV_FC_DEFINE(av::osg::FragmentGroup);

AV_FIELD_DEFINE(av::osg::SFFragmentGroup);
AV_FIELD_DEFINE(av::osg::MFFragmentGroup);

av::osg::FragmentGroup::FragmentGroup() :
  mNetMatrixTransform(0),
  mChildrenChanged(false)
{}

/* virtual */
av::osg::FragmentGroup::~FragmentGroup()
{}

/* static */ void
av::osg::FragmentGroup::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::FragmentGroup, false);

    SFFragmentGroup::initClass("av::osg::SFFragmentGroup", "av::Field");
    MFFragmentGroup::initClass("av::osg::MFFragmentGroup", "av::Field");
  }
}

/* virtual */ void
av::osg::FragmentGroup::fieldHasChangedLocalSideEffect(const Field& field)
{
  Group::fieldHasChangedLocalSideEffect(field);

  mChildrenChanged = (&field == &Children);
}

void
av::osg::FragmentGroup::evaluateLocalSideEffect()
{
  Group::evaluateLocalSideEffect();

  if (mChildrenChanged && mNetMatrixTransform) {
    mNetMatrixTransform->fragmentChildrenChanged();
  }
}

void
av::osg::FragmentGroup::registerNetMatrixTransform(NetMatrixTransform* netMatrixTransform)
{
  mNetMatrixTransform = netMatrixTransform;
}


