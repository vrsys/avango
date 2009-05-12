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

#if !defined(AVANGO_ASSERT_H)
#define AVANGO_ASSERT_H

#include <cassert>

namespace av
{
  void assert_fail(const char* assertion, const char* file,
                   unsigned int line, const char* function);
}

/**
 * \file
 * \ingroup av
 */

/**
 * \def AV_ASSERT(expr)
 *
 * Improved assert. It tries to start a debugger when an assertion occurs.
 * The default debugger (gdb) can be overridden by setting the AV_DEBUGGER
 * environment variable accordingly.
 *
 * \ingroup av
 */

/**
 * \def AV_ASSERT_FUNCTION
 *
 * Try to get a nicely formatted function name from the current context.
 * AV_ASSERT_FUNCTION  tries to use the default __ASSERT_FUNCTION from GLIBC.
 *
 * \ingroup av
 */

#ifdef NDEBUG

# define AV_ASSERT(expr) (static_cast<void>(0))

#else // #ifdef NDEBUG

# define AV_ASSERT(expr)                                                \
  ((expr)                                                               \
   ? static_cast<void>(0)                                               \
   : (::av::assert_fail(#expr, __FILE__, __LINE__, AV_ASSERT_FUNCTION),   \
      static_cast<void>(0)))

// use GLIBC __ASSERT_FUNCTION whenever possible
# if defined __ASSERT_FUNCTION
#  define AV_ASSERT_FUNCTION __ASSERT_FUNCTION
# else
#  define AV_ASSERT_FUNCTION ((const char *) 0)
# endif

#endif // #ifdef NDEBUG

#endif // #if !defined(AVANGO_ASSERT_H)
