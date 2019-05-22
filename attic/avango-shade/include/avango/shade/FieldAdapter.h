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

#ifndef AVANGO_SHADE_FIELDADAPTER_H
#define AVANGO_SHADE_FIELDADAPTER_H

/**
 * \file
 * \ingroup av_shade
 */

#include "core/types/TypeAccessor.h"
#include <string>
#include "windows_specific_shade.h"

namespace shade
{
class Type;
}

namespace av
{
class FieldContainer;

namespace shade
{
/**
 * Registers field with appropriate getter and setter functions
 *
 * \ingroup av_shade
 */
class AV_SHADE_DLL FieldAdapter
{
  public:
    virtual ~FieldAdapter(void) {}
    virtual ::shade::types::TypeAccessor::HashType hash(void) const = 0;
    virtual void bindField(::shade::Type* type, const std::string& name, FieldContainer* container) const = 0;
};
} // namespace shade
} // namespace av

#endif
