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

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/osg/Node.h>
#include "OSGNode.h"

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/MatrixTransform>
#include <osg/Billboard>


using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }


namespace {
  class CalculateBoundingBox : public ::osg::NodeVisitor
  {
  public:
    CalculateBoundingBox() :
      NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
    {
      m_transformMatrix.makeIdentity();
    }

    virtual ~CalculateBoundingBox()
    {
    }

    virtual
    void apply(::osg::Geode &geode)
    {
      ::osg::BoundingBox bbox;
      for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
      {
        bbox.expandBy(geode.getDrawable(i)->getBound());
      }
      ::osg::BoundingBox bboxTrans;
      for (unsigned int i = 0; i < 8; ++i)
      {
        ::osg::Vec3 xvec = bbox.corner(i) * m_transformMatrix;
        bboxTrans.expandBy(xvec);
      }
      m_boundingBox.expandBy(bboxTrans);
      traverse(geode);
    }

    virtual
    void apply(::osg::MatrixTransform &node)
    {
      m_transformMatrix *= node.getMatrix();
      traverse(node);
    }

    virtual
    void apply(::osg::Billboard &node)
    {
      traverse(node);
    }

    ::osg::BoundingBox &getBoundBox()
    {
      return m_boundingBox;
    }

  protected:
    ::osg::BoundingBox m_boundingBox; // the overall resultant bounding box
    ::osg::Matrix m_transformMatrix; // the current transform matrix
  };


  osg::BoundingBox CalcBoundingBox(av::osg::Node* node)
  {
    CalculateBoundingBox bbox;
    node->getOsgNode()->accept( bbox );
    return bbox.getBoundBox();
  }

}




void init_OSGNode(void)
 {
  // wrapping osg::Node functionality
  register_field<av::osg::SFNode>("SFNode");
  register_multifield<av::osg::MFNode>("MFNode");
  class_<av::osg::Node, av::Link<av::osg::Node>, bases<av::osg::Object>, boost::noncopyable >("Node", "docstring", no_init)
    .def("get_bounding_sphere", &av::osg::Node::getBoundingSphere)
    .def("get_absolute_transform", &av::osg::Node::getAbsoluteTransform)
    ;

  def("calc_bounding_box", CalcBoundingBox);
 }
