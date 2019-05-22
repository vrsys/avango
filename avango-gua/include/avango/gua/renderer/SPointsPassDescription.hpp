#ifndef AVANGO_GUA_SPOINTS_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SPOINTS_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/spoints/SPointsPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::SPointsPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL SPointsPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::SPointsPassDescription is created.
     * Otherwise, the given ::gua::SPointsPassDescription is used.
     */
    SPointsPassDescription(std::shared_ptr<::gua::SPointsPassDescription> const& SPointsPassDescription = std::shared_ptr<::gua::SPointsPassDescription>(new ::gua::SPointsPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::SPointsPassDescription.
     */
    std::shared_ptr<::gua::SPointsPassDescription> const& getGuaSPointsPassDescription() const;

  private:
    std::shared_ptr<::gua::SPointsPassDescription> m_guaSPointsPassDescription;

    SPointsPassDescription(const SPointsPassDescription&);
    SPointsPassDescription& operator=(const SPointsPassDescription&);
};

using SFSPointsPassDescription = SingleField<Link<SPointsPassDescription>>;
using MFSPointsPassDescription = MultiField<Link<SPointsPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::SPointsPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::SPointsPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SPOINTS_PASS_DESCRIPTION_HPP
