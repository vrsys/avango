// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <avango/Logger.h>
#include <avango/display/mt/MultitouchFinger.h>

namespace
{
av::Logger& logger(av::getLogger("av::display::mtMultitouchFinger"));
}


AV_FC_DEFINE(av::display::mt::MultitouchFinger);

AV_FIELD_DEFINE(av::display::mt::SFMultitouchFinger);
AV_FIELD_DEFINE(av::display::mt::MFMultitouchFinger);

av::display::mt::MultitouchFinger::MultitouchFinger(int _id, ::osg::Vec2 _position, float _creationTime, float _angle, float _width, float _height, float _area)
{
  AV_FC_ADD_FIELD(Identifier, _id);
  AV_FC_ADD_FIELD(Position, _position);
  AV_FC_ADD_FIELD(CreationTime, _creationTime);
  AV_FC_ADD_FIELD(Angle, _angle);
  AV_FC_ADD_FIELD(Width, _width);
  AV_FC_ADD_FIELD(Height, _height);
  AV_FC_ADD_FIELD(Area, _area);
}

av::display::mt::MultitouchFinger::~MultitouchFinger()
{}

void
av::display::mt::MultitouchFinger::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::display::mt::MultitouchFinger, true);

    SFMultitouchFinger::initClass("av::display::mt::SFMultitouchFinger", "av::Field");
    MFMultitouchFinger::initClass("av::display::mt::MFMultitouchFinger", "av::Field");
  }
}
