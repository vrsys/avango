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

#include <osgUtil/IntersectionVisitor>

#include <avango/osg/LineSegmentIntersector.h>
#include <avango/osg/ObjectLink.h>

#include <avango/logging/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::osg::LineSegmentIntersector"));
}

::osg::Vec3 av::osg::lineIntersect(::osg::Vec3 start, ::osg::Vec3 end, Link<Node> node)
{
    ::osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi = new ::osgUtil::LineSegmentIntersector(start, end);

    ::osgUtil::IntersectionVisitor iv(lsi.get());

    iv.apply(*node->getOsgNode());

    if(lsi->containsIntersections())
    {
        const osgUtil::LineSegmentIntersector::Intersection fi = lsi->getFirstIntersection();
        return fi.getWorldIntersectPoint();
    }
    else
        return end;
}

AV_FC_DEFINE(av::osg::LineSegmentIntersector);

av::osg::LineSegmentIntersector::LineSegmentIntersector() : mIntersect(false)
{
    AV_FC_ADD_FIELD(RootNode, 0);
    AV_FC_ADD_FIELD(LineMatrix, ::osg::Matrix());
    AV_FC_ADD_FIELD(LineLength, 999.);
    AV_FC_ADD_FIELD(LineDirection, ::osg::Vec3(0.0f, 1.0f, 0.0f));
    AV_FC_ADD_FIELD(HitCount, 0);
    AV_FC_ADD_FIELD(HitPoint, ::osg::Vec3(0.0f, 0.0f, 0.0f));
    AV_FC_ADD_FIELD(HitNormal, ::osg::Vec3(0.0f, 0.0f, 0.0f));
    AV_FC_ADD_FIELD(HitNodePath, std::vector<Link<Node>>());
}

/* virtual */
av::osg::LineSegmentIntersector::~LineSegmentIntersector() {}

/* static */ void av::osg::LineSegmentIntersector::initClass()
{
    if(!isTypeInitialized())
    {
        Object::initClass();
        AV_FC_INIT(av::Object, av::osg::LineSegmentIntersector, true);
    }
}

/* virtual */ void av::osg::LineSegmentIntersector::evaluate()
{
    LOG_TRACE(logger) << "evaluate()";

    if(mIntersect && RootNode.getValue().isValid())
    {
        LOG_TRACE(logger) << "evaluate(): do intersect with given values";

        ::osg::Vec3 start = LineMatrix.getValue().getTrans();
        ::osg::Vec3 end = LineMatrix.getValue().preMult(LineDirection.getValue() * LineLength.getValue());

        ::osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi = new ::osgUtil::LineSegmentIntersector(start, end);

        ::osgUtil::IntersectionVisitor iv(lsi.get());

        iv.apply(*RootNode.getValue()->getOsgNode());

        if(lsi->containsIntersections())
        {
            const osgUtil::LineSegmentIntersector::Intersections i = lsi->getIntersections();
            HitCount.setValue(i.size());
            HitNodePath.setValue(getAvangoNodePath(i));

            const osgUtil::LineSegmentIntersector::Intersection fi = lsi->getFirstIntersection();
            HitPoint.setValue(fi.getWorldIntersectPoint());
            HitNormal.setValue(fi.getWorldIntersectNormal());
        }
        else
        {
            HitCount.setValue(0);
            HitPoint.setValue(::osg::Vec3(0.0f, 0.0f, 0.0f));
            HitNormal.setValue(::osg::Vec3(0.0f, 0.0f, 0.0f));
            HitNodePath.setValue(std::vector<Link<Node>>());
        }

        mIntersect = false;
    }
}

/* virtual */ void av::osg::LineSegmentIntersector::fieldHasChanged(const av::Field& field)
{
    LOG_TRACE(logger) << "fieldHasChanged()";

    if(&field == &LineMatrix || &field == &RootNode || &field == &LineLength || &field == &LineDirection)
    {
        mIntersect = true;
    }
}

std::vector<av::Link<av::osg::Node>> av::osg::LineSegmentIntersector::getAvangoNodePath(::osgUtil::LineSegmentIntersector::Intersections i)
{
    // ToDo: Enhance method to return node path of all intersected nodes
    std::vector<av::Link<av::osg::Node>> nodepath;

    for(osgUtil::LineSegmentIntersector::Intersections::const_iterator iter = i.begin(); iter != i.end(); ++iter)
    {
        const ::osg::NodePath np = iter->nodePath;

        for(::osg::NodePath::const_iterator p_iter = np.begin(); p_iter != np.end(); ++p_iter)
        {
            av::osg::Node* av_node = av::osg::get_from_osg_object<av::osg::Node>(*p_iter);

            if(av_node != 0)
                nodepath.push_back(av_node);
        }

        break;
    }

    return nodepath;
}
