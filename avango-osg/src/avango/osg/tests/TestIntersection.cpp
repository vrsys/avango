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

#include <avango/osg/Sphere.h>
#include <avango/osg/Group.h>
#include <avango/osg/LineSegmentIntersector.h>
#include <avango/UnitTest++/UnitTest++.h>

namespace
{
  TEST(LineSegmentIntersection)
  {
    av::Link<av::osg::Sphere> sphere = new av::osg::Sphere;
    av::Link<av::osg::LineSegmentIntersector> lsi = new av::osg::LineSegmentIntersector;

    lsi->RootNode.setValue(&*sphere);
    lsi->evaluate();

    CHECK(lsi->HitCount.getValue() > 0);
  }

  TEST(HitNodePath)
  {
    av::Link<av::osg::Group> root = new av::osg::Group;
    av::Link<av::osg::Group> group1 = new av::osg::Group;
    av::Link<av::osg::Group> group2 = new av::osg::Group;
    av::Link<av::osg::Sphere> sphere = new av::osg::Sphere;
    av::Link<av::osg::LineSegmentIntersector> lsi = new av::osg::LineSegmentIntersector;

    root->Children.add1Value(group1.getPtr());
    group1->Children.add1Value(group2.getPtr());
    group2->Children.add1Value(sphere.getPtr());

    lsi->RootNode.setValue(root.getPtr());
    lsi->evaluate();

    // should be 3 excluding the root node
    CHECK_EQUAL(3u, lsi->HitNodePath.getValue().size());
  }

  TEST(ConvenienceMethod)
  {
    av::Link<av::osg::Node> sphere = new av::osg::Sphere;
    ::osg::Vec3 hitpoint = av::osg::lineIntersect(::osg::Vec3(0,0,0), ::osg::Vec3(0,10,0), sphere);
    CHECK(hitpoint != ::osg::Vec3(0,10,0));
  }
}
