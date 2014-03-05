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

#include <avango/tools/ProximitySelector.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ProximitySelector"));

  float
  distance(::gua::math::vec3 const& pos1, ::gua::math::vec3 const& pos2)
  {
    return ::scm::math::length(pos1 - pos2);
  }
}

AV_FC_DEFINE(av::tools::ProximitySelector);

AV_FIELD_DEFINE(av::tools::SFProximitySelector);
AV_FIELD_DEFINE(av::tools::MFProximitySelector);

av::tools::ProximitySelector::ProximitySelector()
{
  AV_FC_ADD_FIELD(TargetObjects, MFContainer::ContainerType());
  AV_FC_ADD_FIELD(Targets, MFTargetHolder::ContainerType());
  AV_FC_ADD_FIELD(Position, ::gua::math::vec3(0.0, 0.0, 0.0));
  AV_FC_ADD_FIELD(PositionTransform, ::gua::math::mat4::identity());
  AV_FC_ADD_FIELD(ProximityRadius, 1.0);
  AV_FC_ADD_FIELD(DistanceRadius, 2.0);
  AV_FC_ADD_FIELD(Time, 0.0);
  AV_FC_ADD_FIELD(ProximityLag, 0.0);
  AV_FC_ADD_FIELD(MaxNumberOfTargets, 0u);
}

av::tools::ProximitySelector::~ProximitySelector()
{}

void
av::tools::ProximitySelector::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Selector::initClass();

    AV_FC_INIT(av::tools::Selector, av::tools::ProximitySelector, true);

    SFProximitySelector::initClass("av::tools::SFProximitySelector", "av::Field");
    MFProximitySelector::initClass("av::tools::MFProximitySelector", "av::Field");
  }
}

