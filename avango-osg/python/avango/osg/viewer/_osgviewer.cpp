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

#include <avango/osg/viewer/Camera.h>
#include <avango/osg/viewer/CompositeViewer.h>
#include <avango/osg/viewer/GraphicsWindow.h>
#include <avango/osg/viewer/EventFields.h>
#include <avango/osg/viewer/Init.h>
#include <avango/osg/viewer/View.h>
#include <avango/osg/viewer/Viewer.h>

#include <avango/python/register_field.h>

#include <boost/python.hpp>

using namespace av::python;
using namespace boost::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
  }
}

BOOST_PYTHON_MODULE(_viewer)
{
  av::osg::viewer::Init::initClass();

  register_field<av::osg::viewer::SFCamera>("SFCamera");
  register_multifield<av::osg::viewer::MFCamera>("MFCamera");
  register_field<av::osg::viewer::SFCompositeViewer>("SFCompositeViewer");
  register_field<av::osg::viewer::SFGraphicsWindow>("SFGraphicsWindow");
  register_multifield<av::osg::viewer::MFGraphicsWindow>("MFGraphicsWindow");
  register_field<av::osg::viewer::SFEventFields>("SFEventFields");
  register_multifield<av::osg::viewer::MFEventFields>("MFEventFields");
  register_field<av::osg::viewer::SFView>("SFView");
  register_multifield<av::osg::viewer::MFView>("MFView");
  register_field<av::osg::viewer::SFViewer>("SFViewer");

  class_<av::osg::viewer::Camera, av::Link<av::osg::viewer::Camera>, bases<av::osg::Object>, boost::noncopyable >("Camera", "Camera: defines Clipping-Planes ; Screen-, Viewer-, Eyetransformation ; Viewport", no_init);
  class_<av::osg::viewer::CompositeViewer, av::Link<av::osg::viewer::CompositeViewer>, bases<av::osg::Object>, boost::noncopyable >("CompositeViewer", "CompositeViewer: contains and renders views", no_init)
    .def("frame", &av::osg::viewer::CompositeViewer::frame)
    .def("run", &av::osg::viewer::CompositeViewer::run)
    .def("done", &av::osg::viewer::CompositeViewer::done)
    .def("frame_without_evaluation", &av::osg::viewer::CompositeViewer::frameWithoutEvaluation)
    ;
  class_<av::osg::viewer::GraphicsWindow, av::Link<av::osg::viewer::GraphicsWindow>, bases<av::FieldContainer>, boost::noncopyable >("GraphicsWindow", "GraphicsWindow: position and size of the outputwindow ", no_init);
  class_<av::osg::viewer::EventFields, av::Link<av::osg::viewer::EventFields>, bases<av::FieldContainer>, boost::noncopyable >("EventFields", "EventFields: setting fields for exposing events ", no_init);
  class_<av::osg::viewer::View, av::Link<av::osg::viewer::View>, bases<av::osg::Object>, boost::noncopyable >("View", "View: contains cameras and the scene", no_init);
  class_<av::osg::viewer::Viewer, av::Link<av::osg::viewer::Viewer>, bases<av::osg::viewer::View>, boost::noncopyable >("Viewer", "Viewer: is and renders a View", no_init)
    .def("frame", &av::osg::viewer::Viewer::frame)
    .def("run", &av::osg::viewer::Viewer::run)
    .def("done", &av::osg::viewer::Viewer::done)
    ;

  enum_<av::osg::viewer::GraphicsWindow::StereoModeType>("stereo_mode")
    .value("STEREO_MODE_QUAD_BUFFER", av::osg::viewer::GraphicsWindow::STEREO_MODE_QUAD_BUFFER)
    .value("STEREO_MODE_ANAGLYPHIC", av::osg::viewer::GraphicsWindow::STEREO_MODE_ANAGLYPHIC)
    .value("STEREO_MODE_HORIZONTAL_SPLIT", av::osg::viewer::GraphicsWindow::STEREO_MODE_HORIZONTAL_SPLIT)
    .value("STEREO_MODE_VERTICAL_SPLIT", av::osg::viewer::GraphicsWindow::STEREO_MODE_VERTICAL_SPLIT)
    .value("STEREO_MODE_HORIZONTAL_INTERLACE", av::osg::viewer::GraphicsWindow::STEREO_MODE_HORIZONTAL_INTERLACE)
    .value("STEREO_MODE_VERTIVAL_INTERLACE", av::osg::viewer::GraphicsWindow::STEREO_MODE_VERTIVAL_INTERLACE)
    .value("STEREO_MODE_CHECKERBOARD", av::osg::viewer::GraphicsWindow::STEREO_MODE_CHECKERBOARD)
    .value("STEREO_MODE_NONE", av::osg::viewer::GraphicsWindow::STEREO_MODE_NONE)
    .export_values()
    ;
}
