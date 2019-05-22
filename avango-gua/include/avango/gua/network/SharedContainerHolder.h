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

#if !defined(AVANGO_OSG_SHAREDCONTAINERHOLDER_H)
#define AVANGO_OSG_SHAREDCONTAINERHOLDER_H

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
class NetTransform;

class SharedContainerHolder : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    SharedContainerHolder();
    virtual ~SharedContainerHolder();

    MFContainer SharedContainers;

    void fieldHasChangedLocalSideEffect(const Field&);
    void evaluateLocalSideEffect();

    void registerNetTransform(NetTransform* netMatrixTransform);

  private:
    NetTransform* mNetTransform;
    bool mContainersChanged;
};
} // namespace gua

} // namespace av

#endif
