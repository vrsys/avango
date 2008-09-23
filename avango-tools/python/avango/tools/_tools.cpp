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

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/tools/Config.h>
#include <avango/tools/Init.h>
#include <avango/tools/TargetHolder.h>
#include <avango/tools/Tool.h>

#include "ScriptTool.h"
#include "Selectors.h"

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
#include "OSGTools.h"
#endif

#include <avango/python/register_field.h>

using namespace boost::python;
using namespace av::python;

namespace
{
  list
  TargetHolderFind(av::tools::TargetHolder& holder, av::tools::TargetHolder* type)
  {
    list py_holders;

    if (type != 0)
    {
      av::tools::TargetHolder::FindList found_holders = holder.find(type->getTypeId());
      for (av::tools::TargetHolder::FindList::iterator found_holder = found_holders.begin();
           found_holder != found_holders.end(); ++found_holder)
      {
        py_holders.append(*found_holder);
      }
    }

    return py_holders;
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

BOOST_PYTHON_MODULE(_tools)
{
  av::tools::Init::initClass();
  av::tools::ScriptTool::initClass();

  register_field<av::tools::SFScriptTool>("SFScriptTool");
  register_multifield<av::tools::MFScriptTool>("MFScriptTool");
  register_field<av::tools::SFTargetHolder>("SFTargetHolder");
  register_multifield<av::tools::MFTargetHolder>("MFTargetHolder");
  register_field<av::tools::SFTool>("SFTool");
  register_multifield<av::tools::MFTool>("MFTool");

  class_<av::tools::Tool, av::Link<av::tools::Tool>, bases<av::Object>, boost::noncopyable >("Tool", "Tool base class", no_init);
  class_<av::tools::ScriptTool, av::Link<av::tools::ScriptTool>, bases<av::tools::Tool>, boost::noncopyable >("ScriptTool", "ScriptTool class", no_init);
  class_<av::tools::TargetHolder, av::Link<av::tools::TargetHolder>, bases<av::Object>, boost::noncopyable >("TargetHolder", "TargetHolder base class", no_init)
    .def("find", TargetHolderFind)
    ;

  init_Selectors();

#if defined(AVANGO_TOOLS_OSG_SUPPORT)
  init_OSGTools();
#endif
}
