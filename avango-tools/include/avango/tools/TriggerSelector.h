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

#if !defined(AVANGO_TOOLS_TRIGGERSELECTOR_H)
#define AVANGO_TOOLS_TRIGGERSELECTOR_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.h>

namespace av
{
  namespace tools
  {
    /**
     * TriggerSelector class passes targets if triggered.
     *
     * \ingroup av_tools
     */
    class TriggerSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TriggerSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TriggerSelector();

    public:

      /**
       * Defines the input targets which are passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Set to true to pass Targets to SelectedTargets.
       */
      SFBool Trigger;

      /**
       * Set to true, if SelectedTargets should only be updated on Trigger transitions.
       */
      SFBool TransitionOnly;

      /* virtual */ void evaluate();

    protected:

      bool mLastTrigger;

    };

    typedef SingleField<Link<TriggerSelector> > SFTriggerSelector;
    typedef MultiField<Link<TriggerSelector> > MFTriggerSelector;
  }
}

#endif
