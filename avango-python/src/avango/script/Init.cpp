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

#include <avango/python/script/Init.h>
#include <avango/python/script/Types.h>
#include <avango/python/register_field.h>
#include "Update.h"
#include "Script.h"
#include <boost/python.hpp>

using namespace av::python;
using namespace boost::python;

AV_FIELD_DEFINE(av::script::SFObject);
AV_FIELD_DEFINE(av::script::MFObject);

void av::script::register_object(void)
{
  av::script::SFObject::initClass("av::script::SFObject", "av::Field");
  av::script::MFObject::initClass("av::script::MFObject", "av::Field");
  av::script::Update::initClass();
}

void av::script::register_object_fields(void)
{
  register_field<av::script::SFObject>("SFObject");
  register_multifield<av::script::MFObject>("MFObject");
}
