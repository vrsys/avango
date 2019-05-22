#ifndef AVANGO_GUA_TV_3_SURFACE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_TV_3_SURFACE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TV_3SurfacePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
namespace tv_3
{
/**
 * Wrapper for ::gua::TV_3SurfacePassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL TV_3SurfacePassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::TV_3SurfacePassDescription is created.
     * Otherwise, the given ::gua::TV_3SurfacePassDescription is used.
     */
    TV_3SurfacePassDescription(
        std::shared_ptr<::gua::TV_3SurfacePassDescription> const& TV_3SurfacePassDescription = std::shared_ptr<::gua::TV_3SurfacePassDescription>(new ::gua::TV_3SurfacePassDescription()));

  public:
    /**
     * Get the wrapped ::gua::TV_3SurfacePassDescription.
     */
    std::shared_ptr<::gua::TV_3SurfacePassDescription> const& getGuaTV_3SurfacePassDescription() const;

  private:
    std::shared_ptr<::gua::TV_3SurfacePassDescription> m_guaTV_3SurfacePassDescription;

    TV_3SurfacePassDescription(const TV_3SurfacePassDescription&);
    TV_3SurfacePassDescription& operator=(const TV_3SurfacePassDescription&);
};

using SFTV_3SurfacePassDescription = SingleField<Link<TV_3SurfacePassDescription>>;
using MFTV_3SurfacePassDescription = MultiField<Link<TV_3SurfacePassDescription>>;
} // namespace tv_3
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::TV_3SurfacePassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::TV_3SurfacePassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_TV_3_SURFACE_PASS_DESCRIPTION_HPP
