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

#include <avango/osg/viewer/Init.h>

#include <avango/Logger.h>

#include <avango/osg/viewer/Camera.h>
#include <avango/osg/viewer/CompositeViewer.h>
#include <avango/osg/viewer/GraphicsWindow.h>
#include <avango/osg/viewer/EventFields.h>
#include <avango/osg/viewer/View.h>
#include <avango/osg/viewer/Viewer.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::osg::viewer::Init);

/* static */ void
av::osg::viewer::Init::initClass()
{
  if (!isTypeInitialized())
  {
#if !defined(_WIN32)
    // OSG should not change near and far clipping planes,
    // because this could clip away geometry from draw callbacks
    ::setenv("OSG_COMPUTE_NEAR_FAR_MODE", "DO_NOT_COMPUTE_NEAR_FAR", 1);
#endif

    av::osg::viewer::Camera::initClass();
    av::osg::viewer::CompositeViewer::initClass();
    av::osg::viewer::GraphicsWindow::initClass();
    av::osg::viewer::EventFields::initClass();
    av::osg::viewer::View::initClass();
    av::osg::viewer::Viewer::initClass();

    AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::osg::viewer::Init", true);
  }
}
