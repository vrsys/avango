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

#include <avango/NetLock.h>
#include <iostream>

AV_FC_DEFINE(av::NetLock);

av::NetLock::NetLock()
  : mRelease(false), mRequest(false)
{
  AV_FC_ADD_FIELD(Request, "");
  AV_FC_ADD_FIELD(Granted, "");
  AV_FC_ADD_FIELD(Release, "");

  getTypeId().setDistributable(true);

}

av::NetLock::~NetLock()
{}

/*static*/ void
av::NetLock::initClass()
{
  if(!isTypeInitialized())
  {
    Object::initClass();
    AV_FC_INIT(av::Object,
               av::NetLock,
               true);
  }
}

void
av::NetLock::fieldHasChanged(const av::Field& field)
{
  if (&field == &Request) {
    mRequest = true;
  } else if (&field == &Release) {
    mRelease = true;
  }
}

void
av::NetLock::evaluate()
{
  if (mRequest) {
    if (Granted.getValue().empty()) {
      Granted.setValue(Request.getValue());
    }
    mRequest = false;
  }
  if (mRelease) {
    if (Granted.getValue() == Release.getValue()) {
      Granted.setValue("");
    }
    mRelease = false;
  }
}


