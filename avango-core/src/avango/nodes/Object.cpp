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

#include "avango/Object.h"

#include <avango/logging/Logger.h>

namespace
{
  av::Logger& logger(av::Logger::getLogger("av::Object"));
}

AV_FC_DEFINE(av::Object);

AV_FIELD_DEFINE(av::SFObject);
AV_FIELD_DEFINE(av::MFObject);

av::Object::Object()
{}

av::Object::~Object()
{}

void
av::Object::initClass()
{
  if (!isTypeInitialized())
  {
    FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::Object, false);

    SFObject::initClass("av::SFObject", "av::Field");
    MFObject::initClass("av::MFObject", "av::Field");
  }
}
