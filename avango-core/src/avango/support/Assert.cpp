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

#include "avango/Assert.h"

#include <cstdlib>
#include <sys/types.h>

#if defined(_WIN32)
#include <process.h>
#else
#include <unistd.h>
#endif

#include <sstream>
#include <iostream>

void av::assert_fail(const char* assertion, const char* file,
                     unsigned int line, const char* function)
{
  std::cerr << "Assertion \"" << assertion << "\" failed in file " << file
            << ", line " << line << ", function " << function << std::endl;

  const char* debugger = ::getenv("AV_DEBUGGER");
  if (debugger)
  {
#if defined(_WIN32)
    int my_pid = ::_getpid();
#else
    pid_t my_pid = ::getpid();
#endif

    std::ostringstream os;

    os << debugger << " /proc/" << my_pid << "/exe " << my_pid
      << std::ends;

    ::system(os.str().c_str());

  }

  ::abort();
}
