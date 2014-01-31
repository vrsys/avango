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

#if !defined(AVANGO_SHADE_STATEGROUP_H)
#define AVANGO_SHADE_STATEGROUP_H

/**
 * \file
 * \ingroup av_shade
 */

#include <avango/osg/Group.h>
#include <osg/NodeCallback>
#include "State.h"
#include "windows_specific_shade.h"

namespace av
{
  namespace shade
  {
    /**
     * StateGroup references a State and applies the shader to its subgraph
     *
     * \ingroup av_shade
     */
    class AV_SHADE_DLL StateGroup : public av::osg::Group
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      StateGroup();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~StateGroup();

    public:

      /**
       * State to apply to subgraph
       */
      SFState State;

      /* virtual */ void evaluate();

    private:

      class UpdateCallback : public ::osg::NodeCallback
      {
        /*virtual*/ void operator() (::osg::Node *node, ::osg::NodeVisitor *nv);

      private:
        void callUpdate(::osg::Node *node) const;
      };
    };
  }
}

#endif
