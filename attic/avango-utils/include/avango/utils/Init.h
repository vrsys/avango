// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_UTILS_INIT_H)
#define AVANGO_UTILS_INIT_H

/**
 * \file
 * \ingroup av_utils
 */

#include <avango/Typed.h>
#include "windows_specific_utils.h"

namespace av
{
  namespace utils
  {
    /**
     * Initializes the AVANGO utility library.
     * Usage: \code av::utils::Init::initClass(); \endcode
     *
     * \ingroup av_utils
     */
    class AV_UTILS_DLL Init
    {
      AV_TYPED_DECLARE_ABSTRACT();
    };
  }
}

#endif
