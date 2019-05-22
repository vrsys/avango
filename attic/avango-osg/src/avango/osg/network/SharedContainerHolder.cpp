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

#include <avango/osg/network/SharedContainerHolder.h>
#include <avango/osg/network/NetMatrixTransform.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::SharedContainerHolder"));
}

AV_FC_DEFINE(av::osg::SharedContainerHolder);

av::osg::SharedContainerHolder::SharedContainerHolder() { AV_FC_ADD_FIELD(SharedContainers, std::vector<av::Link<av::FieldContainer>>()); }

/* virtual */
av::osg::SharedContainerHolder::~SharedContainerHolder() {}

/* static */ void av::osg::SharedContainerHolder::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();
        AV_FC_INIT(av::FieldContainer, av::osg::SharedContainerHolder, true);
        sClassTypeId.setDistributable(true);
    }
}

/* virtual */ void av::osg::SharedContainerHolder::fieldHasChangedLocalSideEffect(const Field& field)
{
    FieldContainer::fieldHasChangedLocalSideEffect(field);

    mContainersChanged = (&field == &SharedContainers);
}

void av::osg::SharedContainerHolder::evaluateLocalSideEffect()
{
    FieldContainer::evaluateLocalSideEffect();

    if(mContainersChanged && mNetMatrixTransform)
    {
        mNetMatrixTransform->sharedContainersChanged();
    }
}

void av::osg::SharedContainerHolder::registerNetMatrixTransform(av::osg::NetMatrixTransform* netMatrixTransform) { mNetMatrixTransform = netMatrixTransform; }
