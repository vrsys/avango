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

#if !defined(AVANGO_TOOLS_PICKSELECTOR_H)
#define AVANGO_TOOLS_PICKSELECTOR_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.h>

#include <avango/osg/Fields.h>
#include <avango/osg/Intersection.h>
#include <avango/osg/MatrixTransform.h>

#include <osgUtil/LineSegmentIntersector>

#include "windows_specific_tools.h"

namespace av
{
  namespace tools
  {
    /**
     * Pick selector class.
     *
     * If triggered, a line segment intersection is performed and for each valid intersection
     * a TargetHolder is created and placed in SelectedTargets. The type of the TargetHolder can
     * be defined with CreateNodePaths and CreateIntersections.
     * The Target in the TargetHolder is the first not ignored avango object found in the
     * intersection node path.
     *
     * The following fields may be added to nodes to influence the picking result:
     * - SFUInt PickMask:      See description of NodePickMask and SubtreePickMask below.
     * - SFBool PickFrontFace: Set to false to ignore front face hits for this node.
     * - SFBool PickBackFace:  Set to false to ignore back face hits for this node.
     * - SFBool PickIgnore:    Set to true to ignore this node as target and in the node path.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL PickSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      PickSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PickSelector();

    public:

      /**
       * Triggers the picking.
       */
      SFBool PickTrigger;

      /**
       * If true, the picking is only updated on PickTrigger transitions, which is the default.
       * If false, picking is updated in every evaluate while PickTrigger is true.
       */
      SFBool TransitionOnly;

      /**
       * If TransitionOnly is False and EveryFrame is true, picking is updated in every frame
       * while PickTrigger is true. By default, EveryFrame is false, which updates the picking
       * only in the frames when evaluate was triggered, e.g. by a change of PickRayTransform.
       */
      SFBool EveryFrame;

      /**
       * Defines the pick ray transformation relative to the root node coordinate system.
       */
      av::osg::SFMatrix PickRayTransform;

      /**
       * Defines the pick ray direction. Default is (0.0, 0.0, -1.0).
       */
      av::osg::SFVec3 PickRayDirection;

      /**
       * Defines the length of the pick ray. Default is 1000.0
       */
      SFDouble PickRayLength;

      /**
       * Entry scenegraph node from which the intersection traversal should start.
       */
      av::osg::SFNode RootNode;

      /**
       * If true, only the first visible hit may be selected. This is the default.
       */
      SFBool FirstHitOnly;

      /**
       * To disable picking for specific nodes, add a SFUInt field called "PickMask"
       * to the node. The picking is disabled, if the bitwise AND of it with the
       * NodePickMask of the PickSelector is zero. By default the mask is all 1's.
       */
      SFUInt NodePickMask;

      /**
       * To disable picking for the subtree of a specific node, add a SFUInt field called
       * "PickMask" to the node. The picking is disabled, if the bitwise AND of it with the
       * SubtreePickMask of the PickSelector is zero. By default the mask is all 1's.
       */
      SFUInt SubtreePickMask;

      /**
       * If true, nodes without a field called "PickMask" are picked. This is the default.
       */
      SFBool PickNodesWithoutPickMask;

      /**
       * If true, NodePathTargetHolder instead of TargetHolder objects are created
       * for every target in SelectedTargets. By default, it is false.
       */
      SFBool CreateNodePaths;

      /**
       * If true, IntersectionTargetHolder instead of TargetHolder objects are created
       * for every target in SelectedTargets. By default, it is false.
       */
      SFBool CreateIntersections;

      /**
       * Manually pick with the current field values and return the selected targets.
       */
      const MFTargetHolder::ContainerType& pick();

      /* virtual */ void evaluate();

    protected:

      ::osg::ref_ptr< ::osgUtil::LineSegmentIntersector> mIntersector;
      ::osg::ref_ptr< ::osgUtil::IntersectionVisitor> mVisitor;
      MFTargetHolder::ContainerType mSelectedTargets;
      bool mLastPickTrigger;
    };

    typedef SingleField<Link<PickSelector> > SFPickSelector;
    typedef MultiField<Link<PickSelector> > MFPickSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::PickSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::PickSelector> >;
#endif

}

#endif
