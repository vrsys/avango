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

#include <avango/Create.h>

// includes, system

#include <cstdlib>
#include <boost/format.hpp>

// includes, project

#include <avango/logging/Logger.h>

// internal unnamed namespace

namespace
{
// types, internal

// variables, internal

av::Logger& logger(av::getLogger("av::Create"));

// functions, internal

} // namespace

// variables, exported

// functions, exported

/* virtual */
av::Create::~Create() {}

/* virtual */ av::Typed* av::Create::makeInstance() const
{
    AVANGO_LOG(logger, logging::FATAL, "makeInstance(): logically 'pure virtual' function called; aborting!")

    ::abort();

#if defined(_WIN32)
    return NULL; // and this is a special treatment to satisfy MS VS 8.
#endif
}
