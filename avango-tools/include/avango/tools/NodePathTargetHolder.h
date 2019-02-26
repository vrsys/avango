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

#if !defined(AVANGO_TOOLS_NODEPATHTARGETHOLDER_H)
#define AVANGO_TOOLS_NODEPATHTARGETHOLDER_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/TargetHolder.h>
#include <avango/osg/Node.h>
#include "windows_specific_tools.h"

namespace av
{
namespace tools
{
/**
 * NodePathTargetHolder class.
 *
 * A TargetHolder that holds a NodePath.
 * Sets the Keep value to true by default.
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL NodePathTargetHolder : public TargetHolder
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    NodePathTargetHolder();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~NodePathTargetHolder();

  public:
    /**
     * Outputs node path from root node to intersected node.
     */
    av::osg::MFNode NodePath;
};

using SFNodePathTargetHolder = SingleField<Link<NodePathTargetHolder>>;
using MFNodePathTargetHolder = MultiField<Link<NodePathTargetHolder>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::NodePathTargetHolder>>;
template class AV_TOOLS_DLL MultiField<Link<tools::NodePathTargetHolder>>;
#endif

} // namespace av

#endif
