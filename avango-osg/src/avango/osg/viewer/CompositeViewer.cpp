
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

#include <avango/osg/viewer/CompositeViewer.h>

#include <boost/bind.hpp>

#include <avango/Application.h>
#include <avango/Logger.h>


namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::CompositeViewer"));

  inline bool
  find(const av::osg::viewer::MFView::ContainerType& views,
       const av::osg::viewer::MFView::ValueType& view)
  {
    return (std::find(views.begin(), views.end(), view) != views.end());
  }
}


AV_FC_DEFINE(av::osg::viewer::CompositeViewer);

AV_FIELD_DEFINE(av::osg::viewer::SFCompositeViewer);

av::osg::viewer::CompositeViewer::CompositeViewer() :
  Object(new ::osgViewer::CompositeViewer),
  mOsgCompositeViewer(dynamic_cast< ::osgViewer::CompositeViewer*>(getOsgObject()))
{
  AV_FC_ADD_FIELD(Views, MFView::ContainerType());

  AV_FC_ADD_ADAPTOR_FIELD(ThreadingModel,
    boost::bind(&av::osg::viewer::CompositeViewer::getThreadingModelCB, this, _1),
    boost::bind(&av::osg::viewer::CompositeViewer::setThreadingModelCB, this, _1));

  mRenderCallbackHandle = av::ApplicationInstance::get().
    addRenderCallback(boost::bind(&av::osg::viewer::CompositeViewer::renderCB, this));
}

/* virtual */
av::osg::viewer::CompositeViewer::~CompositeViewer()
{
  av::ApplicationInstance::get().removeCallback(mRenderCallbackHandle);
}

/* static */ void
av::osg::viewer::CompositeViewer::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();
    av::osg::viewer::View::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::viewer::CompositeViewer, false);

    SFCompositeViewer::initClass("av::osg::viewer::SFCompositeViewer", "av::Field");
  }
}

/* virtual */ void
av::osg::viewer::CompositeViewer::fieldHasChangedLocalSideEffect(const Field& field)
{
  av::osg::Object::fieldHasChangedLocalSideEffect(field);

  if (&field == &Views)
  {
    const MFView::ContainerType &views = Views.getValue();

    for (MFView::ContainerType::const_iterator last_view = mLastViews.begin();
         last_view != mLastViews.end(); ++last_view)
    {
      if (!find(views, *last_view))
        mOsgCompositeViewer->removeView((*last_view)->getOsgView());
    }

    for (MFView::ContainerType::const_iterator view = views.begin();
         view != views.end(); ++view)
    {
      if (!find(mLastViews, *view))
        mOsgCompositeViewer->addView((*view)->getOsgView());
    }

    mLastViews = views;
  }
}

::osgViewer::CompositeViewer*
av::osg::viewer::CompositeViewer::getOsgCompositeViewer() const
{
  return mOsgCompositeViewer;
}

void
av::osg::viewer::CompositeViewer::frame()
{
  av::ApplicationInstance::get().evaluate();
}

bool
av::osg::viewer::CompositeViewer::run()
{
  return av::ApplicationInstance::get().start();
}

bool
av::osg::viewer::CompositeViewer::done()
{
  return av::ApplicationInstance::get().stop();
}

void
av::osg::viewer::CompositeViewer::renderCB()
{
  if (mOsgCompositeViewer->done())
  {
    mOsgCompositeViewer->setDone(false);
    av::ApplicationInstance::get().stop();
  }
  else if (mOsgCompositeViewer->getNumViews() != 0u)
    mOsgCompositeViewer->frame();
}

/* virtual */ void
av::osg::viewer::CompositeViewer::getThreadingModelCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgCompositeViewer->getThreadingModel();
}

/* virtual */ void
av::osg::viewer::CompositeViewer::setThreadingModelCB(const SFInt::SetValueEvent& event)
{
  mOsgCompositeViewer->
    setThreadingModel(static_cast< ::osgViewer::ViewerBase::ThreadingModel>(event.getValue()));
}
