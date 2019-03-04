#ifndef AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/skelanim/renderer/SkeletalAnimationPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
namespace skelanim
{
/**
 * Wrapper for ::gua::SkeletalAnimationPassDescription
 *
 * \ingroup av_gua
 */
class AV_GUA_ANIM_DLL SkeletalAnimationPassDescription : public av::gua::PipelinePassDescription
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::SkeletalAnimationPassDescription is created.
     * Otherwise, the given ::gua::SkeletalAnimationPassDescription is used.
     */
    SkeletalAnimationPassDescription(std::shared_ptr<::gua::SkeletalAnimationPassDescription> const& SkeletalAnimationPassDescription =
                                         std::shared_ptr<::gua::SkeletalAnimationPassDescription>(new ::gua::SkeletalAnimationPassDescription()));

  public:
    /**
     * Get the wrapped ::gua::SkeletalAnimationPassDescription.
     */
    std::shared_ptr<::gua::SkeletalAnimationPassDescription> const& getGuaSkeletalAnimationPassDescription() const;

  private:
    std::shared_ptr<::gua::SkeletalAnimationPassDescription> m_guaSkeletalAnimationPassDescription;

    SkeletalAnimationPassDescription(const SkeletalAnimationPassDescription&);
    SkeletalAnimationPassDescription& operator=(const SkeletalAnimationPassDescription&);
};

typedef SingleField<Link<SkeletalAnimationPassDescription>> SFSkeletalAnimationPassDescription;
typedef MultiField<Link<SkeletalAnimationPassDescription>> MFSkeletalAnimationPassDescription;

} // namespace skelanim
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_ANIM_DLL SingleField<Link<gua::skelanim::SkeletalAnimationPassDescription>>;
template class AV_GUA_ANIM_DLL MultiField<Link<gua::skelanim::SkeletalAnimationPassDescription>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP
