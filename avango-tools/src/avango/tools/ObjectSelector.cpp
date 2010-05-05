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

#include <avango/tools/ObjectSelector.h>

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
#include <avango/tools/NodePathTargetHolder.h>
#endif

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ObjectSelector"));
}

AV_FC_DEFINE(av::tools::ObjectSelector);

AV_FIELD_DEFINE(av::tools::SFObjectSelector);
AV_FIELD_DEFINE(av::tools::MFObjectSelector);

av::tools::ObjectSelector::ObjectSelector()
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableObjects, MFContainer::ContainerType());
  AV_FC_ADD_FIELD(SelectableTargets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SearchTargetHolderNodePaths, false);
  AV_FC_ADD_FIELD(SearchOSGNodePaths, false);
}

av::tools::ObjectSelector::~ObjectSelector()
{}

void
av::tools::ObjectSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::ObjectSelector, true);

    SFObjectSelector::initClass("av::tools::SFObjectSelector", "av::Field");
    MFObjectSelector::initClass("av::tools::MFObjectSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::ObjectSelector::evaluate()
{
  av::tools::Selector::evaluate();

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  const MFContainer::ContainerType &sel_objects = SelectableObjects.getValue();
  const MFTargetHolder::ContainerType &sel_targets = SelectableTargets.getValue();
  MFTargetHolder::ContainerType selected_targets;

  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    bool found = false;

    if (hasObject(sel_objects, target))
    {
      selected_targets.push_back(*holder);
      found = true;
    }
    else
    {
      MFTargetHolder::ContainerType::const_iterator sel_holder = find(sel_targets, target);
      if (sel_holder != sel_targets.end())
      {
        selected_targets.push_back(combine(*holder, *sel_holder));
        found = true;
      }
    }

#if defined(AVANGO_TOOLS_OSG_SUPPORT)

    if (!found && SearchTargetHolderNodePaths.getValue())
    {
      TargetHolder::FindList found_holders = (*holder)->find(NodePathTargetHolder::getClassTypeId());
      for (TargetHolder::FindList::const_iterator found_holder = found_holders.begin();
           found_holder != found_holders.end() && !found; ++found_holder)
      {
        const Link<NodePathTargetHolder> path_holder =
          dynamic_cast<NodePathTargetHolder*>(found_holder->getPtr());
        const av::osg::MFNode::ContainerType &node_path = path_holder->NodePath.getValue();
        for (av::osg::MFNode::ContainerType::const_reverse_iterator node = node_path.rbegin();
             node != node_path.rend() && !found; ++node)
        {
          if (hasObject(sel_objects, *node))
          {
            selected_targets.push_back(new TargetHolder);
            selected_targets.back()->Target.setValue(*node);
            selected_targets.back()->Creator.setValue(this);
            selected_targets.back()->ParentTargets.add1Value(*holder);
            found = true;
          }
          else
          {
            MFTargetHolder::ContainerType::const_iterator sel_holder = find(sel_targets, *node);
            if (sel_holder != sel_targets.end())
            {
              selected_targets.push_back(new TargetHolder);
              selected_targets.back()->Target.setValue(*node);
              selected_targets.back()->Creator.setValue(this);

              const bool keep_sel_holder = (*sel_holder)->keep();
              if ((*holder)->keep() || !keep_sel_holder)
                selected_targets.back()->ParentTargets.add1Value(*holder);
              if (keep_sel_holder)
                selected_targets.back()->ParentTargets.add1Value(*sel_holder);

              found = true;
            }
          }
        }
      }
    }

    if (!found && SearchOSGNodePaths.getValue())
    {
      Link<av::osg::Node> target_node = dynamic_cast<av::osg::Node*>(target.getPtr());
      if (target_node.isValid())
      {
        const ::osg::NodePathList node_paths = target_node->getOsgNode()->getParentalNodePaths();
        for (::osg::NodePathList::const_iterator node_path = node_paths.begin();
             node_path != node_paths.end() && !found; ++node_path)
        {
          for (::osg::NodePath::const_reverse_iterator node = node_path->rbegin();
               node != node_path->rend() && !found; ++node)
          {
            Link<av::osg::Node> av_node = av::osg::get_from_osg_object<av::osg::Node>(*node);
            if (av_node.isValid())
            {
              if (hasObject(sel_objects, av_node))
              {
                selected_targets.push_back(new TargetHolder);
                selected_targets.back()->Target.setValue(av_node);
                selected_targets.back()->Creator.setValue(this);
                selected_targets.back()->ParentTargets.add1Value(*holder);
                found = true;
              }
              else
              {
                MFTargetHolder::ContainerType::const_iterator sel_holder = find(sel_targets, av_node);
                if (sel_holder != sel_targets.end())
                {
                  selected_targets.push_back(new TargetHolder);
                  selected_targets.back()->Target.setValue(av_node);
                  selected_targets.back()->Creator.setValue(this);

                  const bool keep_sel_holder = (*sel_holder)->keep();
                  if ((*holder)->keep() || !keep_sel_holder)
                    selected_targets.back()->ParentTargets.add1Value(*holder);
                  if (keep_sel_holder)
                    selected_targets.back()->ParentTargets.add1Value(*sel_holder);

                  found = true;
                }
              }
            }
          }
        }
      }
    }

#endif
  }

  if (!selected_targets.empty() || !SelectedTargets.isEmpty())
    SelectedTargets.setValue(selected_targets);
}
