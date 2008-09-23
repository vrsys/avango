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

#if !defined(AVANGO_TOOLS_TYPESELECTOR_H)
#define AVANGO_TOOLS_TYPESELECTOR_H

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
     * TypeSelector class selects targets from the types of given av::Objects.
     *
     * \ingroup av_tools
     */
    class TypeSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TypeSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TypeSelector();

    public:

      /**
       * Defines the input targets from which the objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines types by objects which may be selected. The output is in SelectedTargets.
       */
      MFObject SelectableTypes;

      /**
       * Defines additional types by objects which may come as output from another Selector.
       */
      MFTargetHolder SelectableTargetTypes;

      /**
       * Set to true if only equal types should be selectable.
       * Otherwise also derived types are selectable, which is the default.
       */
      SFBool EqualTypeOnly;

      /**
       * Set to true if the selectable types should also be searched in the OSG node paths
       * found in NodePathTargetHolder objects.
       */
      SFBool SearchTargetHolderNodePaths;

      /**
       * Set to true if the selectable types should also be searched in the OSG node paths.
       */
      SFBool SearchOSGNodePaths;

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    protected:

      bool isSelectable(const av::Object& object);

      std::set<Type> mTypes;
      bool mTypesDirty;
    };

    typedef SingleField<Link<TypeSelector> > SFTypeSelector;
    typedef MultiField<Link<TypeSelector> > MFTypeSelector;
  }
}

#endif
