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

#if !defined(AVANGO_OSG_PROJECTION_H_)
#define AVANGO_OSG_PROJECTION_H_

/**
 * \file
 * \ingroup av_osg
 */
#include <osg/Projection>

#include <avango/osg/Fields.h>

#include <avango/osg/Group.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Projection OsgProjection;

    /**
     * Wrapper for ::osg::MatrixTransform
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Projection  : public Group
    {
      AV_FC_DECLARE();

      public:
        Projection(OsgProjection* osgprojection = new OsgProjection());

        /**
         * Matrix directly sets and gets the Matrix of the wrapped ::osg::Projection.
         */
        SFMatrix Matrix;

        /**
         * Get the wrapped ::osg::Projection.
         */
        ::osg::Projection* getOsgProjection() const;

      protected:
        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~Projection();

        virtual void getMatrixCB(const av::osg::SFMatrix::GetValueEvent& event);
        virtual void setMatrixCB(const av::osg::SFMatrix::SetValueEvent& event);


      private:

        ::osg::Projection *mOsgProjection;
    };

    typedef SingleField<Link<Projection> > SFProjection;
    typedef MultiField<Link<Projection> > MFProjection;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Projection> >;
  template class AV_OSG_DLL MultiField<Link<osg::Projection> >;
#endif
}

#endif /* AVANGO_OSG_PROJECTION_H_ */
