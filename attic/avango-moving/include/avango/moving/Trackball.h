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

#if !defined(AVANGO_MOVING_TRACKBALL_H)
#define AVANGO_MOVING_TRACKBALL_H

/**
 * \file
 * \ingroup av_moving
 */

#include <avango/osg/Fields.h>
#include <avango/osg/MatrixTransform.h>
#include "windows_specific_moving.h"

namespace av
{
namespace moving
{
/**
 * Trackball mover
 *
 * \ingroup av_moving
 */
class AV_MOVING_DLL Trackball : public av::osg::MatrixTransform
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    Trackball();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Trackball();

  public:
    /**
     * Direction of trackball movement.
     * Typically connected from the MousePositionNorm field of a GraphicsWindow.
     */
    av::osg::SFVec2 Direction;

    /**
     * Trigger for rotating and panning.
     * Typically connected from one of the mouse button fields of a GraphicsWindow.
     */
    SFBool RotateTrigger;
    SFBool ZoomTrigger;
    SFBool PanTrigger;

    /**
     * Defines the orientation and translation of the rotation center relative
     * to the local coordinate system. Default is 0.6 meter in front of it.
     */
    av::osg::SFMatrix CenterTransform;

    /* virtual */ void fieldHasChanged(const av::Field& field);
    /* virtual */ void evaluate();

  private:
    ::osg::Vec2 mLastDirection;
    ::osg::Vec3 mLastProjected;
    bool mDragging;
    ::osg::Matrix mCenterTransInv;
};

typedef SingleField<Link<Trackball>> SFTrackball;
typedef MultiField<Link<Trackball>> MFTrackball;
} // namespace moving

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_MOVING_DLL SingleField<Link<moving::Trackball>>;
template class AV_MOVING_DLL MultiField<Link<moving::Trackball>>;
#endif
} // namespace av

#endif