/* virtual */ void
av::tools::ProximitySelector::evaluate()
{
  av::tools::Selector::evaluate();

  // get needed field values
  const MFContainer::ContainerType &target_objects = TargetObjects.getValue();
  const MFTargetHolder::ContainerType &targets = Targets.getValue();
  const ::gua::math::vec3 pos = PositionTransform.getValue() * Position.getValue();
  const double prox_radius = ProximityRadius.getValue();
  const double dist_radius = DistanceRadius.getValue();
  const double time = Time.getValue();
  const double lag = ProximityLag.getValue();
  const unsigned int max_targets = MaxNumberOfTargets.getValue();

  // remove proximity candidates, which are no longer in the target lists
  {
    TargetTimeList_t::iterator cand = mProxCands.begin();
    while (cand != mProxCands.end())
    {
      if (hasObject(target_objects, cand->first) || av::tools::hasTarget(targets, cand->first))
        ++cand;
      else
        cand = mProxCands.erase(cand);
    }
  }

  // remove distance candidates, which are no longer in the target lists
  {
    TargetTimeList_t::iterator cand = mDistCands.begin();
    while (cand != mDistCands.end())
    {
      if (hasObject(target_objects, cand->first) || av::tools::hasTarget(targets, cand->first))
        ++cand;
      else
      {
        MFTargetHolder::ContainerType::iterator holder = av::tools::find(mSelTargets, cand->first);
        if (holder != mSelTargets.end())
          mSelTargets.erase(holder);
        else
          logger.warn() << "distance candidate was not in selected targets";
        cand = mDistCands.erase(cand);
      }
    }
  }

  // remove selected targets, which are no longer in the target lists
  {
    MFTargetHolder::ContainerType::iterator holder = mSelTargets.begin();
    while (holder != mSelTargets.end())
    {
      const SFContainer::ValueType &target = (*holder)->Target.getValue();
      if (hasObject(target_objects, target) || av::tools::hasTarget(targets, target))
        ++holder;
      else
        holder = mSelTargets.erase(holder);
    }
  }

  TargetTimeList_t new_prox_cands, new_dist_cands;
  MFTargetHolder::ContainerType new_sel_targets;

  // check for new proximity candidates in TargetObjects
  for (MFContainer::ContainerType::const_iterator target = target_objects.begin();
       target != target_objects.end(); ++target)
  {
    if (!hasTarget(mProxCands, *target) && !av::tools::hasTarget(mSelTargets, *target))
    {
      // we only accept gua nodes to get the absolute transform
      Link<av::gua::Node> node = dynamic_cast<av::gua::Node*>(target->getBasePtr());
      if (node.isValid())
      {
        const ::gua::math::mat4 nodeTransform(node->getGuaNode()->get_world_transform());
        const ::gua::math::vec3 nodeTranslation(nodeTransform[12],
                                                nodeTransform[13],
                                                nodeTransform[14]);
        if (distance(pos,  nodeTranslation) < prox_radius) {
          if (lag > 0.000001)
            new_prox_cands.push_back(TargetTimePair_t(node, time));
          else
          {
            new_sel_targets.push_back(new TargetHolder);
            new_sel_targets.back()->Target.setValue(*target);
            new_sel_targets.back()->Creator.setValue(this);
          }
        }
      }
    }
  }

  // check for new proximity candidates in Targets
  for (MFTargetHolder::ContainerType::const_iterator holder = targets.begin();
       holder != targets.end(); ++holder)
  {
    const SFContainer::ValueType &target = (*holder)->Target.getValue();
    if (!hasTarget(mProxCands, target) && !av::tools::hasTarget(mSelTargets, target))
    {
      // we only accept gua nodes to get the absolute transform
      Link<av::gua::Node> node = dynamic_cast<av::gua::Node*>(target.getBasePtr());
      if (node.isValid())
      {
        const ::gua::math::mat4 nodeTransform(node->getGuaNode()->get_world_transform());
        const ::gua::math::vec3 nodeTranslation(nodeTransform[12],
                                                nodeTransform[13],
                                                nodeTransform[14]);
        if (distance(pos,  nodeTranslation) < prox_radius) {
          if (lag > 0.000001)
            new_prox_cands.push_back(TargetTimePair_t(node, time));
          else
            new_sel_targets.push_back(*holder);
        }
      }
    }
  }

  // check if proximity candidates are out of range or have timed out
  {
    TargetTimeList_t::iterator cand = mProxCands.begin();
    while (cand != mProxCands.end())
    {
      const ::gua::math::mat4 candTransform(cand->first->getGuaNode()->get_world_transform());
      const ::gua::math::vec3 candTranslation(candTransform[12],
                                              candTransform[13],
                                              candTransform[14]);
      if (distance(pos, candTranslation) > prox_radius)
        cand = mProxCands.erase(cand);
      else if (time - cand->second > lag)
      {
        MFTargetHolder::ContainerType::const_iterator holder =
          av::tools::find(targets, cand->first);
        if (holder != targets.end())
          new_sel_targets.push_back(*holder);
        else
        {
          new_sel_targets.push_back(new TargetHolder);
          new_sel_targets.back()->Target.setValue(cand->first);
          new_sel_targets.back()->Creator.setValue(this);
        }

        cand = mProxCands.erase(cand);
      }
      else
        ++cand;
    }
  }

  // check for updated target holders and if selected targets are out of range
  {
    MFTargetHolder::ContainerType::iterator holder = mSelTargets.begin();
    while (holder != mSelTargets.end())
    {
      const SFContainer::ValueType target = (*holder)->Target.getValue();
      MFTargetHolder::ContainerType::const_iterator input_holder = av::tools::find(targets, target);
      if (input_holder != targets.end() && *holder != *input_holder)
        *holder = *input_holder;

      if (hasTarget(mDistCands, target))
        ++holder;
      else
      {
          // we only accept gua nodes to get the absolute transform
        Link<av::gua::Node> node = dynamic_cast<av::gua::Node*>(target.getBasePtr());
        if (node.isValid())
        {
          const ::gua::math::mat4 nodeTransform(node->getGuaNode()->get_world_transform());
          const ::gua::math::vec3 nodeTranslation(nodeTransform[12],
                                                  nodeTransform[13],
                                                  nodeTransform[14]);
          if (distance(pos,  nodeTranslation) < prox_radius) {
            if (lag > 0.000001)
            {
              new_dist_cands.push_back(TargetTimePair_t(node, time));
              ++holder;
            }
            else
              holder = mSelTargets.erase(holder);
          }
          else
            ++holder;
        }
        else
        {
          holder = mSelTargets.erase(holder);
          logger.warn() << "non-node target was in selected targets";
        }
      }
    }
  }

  // check if distance candidates are back in range or have have timed out
  {
    TargetTimeList_t::iterator cand = mDistCands.begin();
    while (cand != mDistCands.end())
    {
      const ::gua::math::mat4 candTransform(cand->first->getGuaNode()->get_world_transform());
      const ::gua::math::vec3 candTranslation(candTransform[12],
                                              candTransform[13],
                                              candTransform[14]);

      if (distance(pos, candTranslation) < dist_radius)
        cand = mDistCands.erase(cand);
      else if (time - cand->second > lag)
      {
        MFTargetHolder::ContainerType::iterator holder = av::tools::find(mSelTargets, cand->first);
        if (holder != mSelTargets.end())
          mSelTargets.erase(holder);
        else
          logger.warn() << "distance candidate was not in selected targets";
        cand = mDistCands.erase(cand);
      }
      else
        ++cand;
    }
  }

  // update lists
  mProxCands.splice(mProxCands.end(), new_prox_cands);
  mDistCands.splice(mDistCands.end(), new_dist_cands);
  mSelTargets.insert(mSelTargets.end(), new_sel_targets.begin(), new_sel_targets.end());

  // check if we have too many targets
  if (max_targets != 0u)
  {
    while (mSelTargets.size() > max_targets && !mDistCands.empty())
    {
      MFTargetHolder::ContainerType::iterator holder =
        av::tools::find(mSelTargets, mDistCands.front().first);
      if (holder != mSelTargets.end())
        mSelTargets.erase(holder);
      else
        logger.warn() << "distance candidate was not in selected targets";
      mDistCands.pop_front();
    }

    if (mSelTargets.size() > max_targets)
    {
      mSelTargets.erase(mSelTargets.begin(),
                        mSelTargets.begin() + (mSelTargets.size() - max_targets));
    }
  }

  // update output field
  if (SelectedTargets.getValue() != mSelTargets)
    SelectedTargets.setValue(mSelTargets);
}
