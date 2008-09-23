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

#if !defined(AVANGO_TOOLS_CHANGESELECTOR_H)
#define AVANGO_TOOLS_CHANGESELECTOR_H

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
     * ChangeSelector class passes targets which were added since the last evaluate
     * and additional outputs removed targets.
     *
     * \ingroup av_tools
     */
    class ChangeSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      ChangeSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ChangeSelector();

    public:

      /**
       * Defines the input targets whose added targets are passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Outputs the targets which were removed from Targets since the last evaluate.
       */
      MFTargetHolder KeptTargets;

      /**
       * Outputs the targets which were removed from Targets since the last evaluate.
       */
      MFTargetHolder RemovedTargets;

      /* virtual */ void evaluate();

    protected:

      MFTargetHolder::ContainerType mLastTargets;

    };

    typedef SingleField<Link<ChangeSelector> > SFChangeSelector;
    typedef MultiField<Link<ChangeSelector> > MFChangeSelector;
  }
}

#endif
