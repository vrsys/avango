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

#if !defined(AVANGO_OSG_GEODE_H)
#define AVANGO_OSG_GEODE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Geode>
#include <avango/osg/Node.h>
#include <avango/osg/Drawable.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Geode OsgGeode;

    /**
     * Wrapper for ::osg::Geode
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Geode : public Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Geode is created.
       * Otherwise, the given ::osg::Geode is used.
       */
      Geode(OsgGeode* osggeode = new OsgGeode());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      ~Geode();

    public:

      MFDrawable Drawables;

      /**
       * Get the wrapped ::osg::Geode.
       */
      ::osg::Geode* getOsgGeode() const;
      const ::osg::BoundingBox getBoundingBox() const { return mOsgGeode->getBoundingBox(); }

    private:
      Geode(const Geode&);
      Geode& operator=(const Geode&);

      ::osg::Geode *mOsgGeode;

    protected:

      virtual void getDrawablesCB(const av::osg::MFDrawable::GetValueEvent& event);
      virtual void setDrawablesCB(const av::osg::MFDrawable::SetValueEvent& event);

    };

    typedef SingleField<Link<Geode> > SFGeode;
    typedef MultiField<Link<Geode> > MFGeode;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Geode> >;
  template class AV_OSG_DLL MultiField<Link<osg::Geode> >;
#endif

}

#endif
