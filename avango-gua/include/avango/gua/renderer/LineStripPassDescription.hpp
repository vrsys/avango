#ifndef AVANGO_GUA_LINE_STRIP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_LINE_STRIP_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/LineStripPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::LineStripPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL LineStripPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::LineStripPassDescription is created.
     * Otherwise, the given ::gua::LineStripPassDescription is used.
     */
    LineStripPassDescription(
        std::shared_ptr<::gua::LineStripPassDescription> const& LineStripPassDescription = std::shared_ptr<::gua::LineStripPassDescription>(new ::gua::LineStripPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::LineStripPassDescription.
     */
    std::shared_ptr<::gua::LineStripPassDescription> const& getGuaLineStripPassDescription() const;

  private:
    std::shared_ptr<::gua::LineStripPassDescription> m_guaLineStripPassDescription;

    LineStripPassDescription(const LineStripPassDescription&);
    LineStripPassDescription& operator=(const LineStripPassDescription&);
};

using SFLineStripPassDescription = SingleField<Link<LineStripPassDescription>>;
using MFLineStripPassDescription = MultiField<Link<LineStripPassDescription>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::LineStripPassDescription>>;
template class AV_GUA_DLL MultiField<Link<gua::LineStripPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_LINE_STRIP_PASS_DESCRIPTION_HPP
