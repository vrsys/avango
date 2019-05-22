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

#if !defined(AV_UTILS_BOOL2AND_H)
#define AV_UTILS_BOOL2AND_H

/**
 * \file
 * \ingroup av_utils
 */

#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/gua/Fields.hpp>
#include "windows_specific_utils.h"

namespace av
{
namespace utils
{
class AV_UTILS_DLL Bool2And : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    Bool2And();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Bool2And();

    /* virtual */ void evaluate();

  public:
    av::SFBool Input1;
    av::SFBool Input2;
    av::SFBool Output;
};

} // namespace utils
} // namespace av

#endif
