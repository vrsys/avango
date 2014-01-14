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

#if !defined(AVANGO_OSG_CULLFACE_H)
#define AVANGO_OSG_CULLFACE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/CullFace>
#include <avango/osg/Fields.h>
#include <avango/osg/StateAttribute.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::CullFace OsgCullFace;

    /**
     * Wrapper for ::osg::CullFace
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL CullFace : public StateAttribute
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::CullFace is created.
       * Otherwise, the given ::osg::CullFace is used.
       */
      CullFace(OsgCullFace* osgcullface = new OsgCullFace());
      // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~CullFace();

    public:

      SFInt Mode;

      /**
       * Get the wrapped ::osg::CullFace.
       */
      ::osg::CullFace* getOsgCullFace() const;

    protected:

      virtual void getModeCB(const av::SFInt::GetValueEvent& event);
      virtual void setModeCB(const av::SFInt::SetValueEvent& event);

    private:

      ::osg::CullFace *mOsgCullFace;
    };

    typedef SingleField<Link<CullFace> > SFCullFace;
    typedef MultiField<Link<CullFace> > MFCullFace;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::CullFace> >;
  template class AV_OSG_DLL MultiField<Link<osg::CullFace> >;
#endif

}

#endif
