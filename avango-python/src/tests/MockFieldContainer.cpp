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

#include "MockFieldContainer.h"

AV_FC_DEFINE(MockFieldContainer);

MockFieldContainer::MockFieldContainer()
{
  AV_FC_ADD_FIELD(AnIntField, 0);
  AV_FC_ADD_FIELD(ADoubleField, 0.);
  AV_FC_ADD_FIELD(ABoolField, false);
  AV_FC_ADD_FIELD(AStringField, "value");
  AV_FC_ADD_FIELD(AnIntMultiField, std::vector<int>());
  AV_FC_ADD_FIELD(AFieldContainer, 0);
}

/* virtual */ MockFieldContainer::~MockFieldContainer()
{}

/* virtual */ void MockFieldContainer::refImpl()
{}

/* virtual */ void MockFieldContainer::unrefImpl()
{}

/* virtual */ int MockFieldContainer::refCountImpl()
{
  return 1;
}

/* static */ void MockFieldContainer::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, MockFieldContainer, true);
  }
}

