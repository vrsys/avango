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

#include <avango/sound/SoundTraverser.h>
#include <avango/sound/SoundSource.h>
#include <avango/gua/scenegraph/TransformNode.hpp>
//#include <avango/gua/scenegraph/Group.hpp>
#include <avango/Application.h>
#include <functional>
#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::sound::SoundTraverser"));
}

AV_FC_DEFINE(av::sound::SoundTraverser);

AV_FIELD_DEFINE(av::sound::SFSoundTraverser);
AV_FIELD_DEFINE(av::sound::MFSoundTraverser);

av::sound::SoundTraverser::SoundTraverser()
: mCheckTraversal(false), mHaveCallbackHandle(false)
{
  AV_FC_ADD_FIELD(Renderers, std::vector<Link<av::sound::SoundRenderer> >());
  AV_FC_ADD_FIELD(RootNode, Link<av::gua::Node>());
  AV_FC_ADD_FIELD(Traverse, false);

  getTypeId().setDistributable(false);
}


av::sound::SoundTraverser::~SoundTraverser()
{
  const SoundRenderers& renderers = Renderers.getValue();
  for (SoundRenderers::const_iterator each_renderer = renderers.begin();
       each_renderer != renderers.end(); ++each_renderer)
  {
    (*each_renderer)->resetRenderer();
  }

  std::cerr << "~SoundTraverser() called" << std::endl;
  if (mHaveCallbackHandle)
  {
    Application& app = ApplicationInstance::get();
    app.removeCallback(mCallbackHandle);
  }
}


/*static*/ void
av::sound::SoundTraverser::initClass()
{
  if(!isTypeInitialized())
  {
    FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::sound::SoundTraverser, true);

    SFSoundTraverser::initClass("av::sound::SFSoundTraverser", "av::Field");
    MFSoundTraverser::initClass("av::sound::MFSoundTraverser", "av::Field");
  }
}


void
av::sound::SoundTraverser::traverse(av::Link<av::gua::Node> rootNode, ::gua::math::mat4 absolutTransform ) const
{

  if (rootNode->getTypeId().isOfType(::av::sound::SoundSource::getClassTypeId())) {
    Link<av::sound::SoundSource> sound_source(dynamic_cast<av::sound::SoundSource*>(rootNode.getPtr()));
    AV_ASSERT(sound_source.isValid());
    const SoundRenderers& renderers = Renderers.getValue();
    for (SoundRenderers::const_iterator each_renderer = renderers.begin();
          each_renderer != renderers.end(); ++each_renderer)
    {
      (*each_renderer)->updateSoundSourcePosition(sound_source, absolutTransform);
    }
  }
  if (rootNode->getTypeId().isOfType(::av::gua::TransformNode::getClassTypeId())) {
    Link<av::gua::TransformNode> transform(dynamic_cast<av::gua::TransformNode*>(rootNode.getPtr()));
    AV_ASSERT(transform.isValid());
    absolutTransform *= transform->Transform.getValue();
  }
  //if (rootNode->getTypeId().isOfType(::av::osg::Group::getClassTypeId())) {
  //  Link<av::osg::Group> group(dynamic_cast<av::osg::Group*>(rootNode.getPtr()));
  if (rootNode->getTypeId().isOfType(::av::gua::Node::getClassTypeId())) {
    Link<av::gua::Node> group(dynamic_cast<av::gua::Node*>(rootNode.getPtr()));
    AV_ASSERT(group.isValid());
    const ChildNodes& children = group->Children.getValue();
    for (ChildNodes::const_iterator each_child = children.begin();
          each_child != children.end(); ++each_child)
    {
      traverse(*each_child, absolutTransform);
    }
  }
}

void
av::sound::SoundTraverser::fieldHasChanged(const av::Field& field)
{
  if (&field == &Traverse) {
    mCheckTraversal = true;
  }
}

void
av::sound::SoundTraverser::render()
{
  if (!RootNode.getValue().isValid())
  {
    return;
  }
  traverse(RootNode.getValue(), ::gua::math::mat4::identity());
}

void
av::sound::SoundTraverser::evaluate()
{
  if (mCheckTraversal) {
    Application& app = ApplicationInstance::get();
    if (Traverse.getValue())
    {
      if (mHaveCallbackHandle) {
        return;
      }
      mCallbackHandle = app.addRenderCallback(std::bind(&SoundTraverser::render, this));
      mHaveCallbackHandle = true;
    } else
    {
      if (!mHaveCallbackHandle) {
        return;
      }
      app.removeCallback(mCallbackHandle);
      mHaveCallbackHandle = false;
    }
    mCheckTraversal = false;
  }
}


