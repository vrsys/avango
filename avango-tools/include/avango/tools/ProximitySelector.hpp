#ifndef AVANGO_TOOLS_PROXIMITYSELECTOR_HPP
#define AVANGO_TOOLS_PROXIMITYSELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/Node.hpp>

#include <avango/tools/windows_specific_tools.hpp>

namespace av
{
namespace tools
{
/**
 * ProximitySelector class selects targets from given targets
 * which have less than a specific distance to a given position.
 *
 * \ingroup av_tools
 */
class AV_TOOLS_DLL ProximitySelector : public Selector
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor.
     */
    ProximitySelector();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ProximitySelector();

  public:
    /**
     * Defines the input objects whose distances will be obversed.
     */
    MFContainer TargetObjects;

    /**
     * Defines additional input targets which may come as output from another Selector.
     */
    MFTargetHolder Targets;

    /**
     * Defines the reference position for the object distances relative to
     * the coordinate system specified by PositionTransform. Default is (0, 0, 0).
     */
    av::gua::SFVec3 Position;

    /**
     * Defines the position transformation relative to the root node coordinate system.
     */
    av::gua::SFMatrix PositionTransform;

    /**
     * Objects inside this radius are added to the selected targets. Default is 1.0.
     */
    SFDouble ProximityRadius;

    /**
     * Objects outside this radius are removed from the selected targets. Default is 2.0.
     */
    SFDouble DistanceRadius;

    /**
     * Time input field is required for timeouts.
     */
    SFDouble Time;

    /**
     * Objects have to stay this time in- or outside the given radiuses
     * before the will be added to or removed from the selected targets.
     * A value of 0.0 seconds disables the lag, which is the default.
     */
    SFDouble ProximityLag;

    /**
     * Defines the maximum number of concurrently selected targets.
     * Most recently added have priority.
     * The default of zero means an unlimited number of targets.
     */
    SFUInt MaxNumberOfTargets;

    /* virtual */ void evaluate();

  protected:
    using TargetTimePair_t = std::pair<Link<av::gua::Node>, double>;
    using TargetTimeList_t = std::list<TargetTimePair_t>;

    static inline TargetTimeList_t::iterator find(TargetTimeList_t& candidates, const SFContainer::ValueType& target)
    {
        TargetTimeList_t::iterator candidate = candidates.begin();
        while(candidate != candidates.end() && candidate->first != target)
            ++candidate;
        return candidate;
    }

    inline bool hasTarget(TargetTimeList_t& candidates, const SFContainer::ValueType& target) { return (find(candidates, target) != candidates.end()); }

    TargetTimeList_t mProxCands, mDistCands;
    MFTargetHolder::ContainerType mSelTargets;
};

using SFProximitySelector = SingleField<Link<ProximitySelector>>;
using MFProximitySelector = MultiField<Link<ProximitySelector>>;
} // namespace tools

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_TOOLS_DLL SingleField<Link<tools::ProximitySelector>>;
template class AV_TOOLS_DLL MultiField<Link<tools::ProximitySelector>>;
#endif

} // namespace av

#endif // AVANGO_TOOLS_PROXIMITYSELECTOR_HPP
