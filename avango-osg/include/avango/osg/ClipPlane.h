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


#ifndef CLIPPLANE_H_
#define CLIPPLANE_H_


/**
 * \file
 * \ingroup av_osg
 */

#include <osg/ClipPlane>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::ClipPlane OsgClipPlane;

    /**
     * Wrapper for ::osg::ClipPlane
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL ClipPlane : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::ClipPlane is created.
       * Otherwise, the given ::osg::ClipPlane is used.
       */
      ClipPlane(OsgClipPlane* osgClipPlane = new OsgClipPlane());

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ClipPlane();

    public:

      osg::SFVec4 Plane;

      /**
       * Get the wrapped ::osg::ClipPlane.
       */
      ::osg::ClipPlane* getOsgClipPlane() const;

    protected:

      virtual void getPlaneCB(const av::osg::SFVec4::GetValueEvent& event);
      virtual void setPlaneCB(const av::osg::SFVec4::SetValueEvent& event);

    private:

      ::osg::ClipPlane *mOsgClipPlane;
    };

    typedef SingleField<Link<ClipPlane> > SFClipPlane;
    typedef MultiField<Link<ClipPlane> > MFClipPlane;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::ClipPlane> >;
  template class AV_OSG_DLL MultiField<Link<osg::ClipPlane> >;
#endif

}
#endif /* CLIPPLANE_H_ */
