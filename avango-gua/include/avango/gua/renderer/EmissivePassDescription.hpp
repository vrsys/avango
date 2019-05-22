#ifndef AVANGO_GUA_EMISSIVE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_EMISSIVE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/EmissivePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::EmissivePassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL EmissivePassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::EmissivePassDescription is created.
     * Otherwise, the given ::gua::EmissivePassDescription is used.
     */
    EmissivePassDescription(std::shared_ptr<::gua::EmissivePassDescription> const& EmissivePassDescription = std::shared_ptr<::gua::EmissivePassDescription>(new ::gua::EmissivePassDescription()));

  public:
    /**
     * Get the wrapped ::gua::EmissivePassDescription.
     */
    std::shared_ptr<::gua::EmissivePassDescription> const& getGuaEmissivePassDescription() const;

  private:
    std::shared_ptr<::gua::EmissivePassDescription> m_guaEmissivePassDescription;

    EmissivePassDescription(const EmissivePassDescription&);
    EmissivePassDescription& operator=(const EmissivePassDescription&);
};

using SFEmissivePassDescription = SingleField<Link<EmissivePassDescription>>;
using MFEmissivePassDescription = MultiField<Link<EmissivePassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::EmissivePassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::EmissivePassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_EMISSIVE_PASS_DESCRIPTION_HPP
