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

// $Id

#if !defined(AVANGO_LOGGING_LEVEL_H)
#define AVANGO_LOGGING_LEVEL_H

#if defined(_WIN32)
// MS VS 8 seems to define ERROR, which replaces the ERROR constant in the enum with a 0.
#if defined(ERROR)
#undef ERROR
#endif
#endif // _WIN32

/**
 * \file
 * \ingroup av_logging
 */

// includes, system

#include <string>

// includes, project
#include <avango/windows_specific.h>

namespace av
{
namespace logging
{
// types, exported (class, enum, struct, union, typedef)

/**
 * Avango log levels.
 *
 * \ingroup av_logging
 */

enum Level
{
    FATAL, ///< Severe error that will lead to an application abort.
    ERROR, ///< Severe error that still allows the application to continue.
    WARN,  ///< Potentially harmful situtation.
    INFO,  ///< Informational message that highlights the process of the application.
    DEBUG, ///< Useful debugging information.
    TRACE  ///< Application tracing that may disturb even in debug level.
};

// variables, exported (extern)

// functions, inlined (inline)

// functions, exported (extern)

/**
 * Avango log level to string conversion
 *
 * \ingroup av_logging
 */
AV_DLL const std::string& levelToString(Level level);

} // namespace logging

} // namespace av

#endif // #if !defined(AVANGO_LOGGING_LEVEL_H)
