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

#include "Selectors.h"

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/tools/Config.h>

#include <avango/tools/ChangeSelector.h>
#include <avango/tools/InjectSelector.h>
#include <avango/tools/ObjectSelector.h>
#include <avango/tools/NameSelector.h>
#include <avango/tools/FieldSelector.h>
#include <avango/tools/Selector.h>
#include <avango/tools/SingleSelector.h>
#include <avango/tools/TriggerSelector.h>
#include <avango/tools/TypeSelector.h>
#include <avango/tools/UnionSelector.h>

#include <avango/python/register_field.h>

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

void init_Selectors(void)
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
  register_field<av::tools::SFTriggerSelector>("SFTriggerSelector");
  register_multifield<av::tools::MFTriggerSelector>("MFTriggerSelector");
  register_field<av::tools::SFTypeSelector>("SFTypeSelector");
  register_multifield<av::tools::MFTypeSelector>("MFTypeSelector");
  register_field<av::tools::SFUnionSelector>("SFUnionSelector");
  register_multifield<av::tools::MFUnionSelector>("MFUnionSelector");

  class_<av::tools::Selector, av::Link<av::tools::Selector>, bases<av::Object>, boost::noncopyable >("Selector", "Selector base class", no_init);
  class_<av::tools::ChangeSelector, av::Link<av::tools::ChangeSelector>, bases<av::tools::Selector>, boost::noncopyable >("ChangeSelector", "ChangeSelector class", no_init);
  class_<av::tools::InjectSelector, av::Link<av::tools::InjectSelector>, bases<av::tools::Selector>, boost::noncopyable >("InjectSelector", "InjectSelector class", no_init);
  class_<av::tools::ObjectSelector, av::Link<av::tools::ObjectSelector>, bases<av::tools::Selector>, boost::noncopyable >("ObjectSelector", "ObjectSelector class", no_init);
  class_<av::tools::NameSelector, av::Link<av::tools::NameSelector>, bases<av::tools::Selector>, boost::noncopyable >("NameSelector", "NameSelector class", no_init);
  class_<av::tools::FieldSelector, av::Link<av::tools::FieldSelector>, bases<av::tools::Selector>, boost::noncopyable >("FieldSelector", "FieldSelector class", no_init);
  class_<av::tools::SingleSelector, av::Link<av::tools::SingleSelector>, bases<av::tools::Selector>, boost::noncopyable >("SingleSelector", "SingleSelector class", no_init);
  class_<av::tools::TriggerSelector, av::Link<av::tools::TriggerSelector>, bases<av::tools::Selector>, boost::noncopyable >("TriggerSelector", "TriggerSelector class", no_init);
  class_<av::tools::TypeSelector, av::Link<av::tools::TypeSelector>, bases<av::tools::Selector>, boost::noncopyable >("TypeSelector", "TypeSelector class", no_init);
  class_<av::tools::UnionSelector, av::Link<av::tools::UnionSelector>, bases<av::tools::Selector>, boost::noncopyable >("UnionSelector", "UnionSelector class", no_init);
}
