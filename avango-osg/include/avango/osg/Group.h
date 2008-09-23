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

#if !defined(AVANGO_OSG_GROUP_H)
#define AVANGO_OSG_GROUP_H

/**
 * \file
 * \ingroup av_osg
 */

#include <vector>

#include <osg/Group>

#include <avango/osg/Node.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::Group OsgGroup;

    /**
     * Wrapper for ::osg::Group
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL Group : public av::osg::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::Group is created.
       * Otherwise, the given ::osg::Group is used.
       */
      Group(OsgGroup* osggroup = new OsgGroup()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Group();

    public:

      /**
       * The children of this node.
       */
      MFNode Children;

      /**
       * Get the wrapped ::osg::Group.
       */
      ::osg::Group* getOsgGroup() const;

    protected:

      virtual void getChildrenCB(const av::osg::MFNode::GetValueEvent& event);
      virtual void setChildrenCB(const av::osg::MFNode::SetValueEvent& event);

    private:

      ::osg::Group *mOsgGroup;
    };

    typedef SingleField<Link<Group> > SFGroup;
    typedef MultiField<Link<Group> > MFGroup;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::Group> >;
  template class AV_OSG_DLL MultiField<Link<osg::Group> >;
#endif

} // namespace av

#endif
