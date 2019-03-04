#include <avango/gua/skelanim/renderer/SkeletalAnimationPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::skelanim::SkeletalAnimationPassDescription"));
}

AV_FC_DEFINE(av::gua::skelanim::SkeletalAnimationPassDescription);

AV_FIELD_DEFINE(av::gua::skelanim::SFSkeletalAnimationPassDescription);
AV_FIELD_DEFINE(av::gua::skelanim::MFSkeletalAnimationPassDescription);

av::gua::skelanim::SkeletalAnimationPassDescription::SkeletalAnimationPassDescription(std::shared_ptr<::gua::SkeletalAnimationPassDescription> const& guaSkeletalAnimationPassDescription)
    : PipelinePassDescription(guaSkeletalAnimationPassDescription), m_guaSkeletalAnimationPassDescription(guaSkeletalAnimationPassDescription)
{
}

void av::gua::skelanim::SkeletalAnimationPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::skelanim::SkeletalAnimationPassDescription, true);

        SFSkeletalAnimationPassDescription::initClass("av::gua::skelanim::SFSkeletalAnimationPassDescription", "av::Field");
        MFSkeletalAnimationPassDescription::initClass("av::gua::skelanim::MFSkeletalAnimationPassDescription", "av::Field");
    }
}

std::shared_ptr<::gua::SkeletalAnimationPassDescription> const& av::gua::skelanim::SkeletalAnimationPassDescription::getGuaSkeletalAnimationPassDescription() const
{
    return m_guaSkeletalAnimationPassDescription;
}
