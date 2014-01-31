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

#if !defined(AVANGO_TOOLS_INJECTSELECTOR_H)
#define AVANGO_TOOLS_INJECTSELECTOR_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.h>
#include "windows_specific_tools.h"

namespace av
{
  namespace tools
  {
    /**
     * InjectSelector class adds or replaces input target holders with given target holders,
     * if the targets match.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL InjectSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      InjectSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~InjectSelector();

    public:

      /**
       * Defines the input targets.
       */
      MFTargetHolder Targets;

      /**
       * Defines target holder which replace or which are added to the input targets.
       */
      MFTargetHolder InjectTargets;

      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<InjectSelector> > SFInjectSelector;
    typedef MultiField<Link<InjectSelector> > MFInjectSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::InjectSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::InjectSelector> >;
#endif

}

#endif
