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

#include "Update.h"

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::script::Update"));
}

AV_FC_DEFINE(av::script::Update);

void av::script::Update::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::script::Update, true);
  }
}

av::script::Update::Update(void)
{
  AV_FC_ADD_FIELD(Callback, SFObject::ValueType());
  AV_FC_ADD_FIELD(Active, false);
}

av::script::Update::~Update()
{}

void av::script::Update::evaluate(void)
{
  const bool active = Active.getValue();

  if (active)
  {
    SFObject::ValueType callback = Callback.getValue();
    if (callback.ptr() != Py_None)
    {
      try
      {
        callback();
      }
      catch (...)
      {
        logger.warn() << "invalid callback in Callback field";
      }
    }
  }

  alwaysEvaluate(active);
}
