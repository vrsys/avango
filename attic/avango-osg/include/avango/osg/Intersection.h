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

#if !defined(AVANGO_OSG_INTERSECTION_H)
#define AVANGO_OSG_INTERSECTION_H

#include <osgUtil/LineSegmentIntersector>

#include <avango/osg/Fields.h>
#include <avango/osg/Node.h>

#include <avango/FieldContainer.h>

namespace av
{
namespace osg
{
/**
 * Intersection wraps functionality of ::osg::LineSegmentIntersector::Intersection
 *
 * Note that this object does not provide any information about the position of
 * the intersection point in the local coordinate system of the intersected node,
 * since this is most probably a osg object whose local coordinate system is not
 * the desired one. Therefore, you have to calculate the position yourself.
 * localPos = globalPos * InvAbsTransform (of the desired node)
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL Intersection : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor
     */
    Intersection();

    /**
     * Outputs node path from root node to intersected node.
     */
    MFNode NodePath;

    /**
     * Outputs intersection point in root node coordinate system.
     */
    SFVec3 Point;

    /**
     * Ouputs normal at intersection point in root node coordinate system.
     */
    SFVec3 Normal;

    /**
     * List containing the indices of the intersected geometry.
     */
    MFUInt IndexList;

    /**
     * Ratios between the vertices in IndexList to get the actual intersection point.
     */
    MFDouble RatioList;

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Intersection();
};

typedef SingleField<Link<Intersection>> SFIntersection;
typedef MultiField<Link<Intersection>> MFIntersection;
} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::Intersection>>;
template class AV_OSG_DLL MultiField<Link<osg::Intersection>>;
#endif

} // namespace av

#endif
