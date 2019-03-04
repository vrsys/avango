#ifndef AVANGO_TOOLS_DRAGTOOL_HPP
#define AVANGO_TOOLS_DRAGTOOL_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Tool.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/tools/windows_specific_tools.hpp>

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
    av::gua::SFMatrix4 DragTransform;

    /* virtual */ void evaluate();

  protected:
    /* virtual */ void evaluateKeptTarget(TargetHolder& holder);

    ::gua::math::mat4 mLastDragMat, mDragDiffMat;
    bool mDragged;
};

using SFDragTool = SingleField<Link<DragTool>>;
using MFDragTool = MultiField<Link<DragTool>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<::av::tools::DragTool>>;
template class AV_TOOLS_DLL MultiField<Link<::av::tools::DragTool>>;
#endif

} // namespace av

#endif // AVANGO_TOOLS_DRAGTOOL_HPP
