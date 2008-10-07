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

#if !defined(AVANGO_TOOLS_SINGLESELECTOR_H)
#define AVANGO_TOOLS_SINGLESELECTOR_H

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
     * SingleSelector class passes one of multiple targets.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL SingleSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      SingleSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SingleSelector();

    public:

      /**
       * Defines the input targets from which one is passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Set to true to pass the last instead of the first target to SelectedTargets.
       */
      SFBool LastTarget;

      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<SingleSelector> > SFSingleSelector;
    typedef MultiField<Link<SingleSelector> > MFSingleSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::SingleSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::SingleSelector> >;
#endif

}

#endif
