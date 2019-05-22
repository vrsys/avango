#ifndef AVANGO_TOOLS_TRIGGERSELECTOR_HPP
#define AVANGO_TOOLS_TRIGGERSELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.hpp>
#include <avango/tools/windows_specific_tools.hpp>

namespace av
{
namespace tools
{
/**
 * TriggerSelector class passes targets if triggered.
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL TriggerSelector : public Selector
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    TriggerSelector();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~TriggerSelector();

  public:
    /**
     * Defines the input targets which are passed to SelectedTargets.
     */
    MFTargetHolder Targets;

    /**
     * Set to true to pass Targets to SelectedTargets.
     */
    SFBool Trigger;

    /**
     * Set to true, if SelectedTargets should only be updated on Trigger transitions.
     */
    SFBool TransitionOnly;

    /* virtual */ void evaluate();

  protected:
    bool mLastTrigger;
};

using SFTriggerSelector = SingleField<Link<TriggerSelector>>;
using MFTriggerSelector = MultiField<Link<TriggerSelector>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::TriggerSelector>>;
template class AV_TOOLS_DLL MultiField<Link<tools::TriggerSelector>>;
#endif

} // namespace av

#endif
