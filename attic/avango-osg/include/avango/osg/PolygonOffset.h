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

#if !defined(AVANGO_OSG_POLYGONOFFSET_H)
#define AVANGO_OSG_POLYGONOFFSET_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/PolygonOffset>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::PolygonOffset OsgPolygonOffset;

    /**
     * Wrapper for ::osg::PolygonOffset
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL PolygonOffset : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::PolygonOffset is created.
       * Otherwise, the given ::osg::PolygonOffset is used.
       */
      PolygonOffset(OsgPolygonOffset* osgpolygonoffset = new OsgPolygonOffset());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PolygonOffset();

    public:

      SFFloat Factor;
      SFFloat Units;

      /**
       * Get the wrapped ::osg::PolygonOffset.
       */
      ::osg::PolygonOffset* getOsgPolygonOffset() const;

    protected:

      virtual void getFactorCB(const av::SFFloat::GetValueEvent& event);
      virtual void setFactorCB(const av::SFFloat::SetValueEvent& event);
      virtual void getUnitsCB(const av::SFFloat::GetValueEvent& event);
      virtual void setUnitsCB(const av::SFFloat::SetValueEvent& event);

    private:

      ::osg::PolygonOffset *mOsgPolygonOffset;
    };

    typedef SingleField<Link<PolygonOffset> > SFPolygonOffset;
    typedef MultiField<Link<PolygonOffset> > MFPolygonOffset;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::PolygonOffset> >;
  template class AV_OSG_DLL MultiField<Link<osg::PolygonOffset> >;
#endif

}

#endif
