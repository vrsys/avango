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

#if !defined(AVANGO_TOOLS_FIELDSELECTOR_H)
#define AVANGO_TOOLS_FIELDSELECTOR_H

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
     * FieldSelector class selects targets containing given field.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL FieldSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      FieldSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~FieldSelector();

    public:

      /**
       * Defines the input targets from which the named objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines the name which is used to select targets. The output is in SelectedTargets.
       */
      SFString SelectableFieldName;

      /**
       * Set to false if also objects should be selectable, which contain the given names
       * as substring. Otherwise only equal names are selectable, which is the default.
       */
      SFBool EqualNameOnly;

      /**
       * Specify, if the given strings in SelectableNames and SelectableTargetNames should be treated
       * as regular expressions. Default value: False.
       * If EqualNameOnly and TreatAsRegularExpression are both set to True, each regular expression is
       * surrounded by .*<Expression>.*
       */
      SFBool TreatAsRegularExpression;

      /**
       * Set to true if the selectable names should also be searched in the OSG node paths
       * found in NodePathTargetHolder objects.
       */
      SFBool SearchTargetHolderNodePaths;

      /**
       * Set to true if the selectable names should also be searched in the OSG node paths.
       */
      SFBool SearchOSGNodePaths;

      /* virtual */ void evaluate();

    protected:

      bool isSelectable(av::FieldContainer& object);
    };

    typedef SingleField<Link<FieldSelector> > SFFieldSelector;
    typedef MultiField<Link<FieldSelector> > MFFieldSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::FieldSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::FieldSelector> >;
#endif

}

#endif
