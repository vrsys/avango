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

#include "OSGTools.h"

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/tools/Config.h>
#include <avango/tools/DragTool.h>
#include <avango/tools/IntersectionTargetHolder.h>
#include <avango/tools/NodePathTargetHolder.h>
#include <avango/tools/PickSelector.h>
#include <avango/tools/ProximitySelector.h>
#include <avango/tools/ScaledGrabTool.h>

#include <avango/python/register_field.h>

using namespace boost::python;
using namespace av::python;

namespace
{
  list
  PickSelectorPick(av::tools::PickSelector& pickSelector)
  {
    list py_selected_targets;

    const av::tools::MFTargetHolder::ContainerType &selected_targets = pickSelector.pick();
    for (av::tools::MFTargetHolder::ContainerType::const_iterator target = selected_targets.begin();
         target != selected_targets.end(); ++target)
    {
      py_selected_targets.append(*target);
    }

    return py_selected_targets;
  }
}

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

void init_OSGTools(void)
{
  register_field<av::tools::SFDragTool>("SFDragTool");
  register_multifield<av::tools::MFDragTool>("MFDragTool");
  register_field<av::tools::SFIntersectionTargetHolder>("SFIntersectionTargetHolder");
  register_multifield<av::tools::MFIntersectionTargetHolder>("MFIntersectionTargetHolder");
  register_field<av::tools::SFNodePathTargetHolder>("SFNodePathTargetHolder");
  register_multifield<av::tools::MFNodePathTargetHolder>("MFNodePathTargetHolder");
  register_field<av::tools::SFPickSelector>("SFPickSelector");
  register_multifield<av::tools::MFPickSelector>("MFPickSelector");
  register_field<av::tools::SFProximitySelector>("SFProximitySelector");
  register_multifield<av::tools::MFProximitySelector>("MFProximitySelector");
  register_field<av::tools::SFScaledGrabTool>("SFScaledGrabTool");
  register_multifield<av::tools::MFScaledGrabTool>("MFScaledGrabTool");

  class_<av::tools::DragTool, av::Link<av::tools::DragTool>, bases<av::tools::Tool>, boost::noncopyable >("DragTool", "DragTool class", no_init);
  class_<av::tools::NodePathTargetHolder, av::Link<av::tools::NodePathTargetHolder>, bases<av::tools::TargetHolder>, boost::noncopyable >("NodePathTargetHolder", "NodePathTargetHolder class", no_init);
  class_<av::tools::IntersectionTargetHolder, av::Link<av::tools::IntersectionTargetHolder>, bases<av::tools::NodePathTargetHolder>, boost::noncopyable >("IntersectionTargetHolder", "IntersectionTargetHolder class", no_init);
  class_<av::tools::PickSelector, av::Link<av::tools::PickSelector>, bases<av::tools::Selector>, boost::noncopyable >("PickSelector", "PickSelector class", no_init)
    .def("pick", PickSelectorPick)
    ;
  class_<av::tools::ProximitySelector, av::Link<av::tools::ProximitySelector>, bases<av::tools::Selector>, boost::noncopyable >("ProximitySelector", "ProximitySelector class", no_init);
  class_<av::tools::ScaledGrabTool, av::Link<av::tools::ScaledGrabTool>, bases<av::tools::Tool>, boost::noncopyable >("ScaledGrabTool", "ScaledGrabTool class", no_init);
}
