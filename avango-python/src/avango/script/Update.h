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

#if !defined(AV_SCRIPT_UPDATE_H)
#define AV_SCRIPT_UPDATE_H

#include <avango/python/script/Types.h>

#include <avango/Object.h>

namespace av
{
  namespace script
  {

    class Update : public av::Object
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
  }
}

#endif // #if !defined(AV_SCRIPT_UPDATE_H)
