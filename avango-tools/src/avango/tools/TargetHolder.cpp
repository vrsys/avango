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

#include <avango/tools/TargetHolder.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::TargetHolder"));
}

AV_FC_DEFINE(av::tools::TargetHolder);

AV_FIELD_DEFINE(av::tools::SFTargetHolder);
AV_FIELD_DEFINE(av::tools::MFTargetHolder);

av::tools::TargetHolder::TargetHolder()
{
  AV_FC_ADD_FIELD(Target, 0);
  AV_FC_ADD_FIELD(Creator, 0);
  AV_FC_ADD_FIELD(ParentTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Keep, false);
}

av::tools::TargetHolder::~TargetHolder()
{}

void
av::tools::TargetHolder::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::tools::TargetHolder, true);

    SFTargetHolder::initClass("av::tools::SFTargetHolder", "av::Field");
    MFTargetHolder::initClass("av::tools::MFTargetHolder", "av::Field");
  }
}

av::tools::TargetHolder::FindList
av::tools::TargetHolder::find(Type type)
{
  FindList found_holders;
  find(type, found_holders);
  return found_holders;
}

/* virtual */ bool
av::tools::TargetHolder::keep()
{
  return (Keep.getValue() || !ParentTargets.isEmpty());
}

void
av::tools::TargetHolder::find(Type type, FindList& foundHolders)
{
  if (getTypeId().isOfType(type))
    foundHolders.push_back(this);

  const MFTargetHolder::ContainerType &parent_targets = ParentTargets.getValue();
  for (MFTargetHolder::ContainerType::const_iterator parent_holder = parent_targets.begin();
       parent_holder != parent_targets.end(); ++parent_holder)
  {
    (*parent_holder)->find(type, foundHolders);
  }
}
