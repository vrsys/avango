#ifndef AVANGO_TOOLS_TOOL_HPP
#define AVANGO_TOOLS_TOOL_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/FieldContainer.h>
#include <avango/tools/TargetHolder.hpp>
#include <avango/tools/windows_specific_tools.hpp>

namespace av
{
namespace tools
{
/**
 * Abstract tool base class
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL Tool : public av::FieldContainer
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

using SFTool = SingleField<Link<Tool>>;
using MFTool = MultiField<Link<Tool>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<::av::tools::Tool>>;
template class AV_TOOLS_DLL MultiField<Link<::av::tools::Tool>>;
#endif

} // namespace av

#endif // AVANGO_TOOLS_TOOL_HPP
