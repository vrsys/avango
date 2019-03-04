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

#if !defined(AVANGO_TOOLS_SELECTOR_H)
#define AVANGO_TOOLS_SELECTOR_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/FieldContainer.h>
#include <avango/tools/TargetHolder.h>
#include "windows_specific_tools.h"

namespace av
{
namespace tools
{
/**
 * Abstract selector base class
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL Selector : public av::FieldContainer
{
    AV_FC_DECLARE_ABSTRACT();

  public:
    /**
     * Constructor.
     */
    Selector();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Selector();

  public:
    /**
     * This multifield outputs which targets are currently selected by the Selector.
     */
    MFTargetHolder SelectedTargets;

    Link<TargetHolder> combine(const Link<TargetHolder> holder1, const Link<TargetHolder> holder2);
};

using SFSelector = SingleField<Link<Selector>>;
using MFSelector = MultiField<Link<Selector>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::Selector>>;
template class AV_TOOLS_DLL MultiField<Link<tools::Selector>>;
#endif

} // namespace av

#endif
