#ifndef AVANGO_TOOLS_CHANGESELECTOR_HPP
#define AVANGO_TOOLS_CHANGESELECTOR_HPP

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
 * ChangeSelector class passes targets which were added since the last evaluate
 * and additional outputs removed targets.
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL ChangeSelector : public Selector
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    ChangeSelector();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ChangeSelector();

  public:
    /**
     * Defines the input targets whose added targets are passed to SelectedTargets.
     */
    MFTargetHolder Targets;

    /**
     * Outputs the targets which were removed from Targets since the last evaluate.
     */
    MFTargetHolder KeptTargets;

    /**
     * Outputs the targets which were removed from Targets since the last evaluate.
     */
    MFTargetHolder RemovedTargets;

    /* virtual */ void evaluate();

  protected:
    MFTargetHolder::ContainerType mLastTargets;
};

using SFChangeSelector = SingleField<Link<ChangeSelector>>;
using MFChangeSelector = MultiField<Link<ChangeSelector>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::ChangeSelector>>;
template class AV_TOOLS_DLL MultiField<Link<tools::ChangeSelector>>;
#endif

} // namespace av

#endif // AVANGO_TOOLS_CHANGESELECTOR_HPP
