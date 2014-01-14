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

#if !defined(AVANGO_OSG_BOUNDINGSPHERE_H)
#define AVANGO_OSG_BOUNDINGSPHERE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Fields.h>
#include <avango/FieldContainer.h>

namespace av
{
  namespace osg
  {
    /**
     * \ingroup av_osg
     * Something that can be added as a leaf node to a scene graph in order
     * to compute the absolute transform of the path to the root. Moreover,
     * the evaluation of all nodes of that path is triggered from here.
     *
     */
    class AV_OSG_DLL BoundingSphere : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      BoundingSphere();

      /**
       * Center of the bounding sphere
       */
      SFVec3 Center;

      /**
       * Radius of the bounding sphere
       */
      SFFloat Radius;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~BoundingSphere();


    };

    typedef SingleField<Link<BoundingSphere> > SFBoundingSphere;
    typedef MultiField<Link<BoundingSphere> > MFBoundingSphere;

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::BoundingSphere> >;
  template class AV_OSG_DLL MultiField<Link<osg::BoundingSphere> >;
#endif

} // namespace av

#endif
