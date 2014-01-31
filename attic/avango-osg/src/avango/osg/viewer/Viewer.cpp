
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

#include <avango/osg/viewer/Viewer.h>

#include <boost/bind.hpp>

#include <avango/Application.h>
#include <avango/Logger.h>


namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::Viewer"));
}

AV_FC_DEFINE(av::osg::viewer::Viewer);

AV_FIELD_DEFINE(av::osg::viewer::SFViewer);

av::osg::viewer::Viewer::Viewer() :
  View(new ::osgViewer::Viewer),
  mOsgViewer(dynamic_cast< ::osgViewer::Viewer*>(getOsgView()))
{
  AV_FC_ADD_ADAPTOR_FIELD(ThreadingModel,
                          boost::bind(&av::osg::viewer::Viewer::getThreadingModelCB, this, _1),
                          boost::bind(&av::osg::viewer::Viewer::setThreadingModelCB, this, _1));

  mRenderCallbackHandle = av::ApplicationInstance::get().
    addRenderCallback(boost::bind(&av::osg::viewer::Viewer::renderCB, this));
}

/* virtual */
av::osg::viewer::Viewer::~Viewer()
{
  av::ApplicationInstance::get().removeCallback(mRenderCallbackHandle);
}

/* static */ void
av::osg::viewer::Viewer::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::viewer::View::initClass();

    AV_FC_INIT(av::osg::viewer::View, av::osg::viewer::Viewer, false);

    SFViewer::initClass("av::osg::viewer::SFViewer", "av::Field");
  }
}

::osgViewer::Viewer*
av::osg::viewer::Viewer::getOsgViewer() const
{
  return mOsgViewer;
}

void
av::osg::viewer::Viewer::frame()
{
  av::ApplicationInstance::get().evaluate();
}

bool
av::osg::viewer::Viewer::run()
{
  return av::ApplicationInstance::get().start();
}

bool
av::osg::viewer::Viewer::done()
{
  return av::ApplicationInstance::get().stop();
}

void
av::osg::viewer::Viewer::renderCB()
{
  if (mOsgViewer->done())
  {
    mOsgViewer->setDone(false);
    av::ApplicationInstance::get().stop();
  }
  else if (mOsgViewer->getCamera() != 0 && mOsgViewer->getSceneData() != 0)
    mOsgViewer->frame();
}

/* virtual */ void
av::osg::viewer::Viewer::getThreadingModelCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgViewer->getThreadingModel();
}

/* virtual */ void
av::osg::viewer::Viewer::setThreadingModelCB(const SFInt::SetValueEvent& event)
{
  mOsgViewer->
    setThreadingModel(static_cast< ::osgViewer::ViewerBase::ThreadingModel>(event.getValue()));
}
