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

#if !defined(AVANGO_OSG_NODE_H)
#define AVANGO_OSG_NODE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <osg/Node>
#include <avango/osg/windows_specific_osg.h>
#include <avango/osg/Fields.h>
#include <avango/osg/Object.h>
#include <avango/osg/StateSet.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Node OsgNode;

    class Group;

    /**
     * Wrapper for ::osg::Node
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Node : public Object
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Node is created.
       * Otherwise, the given ::osg::Node is used.
       */
      Node(OsgNode* osgnode = new OsgNode()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Node();

    public:

      /**
       * Parents field is read only!
       */
      MultiField<Link<Group> > Parents;

      SFStateSet StateSet;

      /**
       * Get the wrapped ::osg::Node.
       */
      ::osg::Node* getOsgNode() const;

      const ::osg::BoundingSphere getBoundingSphere() const { return mOsgNode->getBound(); }

      /**
       * Computes the absolute transformation matrix to the root coordinate system.
       * Define the caller object, if called from an evaluate function,
       * to call evaluate on all nodes on the traversal to the root node.
       */
      ::osg::Matrix getAbsoluteTransform(av::FieldContainer* caller = 0) const;

    protected:

      virtual void getParentsCB(const MultiField<Link<Group> >::GetValueEvent& event);
      virtual void setParentsCB(const MultiField<Link<Group> >::SetValueEvent& event);

      virtual void getStateSetCB(const av::osg::SFStateSet::GetValueEvent& event);
      virtual void setStateSetCB(const av::osg::SFStateSet::SetValueEvent& event);

    private:

      ::osg::Node *mOsgNode;

      Node(const Node&);
      Node& operator=(const Node&);
    };

    typedef SingleField<Link<Node> > SFNode;
    typedef MultiField<Link<Node> > MFNode;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Node> >;
  template class AV_OSG_DLL MultiField<Link<osg::Node> >;
#endif

}

#include <avango/osg/Group.h>

#endif
