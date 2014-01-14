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

#include <avango/osg/LayerGeode.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <osg/GL>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LayerGeode"));
}

AV_FC_DEFINE(av::osg::LayerGeode);

AV_FIELD_DEFINE(av::osg::SFLayerGeode);
AV_FIELD_DEFINE(av::osg::MFLayerGeode);

av::osg::LayerGeode::LayerGeode(::osg::Geode* osggeode) :
  Geode(osggeode),
  mLayerModeChanged(false),
  mDrawablesChanged(false)
{
  AV_FC_ADD_FIELD(LayerMode, STENCIL);
}

av::osg::LayerGeode::~LayerGeode()
{}

void
av::osg::LayerGeode::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::osg::LayerGeode, true);

    SFLayerGeode::initClass("av::osg::SFLayerGeode", "av::Field");
    MFLayerGeode::initClass("av::osg::MFLayerGeode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::LayerGeode::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  Node::fieldHasChangedLocalSideEffect(field);

  if (&field == &LayerMode)
  {
    mLayerModeChanged = true;
  }
}

/* virtual */ void
av::osg::LayerGeode::evaluateLocalSideEffect()
{
  Node::evaluateLocalSideEffect();

  if (mLayerModeChanged || mDrawablesChanged)
  {
    updateDrawCallbacks();
    mLayerModeChanged = false;
    mDrawablesChanged = false;
  }
}

/* virtual */ void
av::osg::LayerGeode::setDrawablesCB(const av::osg::MFDrawable::SetValueEvent& event)
{
  Geode::setDrawablesCB(event);
  mDrawablesChanged = true;
}

void
av::osg::LayerGeode::updateDrawCallbacks()
{
  const ::osg::Geode::DrawableList& drawablelist = getOsgGeode()->getDrawableList();
  if (drawablelist.size()>1 && LayerMode.getValue()!=OFF)
  {
    drawablelist[0]->setDrawCallback(new DrawCallbackBaseLayer(this,LayerMode.getValue()));
    drawablelist[0]->setUseDisplayList(false);
    for (::osg::Geode::DrawableList::const_iterator itr=drawablelist.begin()+1; itr!=drawablelist.end(); ++itr)
    {
      (*itr)->setDrawCallback(new DrawCallbackDisableDraw());
    }
  }
  else
  {
    for (::osg::Geode::DrawableList::const_iterator itr=drawablelist.begin(); itr!=drawablelist.end(); ++itr)
    {
      (*itr)->setDrawCallback(0);
    }
  }
}

// implementation of DrawCallbackBaseLayer

av::osg::LayerGeode::DrawCallbackBaseLayer::DrawCallbackBaseLayer(av::osg::LayerGeode* geode, int mode) :
  mGeode(geode->getOsgGeode()),
  mMode(mode)
{}

/* virtual */ void
av::osg::LayerGeode::DrawCallbackBaseLayer::drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable*) const
{
  switch(mMode)
  {
    case STENCIL:
      drawWithStencil(renderInfo);
      break;
    case NO_DEPTH_TEST:
      drawWithNoDepthTest(renderInfo);
      break;
    default:
      drawWithStencil(renderInfo);
  }
}

void
av::osg::LayerGeode::DrawCallbackBaseLayer::drawWithStencil(::osg::RenderInfo& renderInfo) const
{
  const ::osg::Geode::DrawableList& drawablelist = mGeode->getDrawableList();

  // get state from renderInfo
  ::osg::ref_ptr< ::osg::State> state = renderInfo.getState();

  // clear stencil buffer, enable depth and stencil test, write 1 to stencil buffer
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // because the displaylist is switched off, we need to manually apply the StateSet
  if (mGeode->getStateSet() != 0) state->pushStateSet(mGeode->getStateSet());
  state->apply();

  // apply Drawable's stateset,
  // then draw base layer in frame buffer and stencil buffer
  state->apply(drawablelist[0]->getStateSet());
  drawablelist[0]->drawImplementation(renderInfo);
  state->apply();

  // disable depth test, write colors where stencil buffer is 1, keep stencil buffer unchanged
  glDisable(GL_DEPTH_TEST);
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  // then draw all decals in list order with their statesets applied
  for (::osg::Geode::DrawableList::const_iterator itr=drawablelist.begin()+1; itr!=drawablelist.end(); ++itr)
  {
    state->apply((*itr)->getStateSet());
    (*itr)->drawImplementation(renderInfo);
    state->apply();
  }

  if (mGeode->getStateSet() != 0) state->popStateSet();
  state->apply();

  glPopAttrib();
}

void
av::osg::LayerGeode::DrawCallbackBaseLayer::drawWithNoDepthTest(::osg::RenderInfo& renderInfo) const
{
  const ::osg::Geode::DrawableList& drawablelist = mGeode->getDrawableList();

  // get state from renderInfo
  ::osg::ref_ptr< ::osg::State> state = renderInfo.getState();

  // disable depth test
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  // because the displaylist is switched off, we need to manually apply the StateSet
  if (mGeode->getStateSet() != 0) state->pushStateSet(mGeode->getStateSet());
  state->apply();

  // draw all layers in list order with their statesets applied
  for (::osg::Geode::DrawableList::const_iterator itr=drawablelist.begin(); itr!=drawablelist.end(); ++itr)
  {
    state->apply((*itr)->getStateSet());
    (*itr)->drawImplementation(renderInfo);
    state->apply();
  }

  if (mGeode->getStateSet() != 0) state->popStateSet();
  state->apply();

  glPopAttrib();
}
