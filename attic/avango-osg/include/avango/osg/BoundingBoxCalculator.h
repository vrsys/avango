// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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
* AVANGO is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#ifndef BOUNDINGBOXCALCULATOR_H_
#define BOUNDINGBOXCALCULATOR_H_

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/osg/Group.h>
#include <avango/osg/BoundingSphere.h>

namespace av
{
  namespace osg
  {
    /**
     * Wrapper for ::osg::BoundingBoxCalculator
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL BoundingBoxCalculator : public Group
    {
      AV_FC_DECLARE();

    public:

      BoundingBoxCalculator();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~BoundingBoxCalculator();

    public:
      /* virtual */ void evaluate();

      // Fudge factor, which will be used to consider the center and the radius of the bounding box to be equal
      SFFloat Fudge;

      //Encapsulating the current bounding sphere of this node (including all of its children)
      SFBoundingSphere BoundingSphere;

    private:
      ::osg::Vec3 mCenter;
      float mRadius;

      float mFudge;

    };

    typedef SingleField<Link<BoundingBoxCalculator> > SFBoundingBoxCalculator;
    typedef MultiField<Link<BoundingBoxCalculator> > MFBoundingBoxCalculator;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::BoundingBoxCalculator> >;
  template class AV_OSG_DLL MultiField<Link<osg::BoundingBoxCalculator> >;
#endif

}

#endif /* BOUNDINGBOXCALCULATOR_H_ */
