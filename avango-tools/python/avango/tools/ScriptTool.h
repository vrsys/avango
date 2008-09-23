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

#if !defined(AVANGO_TOOLS_SCRIPTTOOL_H)
#define AVANGO_TOOLS_SCRIPTTOOL_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Tool.h>

#include <avango/python/script/Types.h>

namespace av
{
  namespace tools
  {
    /**
     * Script tool class
     *
     * \ingroup av_tools
     */
    class ScriptTool : public Tool
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      ScriptTool();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ScriptTool();

    public:

      av::script::SFObject TargetCallback;
      av::script::SFObject AddedTargetCallback;
      av::script::SFObject KeptTargetCallback;
      av::script::SFObject RemovedTargetCallback;

    protected:

      /* virtual */ void evaluateTarget(TargetHolder& holder);
      /* virtual */ void evaluateAddedTarget(TargetHolder& holder);
      /* virtual */ void evaluateKeptTarget(TargetHolder& holder);
      /* virtual */ void evaluateRemovedTarget(TargetHolder& holder);

    };

    typedef SingleField<Link<ScriptTool> > SFScriptTool;
    typedef MultiField<Link<ScriptTool> > MFScriptTool;
  }
}

#endif
