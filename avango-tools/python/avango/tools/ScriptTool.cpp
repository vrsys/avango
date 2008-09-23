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

#include "ScriptTool.h"

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ScriptTool"));
}

AV_FC_DEFINE(av::tools::ScriptTool);

AV_FIELD_DEFINE(av::tools::SFScriptTool);
AV_FIELD_DEFINE(av::tools::MFScriptTool);

av::tools::ScriptTool::ScriptTool()
{
  AV_FC_ADD_FIELD(TargetCallback, av::script::SFObject::ValueType());
  AV_FC_ADD_FIELD(AddedTargetCallback, av::script::SFObject::ValueType());
  AV_FC_ADD_FIELD(KeptTargetCallback, av::script::SFObject::ValueType());
  AV_FC_ADD_FIELD(RemovedTargetCallback, av::script::SFObject::ValueType());
}

av::tools::ScriptTool::~ScriptTool()
{}

void
av::tools::ScriptTool::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Tool::initClass();

    AV_FC_INIT(av::tools::Tool, av::tools::ScriptTool, true);

    SFScriptTool::initClass("av::tools::SFScriptTool", "av::Field");
    MFScriptTool::initClass("av::tools::MFScriptTool", "av::Field");
  }
}

/* virtual */ void
av::tools::ScriptTool::evaluateTarget(TargetHolder& holder)
{
  av::script::SFObject::ValueType callback = TargetCallback.getValue();
  if (callback.ptr() != Py_None)
    callback(Link<TargetHolder>(&holder));
}

/* virtual */ void
av::tools::ScriptTool::evaluateAddedTarget(TargetHolder& holder)
{
  av::script::SFObject::ValueType callback = AddedTargetCallback.getValue();
  if (callback.ptr() != Py_None)
    callback(Link<TargetHolder>(&holder));
}

/* virtual */ void
av::tools::ScriptTool::evaluateKeptTarget(TargetHolder& holder)
{
  av::script::SFObject::ValueType callback = KeptTargetCallback.getValue();
  if (callback.ptr() != Py_None)
    callback(Link<TargetHolder>(&holder));
}

/* virtual */ void
av::tools::ScriptTool::evaluateRemovedTarget(TargetHolder& holder)
{
  av::script::SFObject::ValueType callback = RemovedTargetCallback.getValue();
  if (callback.ptr() != Py_None)
    callback(Link<TargetHolder>(&holder));
}
