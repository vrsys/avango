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

#include <avango/tools/PickSelector.h>

#include <avango/tools/IntersectionTargetHolder.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::PickSelector"));
}

AV_FC_DEFINE(av::tools::PickSelector);

AV_FIELD_DEFINE(av::tools::SFPickSelector);
AV_FIELD_DEFINE(av::tools::MFPickSelector);

av::tools::PickSelector::PickSelector():
  mLastPickTrigger(false)
{
  AV_FC_ADD_FIELD(PickTrigger, false);
  AV_FC_ADD_FIELD(TransitionOnly, true);
  AV_FC_ADD_FIELD(SetCreator, true);
  AV_FC_ADD_FIELD(EveryFrame, false);
  AV_FC_ADD_FIELD(PickRayTransform, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(PickRayDirection, ::osg::Vec3(0.0, 0.0, -1.0));
  AV_FC_ADD_FIELD(PickRayLength, 1000.0);
  AV_FC_ADD_FIELD(RootNode, 0);
  AV_FC_ADD_FIELD(FirstHitOnly, true);
  AV_FC_ADD_FIELD(NodePickMask, ~0u);
  AV_FC_ADD_FIELD(SubtreePickMask, ~0u);
  AV_FC_ADD_FIELD(PickNodesWithoutPickMask, true);
  AV_FC_ADD_FIELD(CreateNodePaths, false);
  AV_FC_ADD_FIELD(CreateIntersections, false);

  mIntersector =
    new ::osgUtil::LineSegmentIntersector(::osg::Vec3(0.0, 0.0, 0.0), ::osg::Vec3(0.0, 0.0, -1.0));
  mVisitor = new ::osgUtil::IntersectionVisitor(mIntersector.get());
}

av::tools::PickSelector::~PickSelector()
{}

void
av::tools::PickSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::PickSelector, true);

    SFPickSelector::initClass("av::tools::SFPickSelector", "av::Field");
    MFPickSelector::initClass("av::tools::MFPickSelector", "av::Field");
  }
}

