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

#if !defined(AVANGO_TOOLS_DRAGTOOL_H)
#define AVANGO_TOOLS_DRAGTOOL_H

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Tool.h>
#include <avango/gua/Fields.hpp>
#include "windows_specific_tools.h"

namespace av
{
namespace tools
{
/**
 * Drag tool class
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL DragTool : public Tool
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    DragTool();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~DragTool();

  public:
    /**
     * Defines the dragging transformation relative to the root node coordinate system.
     */
    av::osg::SFMatrix DragTransform;

    /* virtual */ void evaluate();

  protected:
    /* virtual */ void evaluateKeptTarget(TargetHolder& holder);

    ::osg::Matrix mLastDragMat, mDragDiffMat;
    bool mDragged;
};

using SFDragTool = SingleField<Link<DragTool>>;
using MFDragTool = MultiField<Link<DragTool>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::DragTool>>;
template class AV_TOOLS_DLL MultiField<Link<tools::DragTool>>;
#endif

} // namespace av

#endif
