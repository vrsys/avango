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

#include <avango/StandardFields.h>

AV_FIELD_DEFINE(av::SFBool)
AV_FIELD_DEFINE(av::MFBool)

AV_FIELD_DEFINE(av::SFDouble)
AV_FIELD_DEFINE(av::MFDouble)

AV_FIELD_DEFINE(av::SFFloat)
AV_FIELD_DEFINE(av::MFFloat)

AV_FIELD_DEFINE(av::SFInt)
AV_FIELD_DEFINE(av::MFInt)

AV_FIELD_DEFINE(av::SFLong)
AV_FIELD_DEFINE(av::MFLong)

AV_FIELD_DEFINE(av::SFUInt)
AV_FIELD_DEFINE(av::MFUInt)

AV_FIELD_DEFINE(av::SFULong)
AV_FIELD_DEFINE(av::MFULong)

AV_FIELD_DEFINE(av::SFString)
AV_FIELD_DEFINE(av::MFString)

void av::initStandardFields()
{
    av::Field::initClass();

    av::SFBool::initClass("av::SFBool", "av::Field");
    av::MFBool::initClass("av::MFBool", "av::Field");

    av::SFDouble::initClass("av::SFDouble", "av::Field");
    av::MFDouble::initClass("av::MFDouble", "av::Field");

    av::SFFloat::initClass("av::SFFloat", "av::Field");
    av::MFFloat::initClass("av::MFFloat", "av::Field");

    av::SFInt::initClass("av::SFInt", "av::Field");
    av::MFInt::initClass("av::MFInt", "av::Field");

    av::SFLong::initClass("av::SFLong", "av::Field");
    av::MFLong::initClass("av::MFLong", "av::Field");

    av::SFUInt::initClass("av::SFUInt", "av::Field");
    av::MFUInt::initClass("av::MFUInt", "av::Field");

    av::SFULong::initClass("av::SFULong", "av::Field");
    av::MFULong::initClass("av::MFULong", "av::Field");

    av::SFString::initClass("av::SFString", "av::Field");
    av::MFString::initClass("av::MFString", "av::Field");
}
