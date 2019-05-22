#ifndef AVANGO_GUA_DEBUG_VIEW_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_DEBUG_VIEW_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/DebugViewPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::DebugViewPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL DebugViewPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::DebugViewPassDescription is created.
     * Otherwise, the given ::gua::DebugViewPassDescription is used.
     */
    DebugViewPassDescription(
        std::shared_ptr<::gua::DebugViewPassDescription> const& DebugViewPassDescription = std::shared_ptr<::gua::DebugViewPassDescription>(new ::gua::DebugViewPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::DebugViewPassDescription.
     */
    std::shared_ptr<::gua::DebugViewPassDescription> const& getGuaDebugViewPassDescription() const;

  private:
    std::shared_ptr<::gua::DebugViewPassDescription> m_guaDebugViewPassDescription;

    DebugViewPassDescription(const DebugViewPassDescription&);
    DebugViewPassDescription& operator=(const DebugViewPassDescription&);
};

using SFDebugViewPassDescription = SingleField<Link<DebugViewPassDescription>>;
using MFDebugViewPassDescription = MultiField<Link<DebugViewPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::DebugViewPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::DebugViewPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_DEBUG_VIEW_PASS_DESCRIPTION_HPP
