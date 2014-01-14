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

#include <avango/osg/LayerGroup.h>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <osg/GL>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LayerGroup"));
}

AV_FC_DEFINE(av::osg::LayerGroup);

AV_FIELD_DEFINE(av::osg::SFLayerGroup);
AV_FIELD_DEFINE(av::osg::MFLayerGroup);

av::osg::LayerGroup::LayerGroup(::osg::Group* osggroup) :
  Group(osggroup),
  mOsgGroup(osggroup),
  mLayerModeChanged(false),
  mChildrenChanged(false),
  mFirstDrawable(0)
{
  AV_FC_ADD_FIELD(LayerMode, STENCIL);
}

av::osg::LayerGroup::~LayerGroup()
{}

void
av::osg::LayerGroup::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::LayerGroup, true);

    SFLayerGroup::initClass("av::osg::SFLayerGroup", "av::Field");
    MFLayerGroup::initClass("av::osg::MFLayerGroup", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

/* virtual */ void
av::osg::LayerGroup::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  Group::fieldHasChangedLocalSideEffect(field);

  if (&field == &LayerMode)
  {
    mLayerModeChanged = true;
  }
}

/* virtual */ void
av::osg::LayerGroup::evaluateLocalSideEffect()
{
  Group::evaluateLocalSideEffect();

  if (mLayerModeChanged | mChildrenChanged)
  {
    mFirstDrawable = 0;
    updateDrawCallbacks(mOsgGroup, mFirstDrawable);
    mLayerModeChanged = false;
  }
}

/* virtual */ void
av::osg::LayerGroup::setChildrenCB(const av::osg::MFNode::SetValueEvent& event)
{
  Group::setChildrenCB(event);
  mChildrenChanged = true;
}

void
av::osg::LayerGroup::updateDrawCallbacks(::osg::Node* osg_node, ::osg::Drawable*& firstdrawable)
{
  // this method is a recursive traverser that follows children of osg::Nodes,
  // looks for osg::Geodes and updates the DrawCallbacks of all Drawables found.

  ::osg::Geode* osg_geode = dynamic_cast< ::osg::Geode*>(osg_node);
  if (osg_geode!=0)
  {
    int num_drawables = osg_geode->getNumDrawables();
    for (int i=0; i<num_drawables; ++i)
    {
      if (firstdrawable == 0)
      {
        firstdrawable = osg_geode->getDrawable(i);
        firstdrawable->setDrawCallback(new DrawCallbackBaseLayer(mOsgGroup, LayerMode.getValue(), firstdrawable));
        firstdrawable->setUseDisplayList(false);
      }
      else
      {
        osg_geode->getDrawable(i)->setDrawCallback(new DrawCallbackDisableDraw());
      }
    }
  }

  ::osg::Group* osg_group = osg_node->asGroup();
  if (osg_group != 0)
  {
    int num_children = osg_group->getNumChildren();
    for (int i=0; i<num_children; ++i)
    {
      updateDrawCallbacks(osg_group->getChild(i), firstdrawable);
    }
  }
}

av::osg::LayerGroup::DrawCallbackBaseLayer::DrawCallbackBaseLayer(::osg::Node* osg_node, int mode, ::osg::Drawable* firstdrawable) :
  mNode(osg_node),
  mMode(mode),
  mFirstDrawable(firstdrawable)
{}

/* virtual */ void
av::osg::LayerGroup::DrawCallbackBaseLayer::drawImplementation(::osg::RenderInfo& renderInfo, const ::osg::Drawable*) const
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  drawWithStencil(renderInfo, mNode, false);
  glPopAttrib();
}

bool
av::osg::LayerGroup::DrawCallbackBaseLayer::drawWithStencil(::osg::RenderInfo& renderInfo, ::osg::Node* osg_node, bool firstdrawablefound) const
{
  ::osg::ref_ptr< ::osg::State> state = renderInfo.getState();

  ::osg::Geode* osg_geode = dynamic_cast< ::osg::Geode*>(osg_node);
  if (osg_geode!=0)
  {
    int num_drawables = osg_geode->getNumDrawables();

    // because the displaylist is switched off, we need to manually apply the StateSet
    if (osg_geode->getStateSet() != 0) state->pushStateSet(osg_geode->getStateSet());
    state->apply();

    for (int i=0; i<num_drawables; ++i)
    {
      if (firstdrawablefound == false && osg_geode->getDrawable(i) == mFirstDrawable)
      {
        firstdrawablefound = true;

        // clear stencil buffer, enable depth and stencil test, write 1 to stencil buffer
        glClearStencil(0);
        glClear(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // apply Drawable's stateset,
        // then draw base layer in frame buffer and stencil buffer
        state->apply(mFirstDrawable->getStateSet());
        mFirstDrawable->drawImplementation(renderInfo);
        state->apply();

        // disable depth test, write colors where stencil buffer is 1, keep stencil buffer unchanged
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      }
      else
      {
        // draw all decals in list order with their statesets applied
        state->apply(osg_geode->getDrawable(i)->getStateSet());
        osg_geode->getDrawable(i)->drawImplementation(renderInfo);
        state->apply();
      }
    }

    if (osg_geode->getStateSet() != 0) state->popStateSet();
    state->apply();

    return firstdrawablefound;
  }

  ::osg::Group* osg_group = osg_node->asGroup();
  if (osg_group != 0)
  {
    int num_children = osg_group->getNumChildren();

    if (firstdrawablefound == true && osg_group->getStateSet() != 0) state->pushStateSet(osg_group->getStateSet());
    state->apply();

    for (int i=0; i<num_children; ++i)
    {
      firstdrawablefound = drawWithStencil(renderInfo, osg_group->getChild(i), firstdrawablefound);
    }

    if (firstdrawablefound == true && osg_group->getStateSet() != 0 && osg_group != mNode) state->popStateSet();
    state->apply();

    return firstdrawablefound;
  }
  return false;
}
