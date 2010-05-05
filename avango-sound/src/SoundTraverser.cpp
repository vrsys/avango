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

#include <avango/sound/SoundTraverser.h>
#include <avango/sound/SoundSource.h>
#include <avango/osg/MatrixTransform.h>
#include <avango/osg/Group.h>
#include <avango/Application.h>
#include <boost/bind.hpp>
#include <iostream>

AV_FC_DEFINE(av::sound::SoundTraverser);

av::sound::SoundTraverser::SoundTraverser()
: mCheckTraversal(false), mHaveCallbackHandle(false)
{
  AV_FC_ADD_FIELD(Renderers, std::vector<Link<av::sound::SoundRenderer> >());
  AV_FC_ADD_FIELD(RootNode, Link<av::osg::Node>());
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
    AV_FC_INIT(av::FieldContainer,
               av::sound::SoundTraverser,
               true);
  }
}


void
av::sound::SoundTraverser::traverse(av::Link<av::osg::Node> rootNode, ::osg::Matrix absolutTransform ) const
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
  if (rootNode->getTypeId().isOfType(::av::osg::MatrixTransform::getClassTypeId())) {
    Link<av::osg::MatrixTransform> transform(dynamic_cast<av::osg::MatrixTransform*>(rootNode.getPtr()));
    AV_ASSERT(transform.isValid());
    absolutTransform *= transform->Matrix.getValue();
  }
  if (rootNode->getTypeId().isOfType(::av::osg::Group::getClassTypeId())) {
    Link<av::osg::Group> group(dynamic_cast<av::osg::Group*>(rootNode.getPtr()));
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
  traverse(RootNode.getValue(), ::osg::Matrix::identity());
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
      mCallbackHandle = app.addRenderCallback(boost::bind(&SoundTraverser::render, this));
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


