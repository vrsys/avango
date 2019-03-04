#ifndef AVANGO_GUA_OCCLUSION_SLAVE_RESOLVE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_OCCLUSION_SLAVE_RESOLVE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/OcclusionSlaveResolvePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::OcclusionSlaveResolvePassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL OcclusionSlaveResolvePassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::OcclusionSlaveResolvePassDescription is created.
     * Otherwise, the given ::gua::OcclusionSlaveResolvePassDescription is used.
     */
    OcclusionSlaveResolvePassDescription(std::shared_ptr<::gua::OcclusionSlaveResolvePassDescription> const& OcclusionSlaveResolvePassDescription =
                                             std::shared_ptr<::gua::OcclusionSlaveResolvePassDescription>(new ::gua::OcclusionSlaveResolvePassDescription()));

  public:
    /**
     * Get the wrapped ::gua::ResolvePassDescription.
     */
    std::shared_ptr<::gua::OcclusionSlaveResolvePassDescription> const& getGuaOcclusionSlaveResolvePassDescription() const;

  private:
    std::shared_ptr<::gua::OcclusionSlaveResolvePassDescription> m_guaOcclusionSlaveResolvePassDescription;

    OcclusionSlaveResolvePassDescription(const OcclusionSlaveResolvePassDescription&);
    OcclusionSlaveResolvePassDescription& operator=(const OcclusionSlaveResolvePassDescription&);
};

using SFOcclusionSlaveResolvePassDescription = SingleField<Link<OcclusionSlaveResolvePassDescription>>;
using MFOcclusionSlaveResolvePassDescription = MultiField<Link<OcclusionSlaveResolvePassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::OcclusionSlaveResolvePassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::OcclusionSlaveResolvePassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_OCCLUSION_SLAVE_OCCLUSION_SLAVE_RESOLVE_PASS_DESCRIPTION_HPP
