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

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/CameraAttachment.h>
#include <osg/Camera>
#include "OSGCameraAttachment.h"

using namespace boost::python;
using namespace av::python;

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

void init_OSGCameraAttachment(void)
 {
  register_field<av::osg::SFCameraAttachment>("SFCameraAttachment");
  register_multifield<av::osg::MFCameraAttachment>("MFCameraAttachment");
  class_<av::osg::CameraAttachment, av::Link<av::osg::CameraAttachment>, bases<av::osg::Object>, boost::noncopyable >(
      "CameraAttachment",
      "Helper class to store values for framebuffer attachment",
    no_init);

  // TODO this should be defined globally
  enum_<GLenum>("InternalFormat")
    .value("GL_RGBA", GL_RGBA)
    .value("GL_RGB", GL_RGB)
    ;

  enum_< ::osg::Camera::BufferComponent >("BufferComponent")
    .value("DEPTH_BUFFER", ::osg::Camera::DEPTH_BUFFER)
    .value("STENCIL_BUFFER", ::osg::Camera::STENCIL_BUFFER)
#if OPENSCENEGRAPH_MAJOR_VERSION >= 2 && OPENSCENEGRAPH_MINOR_VERSION >= 8
    .value("PACKED_DEPTH_STENCIL_BUFFER", ::osg::Camera::PACKED_DEPTH_STENCIL_BUFFER)
#endif
    .value("COLOR_BUFFER", ::osg::Camera::COLOR_BUFFER)
    .value("COLOR_BUFFER0", ::osg::Camera::COLOR_BUFFER0)
    .value("COLOR_BUFFER1", ::osg::Camera::COLOR_BUFFER1)
    .value("COLOR_BUFFER2", ::osg::Camera::COLOR_BUFFER2)
    .value("COLOR_BUFFER3", ::osg::Camera::COLOR_BUFFER3)
    .value("COLOR_BUFFER4", ::osg::Camera::COLOR_BUFFER4)
    .value("COLOR_BUFFER5", ::osg::Camera::COLOR_BUFFER5)
    .value("COLOR_BUFFER6", ::osg::Camera::COLOR_BUFFER6)
    .value("COLOR_BUFFER7", ::osg::Camera::COLOR_BUFFER7)
    .value("COLOR_BUFFER8", ::osg::Camera::COLOR_BUFFER8)
    .value("COLOR_BUFFER9", ::osg::Camera::COLOR_BUFFER9)
    .value("COLOR_BUFFER10", ::osg::Camera::COLOR_BUFFER10)
    .value("COLOR_BUFFER11", ::osg::Camera::COLOR_BUFFER11)
    .value("COLOR_BUFFER12", ::osg::Camera::COLOR_BUFFER12)
    .value("COLOR_BUFFER13", ::osg::Camera::COLOR_BUFFER13)
    .value("COLOR_BUFFER14", ::osg::Camera::COLOR_BUFFER14)
    .value("COLOR_BUFFER15", ::osg::Camera::COLOR_BUFFER15)
    ;
 }
