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

#include "Selectors.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>

#include <avango/tools/ChangeSelector.hpp>
#include <avango/tools/InjectSelector.hpp>
#include <avango/tools/ObjectSelector.hpp>
#include <avango/tools/NameSelector.hpp>
#include <avango/tools/FieldSelector.hpp>
#include <avango/tools/Selector.hpp>
#include <avango/tools/SingleSelector.hpp>
#include <avango/tools/PickSelector.hpp>
#include <avango/tools/ProximitySelector.hpp>
#include <avango/tools/TriggerSelector.hpp>
#include <avango/tools/TypeSelector.hpp>
#include <avango/tools/UnionSelector.hpp>

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

void init_Selectors()
{
  register_field<av::tools::SFChangeSelector>("SFChangeSelector");
  register_multifield<av::tools::MFChangeSelector>("MFChangeSelector");
  register_field<av::tools::SFInjectSelector>("SFInjectSelector");
  register_multifield<av::tools::MFInjectSelector>("MFInjectSelector");
  register_field<av::tools::SFObjectSelector>("SFObjectSelector");
  register_multifield<av::tools::MFObjectSelector>("MFObjectSelector");
  register_field<av::tools::SFNameSelector>("SFNameSelector");
  register_multifield<av::tools::MFNameSelector>("MFNameSelector");
  register_field<av::tools::SFFieldSelector>("SFFieldSelector");
  register_multifield<av::tools::MFFieldSelector>("MFFieldSelector");
  register_field<av::tools::SFSelector>("SFSelector");
  register_multifield<av::tools::MFSelector>("MFSelector");
  register_field<av::tools::SFSingleSelector>("SFSingleSelector");
  register_multifield<av::tools::MFSingleSelector>("MFSingleSelector");
  register_field<av::tools::SFPickSelector>("SFPickSelector");
  register_multifield<av::tools::MFPickSelector>("MFPickSelector");
  register_field<av::tools::SFProximitySelector>("SFProximitySelector");
  register_multifield<av::tools::MFProximitySelector>("MFProximitySelector");
  register_field<av::tools::SFTriggerSelector>("SFTriggerSelector");
  register_multifield<av::tools::MFTriggerSelector>("MFTriggerSelector");
  register_field<av::tools::SFTypeSelector>("SFTypeSelector");
  register_multifield<av::tools::MFTypeSelector>("MFTypeSelector");
  register_field<av::tools::SFUnionSelector>("SFUnionSelector");
  register_multifield<av::tools::MFUnionSelector>("MFUnionSelector");

  class_<av::tools::Selector, av::Link<av::tools::Selector>, bases<av::FieldContainer>, boost::noncopyable >("Selector", "Selector base class", no_init);
  class_<av::tools::ChangeSelector, av::Link<av::tools::ChangeSelector>, bases<av::tools::Selector>, boost::noncopyable >("ChangeSelector", "ChangeSelector class", no_init);
  class_<av::tools::InjectSelector, av::Link<av::tools::InjectSelector>, bases<av::tools::Selector>, boost::noncopyable >("InjectSelector", "InjectSelector class", no_init);
  class_<av::tools::ObjectSelector, av::Link<av::tools::ObjectSelector>, bases<av::tools::Selector>, boost::noncopyable >("ObjectSelector", "ObjectSelector class", no_init);
  class_<av::tools::NameSelector, av::Link<av::tools::NameSelector>, bases<av::tools::Selector>, boost::noncopyable >("NameSelector", "NameSelector class", no_init);
  class_<av::tools::FieldSelector, av::Link<av::tools::FieldSelector>, bases<av::tools::Selector>, boost::noncopyable >("FieldSelector", "FieldSelector class", no_init);
  class_<av::tools::SingleSelector, av::Link<av::tools::SingleSelector>, bases<av::tools::Selector>, boost::noncopyable >("SingleSelector", "SingleSelector class", no_init);
  class_<av::tools::PickSelector, av::Link<av::tools::PickSelector>, bases<av::tools::Selector>, boost::noncopyable >("PickSelector", "PickSelector class", no_init);
  class_<av::tools::ProximitySelector, av::Link<av::tools::ProximitySelector>, bases<av::tools::Selector>, boost::noncopyable >("ProximitySelector", "ProximitySelector class", no_init);
  class_<av::tools::TriggerSelector, av::Link<av::tools::TriggerSelector>, bases<av::tools::Selector>, boost::noncopyable >("TriggerSelector", "TriggerSelector class", no_init);
  class_<av::tools::TypeSelector, av::Link<av::tools::TypeSelector>, bases<av::tools::Selector>, boost::noncopyable >("TypeSelector", "TypeSelector class", no_init);
  class_<av::tools::UnionSelector, av::Link<av::tools::UnionSelector>, bases<av::tools::Selector>, boost::noncopyable >("UnionSelector", "UnionSelector class", no_init);
}
