#ifndef AVANGO_GUA_SCREEN_GRAB_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SCREEN_GRAB_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/ScreenGrabPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::ScreenGrabPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL ScreenGrabPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::ScreenGrabPassDescription is created.
     * Otherwise, the given ::gua::ScreenGrabPassDescription is used.
     */
    ScreenGrabPassDescription(
        std::shared_ptr<::gua::ScreenGrabPassDescription> const& ScreenGrabPassDescription = std::shared_ptr<::gua::ScreenGrabPassDescription>(new ::gua::ScreenGrabPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::ScreenGrabPassDescription.
     */
    std::shared_ptr<::gua::ScreenGrabPassDescription> const& getGuaScreenGrabPassDescription() const;

  private:
    std::shared_ptr<::gua::ScreenGrabPassDescription> m_guaScreenGrabPassDescription;

    ScreenGrabPassDescription(const ScreenGrabPassDescription&);
    ScreenGrabPassDescription& operator=(const ScreenGrabPassDescription&);
};

using SFScreenGrabPassDescription = SingleField<Link<ScreenGrabPassDescription>>;
using MFScreenGrabPassDescription = MultiField<Link<ScreenGrabPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::ScreenGrabPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::ScreenGrabPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SCREEN_GRAB_PASS_DESCRIPTION_HPP
