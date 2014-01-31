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

#include "Helper.h"

#include <stdexcept>

#include <avango/Field.h>
#include <avango/StandardFields.h>
#include <avango/FieldContainer.h>

std::string
av::getName(const Link<Distributed>& obj)
{
  if (!obj.isValid())
  {
    throw std::invalid_argument("av::getName(): obj not valid");
  }

  FieldContainer* fc = dynamic_cast<FieldContainer*>(obj.getPtr());
  if (!fc || fc->Name.getValue().empty())
  {
    return "unnamed";
  }
  else
  {
    return fc->Name.getValue();
  }
}
