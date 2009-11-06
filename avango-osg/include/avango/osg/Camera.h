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

#if !defined(AVANGO_OSG_CAMERA_H)
#define AVANGO_OSG_CAMERA_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Camera>
#include <avango/osg/Transform.h>
#include <avango/osg/CameraAttachment.h>
#include <avango/StandardFields.h>
#include <avango/osg/Fields.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Camera OsgCamera;

    /**
     * Low-Level Wrapper for ::osg::Camera used mostly for pre-render effects
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Camera : public Transform
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Camera is created.
       * Otherwise, the given ::osg::Camera is used.
       */
      Camera(OsgCamera* osgswitch = new OsgCamera());
      // use defined type to circumvent compiler bug in VS8

      /**
       * Selects buffers to clear
       */
      SFInt ClearMask;

      /**
       * Selects color to use for clearing color buffers
       */
      SFVec4 ClearColor;

      /**
       * The view matrix for the camera
       */
      SFMatrix ViewMatrix;

      /**
       * The camera matrix use by the camera
       */
      SFMatrix ProjectionMatrix;

      /**
       * The viewport of the camera
       */
      SFVec4 Viewport;

      /**
       * Selects when this camera should be rendered
       */
      SFInt RenderOrder;

      /**
       * Choose implementation for buffer attachements
       */
      SFInt RenderTargetImplementation;

      /**
       * List of attaches textures or images
       */
      MFCameraAttachment Attachment;

      /**
       * Get the wrapped ::osg::Camera.
       */
      ::osg::Camera* getOsgCamera() const;

    private:

      ::osg::Camera *mOsgCamera;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Camera();

      void getClearMaskCB(const av::SFInt::GetValueEvent& event);
      void setClearMaskCB(const av::SFInt::SetValueEvent& event);
      void getClearColorCB(const av::osg::SFVec4::GetValueEvent& event);
      void setClearColorCB(const av::osg::SFVec4::SetValueEvent& event);
      void getViewMatrixCB(const av::osg::SFMatrix::GetValueEvent& event);
      void setViewMatrixCB(const av::osg::SFMatrix::SetValueEvent& event);
      void getProjectionMatrixCB(const av::osg::SFMatrix::GetValueEvent& event);
      void setProjectionMatrixCB(const av::osg::SFMatrix::SetValueEvent& event);
      void getViewportCB(const av::osg::SFVec4::GetValueEvent& event);
      void setViewportCB(const av::osg::SFVec4::SetValueEvent& event);
      void getRenderOrderCB(const av::SFInt::GetValueEvent& event);
      void setRenderOrderCB(const av::SFInt::SetValueEvent& event);
      void getRenderTargetImplementationCB(const av::SFInt::GetValueEvent& event);
      void setRenderTargetImplementationCB(const av::SFInt::SetValueEvent& event);
      void getAttachmentCB(const av::osg::MFCameraAttachment::GetValueEvent& event);
      void setAttachmentCB(const av::osg::MFCameraAttachment::SetValueEvent& event);
    };

    typedef SingleField<Link<Camera> > SFCamera;
    typedef MultiField<Link<Camera> > MFCamera;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Camera> >;
  template class AV_OSG_DLL MultiField<Link<osg::Camera> >;
#endif

}

#endif
