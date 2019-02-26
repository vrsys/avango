#ifndef AVANGO_GUA_VIDEO3D_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_VIDEO3D_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/video3d/Video3DPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::Video3DPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Video3DPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::Video3DPassDescription is created.
     * Otherwise, the given ::gua::Video3DPassDescription is used.
     */
    Video3DPassDescription(std::shared_ptr<::gua::Video3DPassDescription> const& Video3DPassDescription = std::shared_ptr<::gua::Video3DPassDescription>(new ::gua::Video3DPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::Video3DPassDescription.
     */
    std::shared_ptr<::gua::Video3DPassDescription> const& getGuaVideo3DPassDescription() const;

  private:
    std::shared_ptr<::gua::Video3DPassDescription> m_guaVideo3DPassDescription;

    Video3DPassDescription(const Video3DPassDescription&);
    Video3DPassDescription& operator=(const Video3DPassDescription&);
};

using SFVideo3DPassDescription = SingleField<Link<Video3DPassDescription>>;
using MFVideo3DPassDescription = MultiField<Link<Video3DPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::Video3DPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::Video3DPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_VIDEO3D_PASS_DESCRIPTION_HPP
