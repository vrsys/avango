// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#if !defined(AVANGO_UTILS_TRACKBALL_H)
#define AVANGO_UTILS_TRACKBALL_H

/**
 * \file
 * \ingroup av_utils
 */

#include <avango/osg/Fields.h>
#include <avango/FieldContainer.h>
#include "windows_specific_utils.h"

namespace av
{
  namespace utils
  {
    /**
     * Trackball mover
     *
     * \ingroup av_utils
     */
    class AV_UTILS_DLL Trackball : public av::FieldContainer
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
       * The trackball rotation will be written into this field.
       * Connect this matrix to the Camera/Viewer Transform
       */
      av::osg::SFMatrix Matrix;

      /**
       * Connect this to a TimeSensor. Needed for the spinning calculation.
       */
      av::SFDouble TimeIn;

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
       * Reset the current transformation of the trackball
       */
      av::SFBool ResetTrigger;

      /**
       * If set to true, the specified center transform will automatically be adjusted
       * so that the trackball still rotates around the specified object, regardless of any applied zoom
       * Default: True
       */
      av::SFBool AutoAdjustCenterTransform;

      /**
       * All rotation interactions whose duration (in seconds) is below this threshold trigger a spinning
       * Default: 0.3
       */
      av::SFFloat SpinningTimeThreshold;

      /**
       * Wheighting factor which multiplied with all previous rotations to calculate the spinning rotation.
       * Default: 0.97
       */
      av::SFFloat SpinningWeightingCoefficient;

      /**
       * Defines the orientation and translation of the rotation center relative
       * to the local coordinate system. Default is 0.6 meter in front of it.
       */
      av::osg::SFMatrix CenterTransform;
      /**
       * Defines the center transform, which will be set if the field ResetTrigger is set to true
       */
      av::osg::SFMatrix CenterTransformReset;

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    private:

      float mTimeLastMovement;
      ::osg::Matrix mRotation;

      ::osg::Vec2 mLastDirection;
      ::osg::Vec3 mLastProjected;
      bool mDragging;
      bool mSpinning;
      ::osg::Matrix mCenterTransInv;
    };

    typedef SingleField<Link<Trackball> > SFTrackball;
    typedef MultiField<Link<Trackball> > MFTrackball;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_UTILS_DLL SingleField<Link<utils::Trackball> >;
  template class AV_UTILS_DLL MultiField<Link<utils::Trackball> >;
#endif
}

#endif
