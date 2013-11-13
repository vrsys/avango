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

#include "avango/NetID.h"

#include <ostream>

/* static */ const int av::NetID::sNetGroupContainerHolder = -202;
/* static */ const int av::NetID::sNetGroupRootNode = -201;
/* static */ const int av::NetID::sWellKnownBase = -100;

av::NetID::NetID()
{
  setNull();
}

av::NetID::NetID(const std::string& eid, int number)
{
  set(eid, number);
}

av::NetID::NetID(const av::NetID& ni)
{
  set(ni.mCreator, ni.mNumber);
}

av::NetID::~NetID()
{}

av::NetID&
av::NetID::operator=(const NetID& ni)
{
  set(ni.mCreator, ni.mNumber);
  return *this;
}

bool
av::NetID::operator==(const NetID& ni) const
{
  return
    (mNumber == ni.mNumber) &&
    (mCreator == ni.mCreator);
}

void
av::NetID::set(const std::string& eid, int number)
{
  mCreator = eid;
  mNumber = number;
}

int
av::NetID::getNum() const
{
  return mNumber;
}

const std::string&
av::NetID::getEID() const
{
  return mCreator;
}

void
av::NetID::setNull()
{
  mNumber = -1;
}

bool
av::NetID::isNull() const
{
  return mNumber == -1;
}

av::NetID
av::NetID::nullID()
{
  return NetID();
}

bool
av::NetID::isWellKnown() const
{
  return mNumber <= sWellKnownBase;
}

std::ostream& av::operator<< (std::ostream& os, const NetID& id)
{
  if (id.isNull()) {
    os << "<null-id>";
  } else if (id.isWellKnown()) {
    os << id.mCreator << "#<" << id.mNumber << ">";
  } else {
    os << id.mCreator << "#" << id.mNumber;
  }
  return os;
}
