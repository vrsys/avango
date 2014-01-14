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

#if !defined(AVANGO_OSG_LINESEGMENTINTERSECTOR_H)
#define AVANGO_OSG_LINESEGMENTINTERSECTOR_H

#include <osgUtil/LineSegmentIntersector>

#include <avango/osg/Fields.h>
#include <avango/osg/Node.h>

#include <avango/FieldContainer.h>

namespace av
{
  namespace osg
  {
    /**
     * Convenience method for checking line intersection, returns hit point in
     * global coordinates, if no intersection was found given end point is returned.
     */
    AV_OSG_DLL ::osg::Vec3 lineIntersect(::osg::Vec3 start, ::osg::Vec3 end, Link<Node> node);


    /**
     * LineSegmentIntersector wraps functionality of ::osg::LineSegmentIntersector
     * and ::osg::IntersectionVisitor.
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL LineSegmentIntersector : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:
      /**
       * Constructor
       */
      LineSegmentIntersector();

      /**
       * Entry scenegraph node from which the intersection traversal should start.
       */
      SFNode RootNode;

      /**
       * Line segment transformation given in global coordinates (default: identity).
       */
      SFMatrix LineMatrix;

      /**
       * Length of line segment used for intersection (default: 999)
       */
      SFFloat LineLength;

      /**
       * Pointing direction given in transformation of 'LineMatrix' (default: Vec3(0,1,0))
       */
      SFVec3 LineDirection;

      /**
       * Output field: Specifies number of intersections found.
       */
      SFInt HitCount;

      /**
       * Output field: Node path from first hit to root node.
       */
      MFNode HitNodePath;

      /**
       * Output field: Global coordinates of first hit point.
       */
      SFVec3   HitPoint;

      /**
       * Ouput field: Normal at 'HitPoint' in global coordinates.
       */
      SFVec3   HitNormal;


      /* virtual */ void evaluate();
      /* virtual */ void fieldHasChanged(const av::Field& field);

    protected:
      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LineSegmentIntersector();

    private:

      bool mIntersect;

      std::vector<av::Link<av::osg::Node> > getAvangoNodePath(::osgUtil::LineSegmentIntersector::Intersections i);

    };
  }
}

#endif // #if !defined(AVANGO_OSG_ROOTSERVICE_H)

