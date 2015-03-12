#ifndef AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/SkeletalAnimationPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SkeletalAnimationPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SkeletalAnimationPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SkeletalAnimationPassDescription is created.
       * Otherwise, the given ::gua::SkeletalAnimationPassDescription is used.
       */
      SkeletalAnimationPassDescription(std::shared_ptr< ::gua::SkeletalAnimationPassDescription> const& SkeletalAnimationPassDescription =
                             std::shared_ptr< ::gua::SkeletalAnimationPassDescription>(new ::gua::SkeletalAnimationPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::SkeletalAnimationPassDescription.
       */
      std::shared_ptr< ::gua::SkeletalAnimationPassDescription> const& getGuaSkeletalAnimationPassDescription() const;

    private:

      std::shared_ptr< ::gua::SkeletalAnimationPassDescription> m_guaSkeletalAnimationPassDescription;


      SkeletalAnimationPassDescription(const SkeletalAnimationPassDescription&);
      SkeletalAnimationPassDescription& operator=(const SkeletalAnimationPassDescription&);
    };

    typedef SingleField<Link<SkeletalAnimationPassDescription> > SFSkeletalAnimationPassDescription;
    typedef MultiField<Link<SkeletalAnimationPassDescription> > MFSkeletalAnimationPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SkeletalAnimationPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::SkeletalAnimationPassDescription> >;
#endif

}

#endif //AVANGO_GUA_SKEL_ANIM_PASS_DESCRIPTION_HPP
