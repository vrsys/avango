
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

#include <avango/osg/viewer/View.h>

#include <boost/bind.hpp>

#include <avango/Logger.h>


namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::View"));
}

AV_FC_DEFINE(av::osg::viewer::View);

AV_FIELD_DEFINE(av::osg::viewer::SFView);
AV_FIELD_DEFINE(av::osg::viewer::MFView);

av::osg::viewer::View::View(::osgViewer::View* osgview) :
  av::osg::Object(osgview),
  mOsgView(osgview),
  mOsgStateSetManipulator(new ::osgGA::StateSetManipulator()),
  mStatsMode(0)
{
  AV_FC_ADD_ADAPTOR_FIELD(MasterCamera,
                          boost::bind(&av::osg::viewer::View::getMasterCameraCB, this, _1),
                          boost::bind(&av::osg::viewer::View::setMasterCameraCB, this, _1));

  AV_FC_ADD_FIELD(SlaveCameras, MFCamera::ContainerType());

  AV_FC_ADD_ADAPTOR_FIELD(Scene, boost::bind(&av::osg::viewer::View::getSceneCB, this, _1),
                                 boost::bind(&av::osg::viewer::View::setSceneCB, this, _1));

  AV_FC_ADD_FIELD(StatsMode, mStatsMode);

  mOsgView->addEventHandler(mOsgStateSetManipulator.get());
  mOsgView->addEventHandler(new ::osgViewer::ThreadingHandler);
  mOsgView->addEventHandler(new ::osgViewer::HelpHandler);
  mOsgStatsHandler = new ::osgViewer::StatsHandler;
  mOsgView->addEventHandler(mOsgStatsHandler.get());
  mOsgStatsToggle = new ::osgGA::GUIEventAdapter;
  mOsgStatsToggle->setEventType(::osgGA::GUIEventAdapter::KEYDOWN);
  mOsgStatsToggle->setKey(mOsgStatsHandler->getKeyEventTogglesOnScreenStats());
}

/* virtual */
av::osg::viewer::View::~View()
{}

/* static */ void
av::osg::viewer::View::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();
    av::osg::viewer::Camera::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::viewer::View, false);

    SFView::initClass("av::osg::viewer::SFView", "av::Field");
    MFView::initClass("av::osg::viewer::MFView", "av::Field");
  }
}

/* virtual */ void
av::osg::viewer::View::fieldHasChangedLocalSideEffect(const Field& field)
{
  av::osg::Object::fieldHasChangedLocalSideEffect(field);

  if (&field == &SlaveCameras)
  {
    // remove old slave cameras
    while (mOsgView->getNumSlaves() != 0u)
      mOsgView->removeSlave(0u);

    // add new slave cameras
    const MFCamera::ContainerType &slave_cameras = SlaveCameras.getValue();
    for (MFCamera::ContainerType::const_iterator camera = slave_cameras.begin();
         camera != slave_cameras.end(); ++camera)
    {
      if (camera->isValid())
      {
        ::osg::Camera *osg_camera = (*camera)->getOsgCamera();
        AV_ASSERT(osg_camera != 0);
        mOsgView->addSlave(osg_camera);
      }
      else
        AVANGO_LOG(logger, av::logging::WARN, "fieldHasChangedLocalSideEffect: invalid slave camera");
    }

    mOsgView->assignSceneDataToCameras();
  }
}

/* virtual */ void
av::osg::viewer::View::evaluateLocalSideEffect()
{
  const int newStatsMode = (StatsMode.getValue() % ::osgViewer::StatsHandler::LAST);
  mOsgStatsToggle->setHandled(false);
  while (mStatsMode != newStatsMode && mOsgStatsHandler->handle(*mOsgStatsToggle, *mOsgView))
  {
    mStatsMode = ((mStatsMode + 1) % ::osgViewer::StatsHandler::LAST);
    mOsgStatsToggle->setHandled(false);
  }
}

::osgViewer::View*
av::osg::viewer::View::getOsgView() const
{
  return mOsgView;
}

/* virtual */ void
av::osg::viewer::View::getMasterCameraCB(const SFCamera::GetValueEvent& event)
{
  *(event.getValuePtr()) = get_from_osg_object<Camera>(mOsgView->getCamera());
}

/* virtual */ void
av::osg::viewer::View::setMasterCameraCB(const SFCamera::SetValueEvent& event)
{
  ::osg::Camera *camera = (event.getValue().isValid() ? event.getValue()->getOsgCamera() : 0);
  mOsgView->setCamera(camera);
  mOsgView->assignSceneDataToCameras();
  mOsgStateSetManipulator->setStateSet(camera != 0 ? camera->getOrCreateStateSet() : 0);
}

/* virtual */ void
av::osg::viewer::View::getSceneCB(const SFNode::GetValueEvent& event)
{
  *(event.getValuePtr()) = get_from_osg_object<Node>(mOsgView->getSceneData());
}

/* virtual */ void
av::osg::viewer::View::setSceneCB(const SFNode::SetValueEvent& event)
{
  mOsgView->setSceneData(event.getValue().isValid() ? event.getValue()->getOsgNode() : 0);
}
