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

#include <avango/tools/TypeSelector.h>

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
#include <avango/tools/NodePathTargetHolder.h>
#endif

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::TypeSelector"));
}

AV_FC_DEFINE(av::tools::TypeSelector);

AV_FIELD_DEFINE(av::tools::SFTypeSelector);
AV_FIELD_DEFINE(av::tools::MFTypeSelector);

av::tools::TypeSelector::TypeSelector():
  mTypesDirty(true)
{
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(SelectableTypes, MFContainer::ContainerType());
  AV_FC_ADD_FIELD(SelectableTargetTypes, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(EqualTypeOnly, false);
  AV_FC_ADD_FIELD(SearchTargetHolderNodePaths, false);
  AV_FC_ADD_FIELD(SearchOSGNodePaths, false);
}

av::tools::TypeSelector::~TypeSelector()
{}

void
av::tools::TypeSelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::TypeSelector, true);

    SFTypeSelector::initClass("av::tools::SFTypeSelector", "av::Field");
    MFTypeSelector::initClass("av::tools::MFTypeSelector", "av::Field");
  }
}

/* virtual */ void
av::tools::TypeSelector::fieldHasChanged(const av::Field& field)
{
  av::tools::Selector::fieldHasChanged(field);

  if (&field == &SelectableTypes || &field == &SelectableTargetTypes)
    mTypesDirty = true;
}

/* virtual */ void
av::tools::TypeSelector::evaluate()
{
  av::tools::Selector::evaluate();

  if (mTypesDirty)
  {
    mTypes.clear();

    const MFContainer::ContainerType &sel_objects = SelectableTypes.getValue();
    for (MFContainer::ContainerType::const_iterator sel_object = sel_objects.begin();
         sel_object != sel_objects.end(); ++sel_object)
    {
      mTypes.insert((*sel_object)->getTypeId());
    }

    const MFTargetHolder::ContainerType &sel_targets = SelectableTargetTypes.getValue();
    for (MFTargetHolder::ContainerType::const_iterator sel_holder = sel_targets.begin();
         sel_holder != sel_targets.end(); ++sel_holder)
    {
      if ((*sel_holder)->Target.getValue().isValid())
        mTypes.insert((*sel_holder)->Target.getValue()->getTypeId());
    }
  }

  MFTargetHolder::ContainerType selected_targets;

  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    bool found = false;

    if (isSelectable(*target))
    {
      selected_targets.push_back(*holder);
      found = true;
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
          if (isSelectable(**node))
          {
            selected_targets.push_back(new TargetHolder);
            selected_targets.back()->Target.setValue(*node);
            selected_targets.back()->Creator.setValue(this);
            selected_targets.back()->ParentTargets.add1Value(*holder);
            found = true;
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
              if (isSelectable(*av_node))
              {
                selected_targets.push_back(new TargetHolder);
                selected_targets.back()->Target.setValue(av_node);
                selected_targets.back()->Creator.setValue(this);
                selected_targets.back()->ParentTargets.add1Value(*holder);
                found = true;
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

bool
av::tools::TypeSelector::isSelectable(const av::FieldContainer& object)
{
  std::set<Type>::const_iterator type = mTypes.end();
  const Type object_type = object.getTypeId();

  if (EqualTypeOnly.getValue())
    type = mTypes.find(object_type);
  else
  {
    type = mTypes.begin();
    while (type != mTypes.end() && !object_type.isOfType(*type))
      ++type;
  }

  return (type != mTypes.end());
}
