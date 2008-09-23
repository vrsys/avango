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

#if !defined(AVANGO_OSG_VIEWER_COMPOSITEVIEWER_H)
#define AVANGO_OSG_VIEWER_COMPOSITEVIEWER_H

/**
 * \file
 * \ingroup av_osg_viewer
 */

#include <osgViewer/CompositeViewer>

#include <avango/Application.h>
#include <avango/osg/Object.h>
#include <avango/osg/viewer/View.h>
#include <avango/osg/viewer/windows_specific_osg_viewer.h>

namespace av
{
  namespace osg
  {
    namespace viewer
    {
      /**
       * Wrapper for ::osgViewer::CompositeViewer.
       * Use it to render more than one scene.
       * OpenSceneGraph allows only one Viewer in an application!
       *
       * \ingroup av_osg_viewer
       */
      class AV_OSG_VIEWER_DLL CompositeViewer : public av::osg::Object
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. Creates a new ::osgViewer::CompositeViewer for internal use.
         */
        CompositeViewer();

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~CompositeViewer();

      public:

        /**
         * Views to be rendered.
         */
        MFView Views;

        /**
         * Threading model. See osgViewer/ViewerBase for enum values.
         */
        SFInt ThreadingModel;

        /* virtual */ void fieldHasChangedLocalSideEffect(const Field& field);

        /**
         * Get the wrapped ::osgViewer::CompositeViewer.
         */
        ::osgViewer::CompositeViewer* getOsgCompositeViewer() const;

        /**
         * Evaluate and renders one frame.
         */
        void frame();

        /**
         * Start the evaluation and render loop.
         */
        bool run();

        /**
         * Stop the evaluation and render loop.
         */
        bool done();

        /**
         * Render callback.
         */
        void renderCB();

        virtual void getThreadingModelCB(const SFInt::GetValueEvent& event);
        virtual void setThreadingModelCB(const SFInt::SetValueEvent& event);

      private:

        ::osgViewer::CompositeViewer *mOsgCompositeViewer;
        MFView::ContainerType mLastViews;
        av::Application::CallbackHandle mRenderCallbackHandle;
      };

      typedef SingleField<Link<CompositeViewer> > SFCompositeViewer;

    } // namespace viewer
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_VIEWER_DLL SingleField<Link<osg::viewer::CompositeViewer> >;
#endif

} // namespace av

#endif
