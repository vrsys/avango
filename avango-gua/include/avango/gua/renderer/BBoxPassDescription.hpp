#ifndef AVANGO_GUA_BBOX_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_BBOX_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/BBoxPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::BBoxPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL BBoxPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::BBoxPassDescription is created.
     * Otherwise, the given ::gua::BBoxPassDescription is used.
     */
    BBoxPassDescription(std::shared_ptr<::gua::BBoxPassDescription> const& BBoxPassDescription = std::shared_ptr<::gua::BBoxPassDescription>(new ::gua::BBoxPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::BBoxPassDescription.
     */
    std::shared_ptr<::gua::BBoxPassDescription> const& getGuaBBoxPassDescription() const;

  private:
    std::shared_ptr<::gua::BBoxPassDescription> m_guaBBoxPassDescription;

    BBoxPassDescription(const BBoxPassDescription&);
    BBoxPassDescription& operator=(const BBoxPassDescription&);
};

using SFBBoxPassDescription = SingleField<Link<BBoxPassDescription>>;
using MFBBoxPassDescription = MultiField<Link<BBoxPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::BBoxPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::BBoxPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_BBOX_PASS_DESCRIPTION_HPP