const av::tools::MFTargetHolder::ContainerType&
av::tools::PickSelector::pick()
{
  mSelectedTargets.clear();

  if (RootNode.getValue().isValid())
  {
    // initialize line segment
    const ::osg::Vec3 start_pos = PickRayTransform.getValue().getTrans();
    const ::osg::Vec3 end_pos =
      (PickRayDirection.getValue() * PickRayLength.getValue()) * PickRayTransform.getValue();
    const ::osg::Vec3 pick_dir = end_pos - start_pos;
    mIntersector->setStart(start_pos);
    mIntersector->setEnd(end_pos);

    // compute intersections. check type of root node because apply is type dependent.
    ::osg::Node *root = RootNode.getValue()->getOsgNode();
    ::osg::Transform *root_transform = dynamic_cast< ::osg::Transform*>(root);
    if (root_transform != 0)
      mVisitor->apply(*root_transform);
    else
      mVisitor->apply(*root);

    if(mIntersector->containsIntersections())
    {
      MFContainer::ContainerType targets;
      const bool create_node_paths = CreateNodePaths.getValue();
      const bool create_intersections = CreateIntersections.getValue();
      const ::osgUtil::LineSegmentIntersector::Intersections &intersections =
        mIntersector->getIntersections();
      const unsigned int node_mask = NodePickMask.getValue();
      const unsigned int subtree_mask = SubtreePickMask.getValue();
      const bool pick_no_mask = PickNodesWithoutPickMask.getValue();

      for (::osgUtil::LineSegmentIntersector::Intersections::const_iterator intersection =
           intersections.begin(); intersection != intersections.end(); ++intersection)
      {
        av::Link<av::FieldContainer> target;
        std::list<av::Link<av::osg::Node> > node_path_list;

        // iterate over the node path from the intersected node to the root node,
        // check the pick masks and find the first Avango node.
        for (::osg::NodePath::const_reverse_iterator node = intersection->nodePath.rbegin();
             node != intersection->nodePath.rend(); ++node)
        {
          av::Link<av::osg::Node> av_node = av::osg::get_from_osg_object<av::osg::Node>(*node);
          if (av_node.isValid())
          {
            SFBool *ignore = dynamic_cast<SFBool*>(av_node->getField("PickIgnore"));
            if (ignore == 0 || !ignore->getValue())
            {
              SFUInt *mask = dynamic_cast<SFUInt*>(av_node->getField("PickMask"));
              if (!target.isValid())
              {
                if ((mask == 0 && pick_no_mask) ||
                    (mask != 0 && (node_mask & mask->getValue()) != 0u))
                {
                  target = av_node;
                  node_path_list.push_back(av_node);
                }
                else
                  break;
              }
              else
              {
                if ((mask == 0 && pick_no_mask) ||
                    (mask != 0 && (subtree_mask & mask->getValue()) != 0u))
                {
                  node_path_list.push_front(av_node);
                }
                else
                {
                  target.clear();
                  break;
                }
              }
            }
          }
        }

        // check if we want to pick front or back face
        if (target.isValid())
        {
          if (intersection->getWorldIntersectNormal() * pick_dir < 0.0)
          {
            SFBool *pick_front = dynamic_cast<SFBool*>(target->getField("PickFrontFace"));
            if (pick_front != 0 && !pick_front->getValue())
              target.clear();
          }
          else
          {
            SFBool *pick_back = dynamic_cast<SFBool*>(target->getField("PickBackFace"));
            if (pick_back != 0 && !pick_back->getValue())
              target.clear();
          }
        }

        // add node to output list, if we don't have it already
        if (target.isValid() && !hasObject(targets, target))
        {
          targets.push_back(target);
          Link<TargetHolder> holder;

          if (create_node_paths || create_intersections)
          {
            std::vector<av::Link<av::osg::Node> > node_path;
            node_path.reserve(node_path_list.size());
            node_path.insert(node_path.begin(), node_path_list.begin(), node_path_list.end());
            Link<NodePathTargetHolder> path_holder;

            if (create_intersections)
            {
              // create IntersectionTargetHolder
              Link<av::osg::Intersection> av_intersection = new av::osg::Intersection;
              av_intersection->NodePath.setValue(node_path);
              av_intersection->Point.setValue(intersection->getWorldIntersectPoint());
              av_intersection->Normal.setValue(intersection->getWorldIntersectNormal());
              Link<IntersectionTargetHolder> intersection_holder = new IntersectionTargetHolder;
              intersection_holder->Intersection.setValue(av_intersection);
              path_holder = intersection_holder;
            }
            else
              path_holder = new NodePathTargetHolder;

            path_holder->NodePath.setValue(node_path);
            holder = path_holder;
          }
          else
            holder = new TargetHolder;

          holder->Target.setValue(target);

          if (SetCreator.getValue())
            holder->Creator.setValue(this);

          mSelectedTargets.push_back(holder);
        }

        // stop if we only consider the first hit
        if (!mSelectedTargets.empty() && FirstHitOnly.getValue())
          break;
      }
    }

    mIntersector->reset();
  }

  return mSelectedTargets;
}

/* virtual */ void
av::tools::PickSelector::evaluate()
{
  av::tools::Selector::evaluate();

  // try to pick if trigger changes to or stays true
  if (PickTrigger.getValue() && (!mLastPickTrigger || !TransitionOnly.getValue()))
  {
    pick();

    if (!mSelectedTargets.empty() || !SelectedTargets.isEmpty())
      SelectedTargets.setValue(mSelectedTargets);

    if (!mLastPickTrigger)
    {
      mLastPickTrigger = true;
      if (!TransitionOnly.getValue() && EveryFrame.getValue())
        alwaysEvaluate(true);
    }
  }
  else if (!PickTrigger.getValue() && mLastPickTrigger)
  {
    if (!SelectedTargets.isEmpty())
      SelectedTargets.clear();
    mLastPickTrigger = false;
    alwaysEvaluate(false);
  }
}
