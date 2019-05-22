// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#if !defined(AV_SCRIPT_UPDATE_H)
#define AV_SCRIPT_UPDATE_H

#include <avango/python/script/Types.h>

#include <avango/FieldContainer.h>

namespace av
{
namespace script
{
class Update : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    Update(void);

  protected:
    virtual ~Update();

  public:
    /* virtual */ void evaluate(void);

    SFObject Callback;
    SFBool Active;
};
} // namespace script
} // namespace av

#endif // #if !defined(AV_SCRIPT_UPDATE_H)
