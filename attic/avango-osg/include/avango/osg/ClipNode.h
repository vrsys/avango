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

#ifndef CLIPNODE_H_
#define CLIPNODE_H_

/**
 * \file
 * \ingroup av_osg
 */

#include <vector>

#include <osg/ClipNode>

#include <avango/osg/Group.h>
#include <avango/osg/ClipPlane.h>

namespace av
{
  namespace osg
  {
    typedef ::osg::ClipNode OsgClipNode;

    /**
     * Wrapper for ::osg::ClipNode
     *
     * \ingroup av_osg
     */
    class AV_OSG_DLL ClipNode : public av::osg::Group
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::osg::ClipNode is created.
       * Otherwise, the given ::osg::ClipNode is used.
       */
      ClipNode(OsgClipNode* osggroup = new OsgClipNode()); // use defined type to circumvent compiler bug in VS8

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ClipNode();

    public:

      MFClipPlane ClipPlanes;

      /**
       * Creates six clip planes corresponding to the given BoundingBox, specified by min and max
       */
      void createClipBox(::osg::Vec3 min, ::osg::Vec3 max);

      /**
       * Get the wrapped ::osg::ClipNode.
       */
      ::osg::ClipNode* getOsgClipNode() const;

    protected:

      virtual void getClipPlaneCB(const av::osg::MFClipPlane::GetValueEvent& event);
      virtual void setClipPlaneCB(const av::osg::MFClipPlane::SetValueEvent& event);

    private:

      ::osg::ClipNode *mOsgClipNode;
    };

    typedef SingleField<Link<ClipNode> > SFClipNode;
    typedef MultiField<Link<ClipNode> > MFClipNode;
  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_DLL SingleField<Link<osg::ClipNode> >;
  template class AV_OSG_DLL MultiField<Link<osg::ClipNode> >;
#endif

} // namespace av

#endif /* CLIPNODE_H_ */
