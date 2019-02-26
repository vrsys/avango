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

#if !defined(AVANGO_TOOLS_SCALEDGRABTOOL_H)
#define AVANGO_TOOLS_SCALEDGRABTOOL_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Tool.h>
#include <avango/osg/Fields.h>
#include "windows_specific_tools.h"

namespace av
{
namespace tools
{
/**
 * ScaledGrabTool class
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL ScaledGrabTool : public Tool
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    ScaledGrabTool();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ScaledGrabTool();

  public:
    /**
     * Defines the device transformation relative to the root node coordinate system.
     */
    av::osg::SFMatrix DeviceTransform;

    /**
     * Defines the head transformation relative to the root node coordinate system.
     */
    av::osg::SFMatrix HeadTransform;

  protected:
    /* virtual */ void evaluateAddedTarget(TargetHolder& holder);
    /* virtual */ void evaluateKeptTarget(TargetHolder& holder);

    ::osg::Matrix mTargetMat, mTargetRotMat, mTargetTransMat;
    ::osg::Matrix mDeviceInvMat;
    ::osg::Matrix mPivotMat, mPivotInvMat;
    ::osg::Vec3 mDevicePos;
    double mScale;
};

using SFScaledGrabTool = SingleField<Link<ScaledGrabTool>>;
using MFScaledGrabTool = MultiField<Link<ScaledGrabTool>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::ScaledGrabTool>>;
template class AV_TOOLS_DLL MultiField<Link<tools::ScaledGrabTool>>;
#endif

} // namespace av

#endif
