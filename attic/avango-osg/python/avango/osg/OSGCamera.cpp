// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/Camera.h>
#include "OSGCamera.h"

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

namespace
{
  // TODO this should be defined globally and used as field type

  enum ClearMask
  {
    COLOR_BUFFER_BIT=GL_COLOR_BUFFER_BIT,
    DEPTH_BUFFER_BIT=GL_DEPTH_BUFFER_BIT,
    ACCUM_BUFFER_BIT=GL_ACCUM_BUFFER_BIT,
    STENCIL_BUFFER_BIT=GL_STENCIL_BUFFER_BIT
  };
}

void init_ClearMask(void)
{
  // TODO this should be defined globally and used as field type

  enum_<ClearMask>("ClearMask")
    .value("GL_COLOR_BUFFER_BIT", COLOR_BUFFER_BIT)
    .value("GL_DEPTH_BUFFER_BIT", DEPTH_BUFFER_BIT)
    .value("GL_ACCUM_BUFFER_BIT", ACCUM_BUFFER_BIT)
    .value("GL_STENCIL_BUFFER_BIT", STENCIL_BUFFER_BIT)
    ;
}

void init_OSGCamera(void)
 {
  register_field<av::osg::SFCamera>("SFCamera");
  register_multifield<av::osg::MFCamera>("MFCamera");

  class_<av::osg::Camera, av::Link<av::osg::Camera>, bases<av::osg::Transform>, boost::noncopyable >(
      "Camera",
      "Low-level wrapper of osg::Camera. Mostly used for pre-render effects",
    no_init);

  init_ClearMask();

  enum_< ::osg::Camera::RenderOrder >("RenderOrder")
    .value("PRE_RENDER", ::osg::Camera::PRE_RENDER)
    .value("NESTED_RENDER", ::osg::Camera::NESTED_RENDER)
    .value("POST_RENDER", ::osg::Camera::POST_RENDER)
    ;

  enum_< ::osg::Camera::RenderTargetImplementation >("RenderTargetImplementation")
    .value("FRAME_BUFFER_OBJECT", ::osg::Camera::FRAME_BUFFER_OBJECT)
    .value("PIXEL_BUFFER_RTT", ::osg::Camera::PIXEL_BUFFER_RTT)
    .value("PIXEL_BUFFER", ::osg::Camera::PIXEL_BUFFER)
    .value("FRAME_BUFFER", ::osg::Camera::FRAME_BUFFER)
    .value("SEPERATE_WINDOW", ::osg::Camera::SEPERATE_WINDOW)
     ;
 }
