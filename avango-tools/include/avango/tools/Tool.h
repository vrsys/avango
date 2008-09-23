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

#if !defined(AVANGO_TOOLS_TOOL_H)
#define AVANGO_TOOLS_TOOL_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/Object.h>

#include <avango/tools/TargetHolder.h>

namespace av
{
  namespace tools
  {
    /**
     * Abstract tool base class
     *
     * \ingroup av_tools
     */
    class Tool : public av::Object
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor.
       */
      Tool();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Tool();

    public:

      /**
       * The tool will operate on the targets defined by this multifield.
       */
      MFTargetHolder Targets;

      /* virtual */ void evaluate();

    protected:

      /**
       * evaluateTarget is called from evaluate for each target.
       */
      virtual void evaluateTarget(TargetHolder& holder);

      /**
       * evaluateAddedTarget is called from evaluate for each new target.
       */
      virtual void evaluateAddedTarget(TargetHolder& holder);

      /**
       * evaluateKeptTarget is called from evaluate for each target
       * which remained in the Target list since the last evaluate.
       */
      virtual void evaluateKeptTarget(TargetHolder& holder);

      /**
       * evaluateRemovedTarget is called from evaluate for each target
       * which was removed from the Target list since the last evaluate.
       */
      virtual void evaluateRemovedTarget(TargetHolder& holder);

      MFTargetHolder::ContainerType mLastTargets;
    };

    typedef SingleField<Link<Tool> > SFTool;
    typedef MultiField<Link<Tool> > MFTool;
  }
}

#endif
