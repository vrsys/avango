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
#ifndef AUTOTRANSFORM_H_
#define AUTOTRANSFORM_H_


/**
 * \file
 * \ingroup av_osg
 */

#include <osg/AutoTransform>
#include <avango/osg/Fields.h>
#include <avango/osg/Drawable.h>
#include <avango/osg/Transform.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::AutoTransform OsgAutoTransform;

    /**
     * Wrapper for ::osg::AutoTransform
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL AutoTransform : public Transform
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::AutoTransform is created.
       * Otherwise, the given ::osg::AutoTransform is used.
       */
      AutoTransform(OsgAutoTransform* osgAutoTransform = new OsgAutoTransform());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~AutoTransform();

    public:

      typedef enum {
          NO_ROTATION      = ::osg::AutoTransform::NO_ROTATION,
          ROTATE_TO_SCREEN = ::osg::AutoTransform::ROTATE_TO_SCREEN,
          ROTATE_TO_CAMERA = ::osg::AutoTransform::ROTATE_TO_CAMERA
      } RotationModeType;
      /**
       * Set the AutoTransform rotation mode
       */
      SFUInt AutoRotateMode;

      /**
       * Minimal scale of the children
       */
      SFDouble MinimumScale;

      /**
       * Scale
       */
      SFVec3 Scale;

      /**
       * Maximal scale of the children
       */
      SFDouble MaximumScale;

      /**
       * AutoScaleToScreen
       */
      SFBool AutoScaleToScreen;

      /**
       * Set the position
       */
      SFVec3 Position;

      /**
       * Set the pivot point
       */
      SFVec3 PivotPoint;

      /**
       * Set the AutoScaleTransitionWidthRatio
       */
      SFFloat AutoScaleTransitionWidthRatio;



      /**
       * Get the wrapped ::osg::Geometry.
       */
      OsgAutoTransform* getOsgAutoTransform() const;

    protected:

      virtual void getScaleCB(const av::osg::SFVec3::GetValueEvent& event);
      virtual void setScaleCB(const av::osg::SFVec3::SetValueEvent& event);

      virtual void getMinimumScaleCB(const av::SFDouble::GetValueEvent& event);
      virtual void setMinimumScaleCB(const av::SFDouble::SetValueEvent& event);

      virtual void getMaximumScaleCB(const av::SFDouble::GetValueEvent& event);
      virtual void setMaximumScaleCB(const av::SFDouble::SetValueEvent& event);

      virtual void getAutoRotateModeCB(const SFUInt::GetValueEvent& event);
      virtual void setAutoRotateModeCB(const SFUInt::SetValueEvent& event);

      virtual void getAutoScaleToScreenCB(const SFBool::GetValueEvent& event);
      virtual void setAutoScaleToScreenCB(const SFBool::SetValueEvent& event);

      virtual void getPositionCB(const av::osg::SFVec3::GetValueEvent& event);
      virtual void setPositionCB(const av::osg::SFVec3::SetValueEvent& event);

      virtual void getPivotPointCB(const av::osg::SFVec3::GetValueEvent& event);
      virtual void setPivotPointCB(const av::osg::SFVec3::SetValueEvent& event);

      virtual void getAutoScaleTransitionWidthRatioCB(const av::SFFloat::GetValueEvent& event);
      virtual void setAutoScaleTransitionWidthRatioCB(const av::SFFloat::SetValueEvent& event);

    private:

      OsgAutoTransform *mOsgAutoTransform;
    };

    typedef SingleField<Link<AutoTransform> > SFAutoTransform;
    typedef MultiField<Link<AutoTransform> > MFAutoTransform;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::AutoTransform> >;
  template class AV_OSG_DLL MultiField<Link<osg::AutoTransform> >;
#endif
}

#endif /* AUTOTRANSFORM_H_ */
