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

#include <avango/gua/network/SharedContainerHolder.h>
#include <avango/gua/network/NetTransform.h>

namespace
{
av::Logger& logger(av::getLogger("av::gua::SharedContainerHolder"));
}

AV_FC_DEFINE(av::gua::SharedContainerHolder);

av::gua::SharedContainerHolder::SharedContainerHolder() { AV_FC_ADD_FIELD(SharedContainers, std::vector<av::Link<av::FieldContainer>>()); }

/* virtual */
av::gua::SharedContainerHolder::~SharedContainerHolder() {}

/* static */ void av::gua::SharedContainerHolder::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();
        AV_FC_INIT(av::FieldContainer, av::gua::SharedContainerHolder, true);
        sClassTypeId.setDistributable(true);
    }
}

/* virtual */ void av::gua::SharedContainerHolder::fieldHasChangedLocalSideEffect(const Field& field)
{
    FieldContainer::fieldHasChangedLocalSideEffect(field);

    mContainersChanged = (&field == &SharedContainers);
}

void av::gua::SharedContainerHolder::evaluateLocalSideEffect()
{
    FieldContainer::evaluateLocalSideEffect();

    if(mContainersChanged && mNetTransform)
    {
        mNetTransform->sharedContainersChanged();
    }
}

void av::gua::SharedContainerHolder::registerNetTransform(av::gua::NetTransform* netMatrixTransform) { mNetTransform = netMatrixTransform; }
