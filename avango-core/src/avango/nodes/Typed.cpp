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

// include i/f header

#include "avango/Typed.h"

// includes, system

#include <cstdlib>
#include <boost/format.hpp>

// includes, project

#include <avango/Assert.h>
#include <avango/logging/Logger.h>

#define AV_TYPED_DEBUG
#undef AV_TYPED_DEBUG

// internal unnamed namespace

namespace
{
// types, internal

// variables, internal

av::logging::Logger& logger(av::logging::Logger::getLogger("av::Typed"));

// functions, internal

} // namespace

// variables, exported

// functions, exported

/* static */ av::Type av::Typed::sClassTypeId = av::Type::badType();

av::Typed::Typed() {}

/* virtual */
av::Typed::~Typed() {}

/* static */ void av::Typed::initClass()
{
    if(Type::badType() == sClassTypeId)
    {
        sClassTypeId = Type::createAbstractType(Type::badType(), "av::Typed", false);

        AV_ASSERT(Type::badType() != sClassTypeId);
    }
}

/* static */ av::Type av::Typed::getClassTypeId()
{
    if(Type::badType() == sClassTypeId)
    {
        AVANGO_LOG(logger, logging::WARN, "getClassTypeId(): returning 'bad_type'!")

        return Type::badType();
    }

    return sClassTypeId;
}
