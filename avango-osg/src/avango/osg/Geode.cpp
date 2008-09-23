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

#include <avango/osg/Geode.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Geode"));
}

AV_FC_DEFINE(av::osg::Geode);

AV_FIELD_DEFINE(av::osg::SFGeode);
AV_FIELD_DEFINE(av::osg::MFGeode);

av::osg::Geode::Geode(::osg::Geode* osggeode) :
  Node(osggeode),
  mOsgGeode(osggeode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Drawables,
                          boost::bind(&Geode::getDrawablesCB, this, _1),
                          boost::bind(&Geode::setDrawablesCB, this, _1));
}

av::osg::Geode::~Geode()
{}

void
av::osg::Geode::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::osg::Geode, true);

    SFGeode::initClass("av::osg::SFGeode", "av::Field");
    MFGeode::initClass("av::osg::MFGeode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

::osg::Geode*
av::osg::Geode::getOsgGeode() const
{
  return mOsgGeode;
}

/* virtual */ void
av::osg::Geode::getDrawablesCB(const av::osg::MFDrawable::GetValueEvent& event)
{
  av::osg::MFDrawable::ContainerType &drawables(*event.getValuePtr());

  drawables.clear();

  int num_drawables = mOsgGeode->getNumDrawables();
  drawables.reserve(num_drawables);

  for (int d = 0; d < num_drawables; ++d)
  {
    ::osg::Drawable *osg_drawable = mOsgGeode->getDrawable(d);

    if (osg_drawable != 0)
    {
      av::osg::Drawable *drawable = av::osg::get_from_osg_object<av::osg::Drawable>(osg_drawable);

      if (drawable != 0)
      {
        drawables.push_back(drawable);
      }
      else
      {
        logger.info() << "getDrawablesCB: "
                      << "found user data that doesn't reference an av::osg::Drawable.";
      }
    }
    else
    {
      logger.warn() << "getDrawableCB: null osg drawable found!";
    }
  }
}

/* virtual */ void
av::osg::Geode::setDrawablesCB(const av::osg::MFDrawable::SetValueEvent& event)
{
  int num_drawables = mOsgGeode->getNumDrawables();

  // Remove (but do not delete) all drawables that are av::osg::Drawable.
  // Drawables of type ::osg::Drawable are not affected.

  for (int d = num_drawables-1; d >= 0 ; --d)
  {
    ::osg::Drawable *osg_drawable = mOsgGeode->getDrawable(d);

    if (osg_drawable != 0)
    {
      ObjectLink *av_drawable_link = dynamic_cast<ObjectLink*>(osg_drawable->getUserData());
      if (av_drawable_link != 0 && av_drawable_link->getObject() != 0)
      {
        mOsgGeode->removeDrawable(osg_drawable);
      }
    }
    else
    {
      logger.warn() << "setDrawableCB: null osg drawable found!";
    }
  }

  // Add new drawables set

  const av::osg::MFDrawable::ContainerType &drawables(event.getValue());

  std::vector<av::Link<av::osg::Drawable> >::const_iterator d_it;
  for (d_it = drawables.begin(); d_it != drawables.end(); ++d_it)
  {
    if (d_it->isValid() && d_it->getPtr()->getOsgDrawable() != 0)
    {
      if (!mOsgGeode->addDrawable(d_it->getPtr()->getOsgDrawable()))
        logger.warn() << "setDrawableCB: couldn't insert drawable!";
    }
    else
    {
      logger.warn() << "setDrawableCB: invalid drawable to add found!";
    }
  }
}
