#include <avango/gua/skelanim/renderer/SkeletalAnimationPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SkeletalAnimationPassDescription"));
}

AV_FC_DEFINE(av::gua::SkeletalAnimationPassDescription);

AV_FIELD_DEFINE(av::gua::SFSkeletalAnimationPassDescription);
AV_FIELD_DEFINE(av::gua::MFSkeletalAnimationPassDescription);

av::gua::SkeletalAnimationPassDescription::SkeletalAnimationPassDescription(
  std::shared_ptr< ::gua::SkeletalAnimationPassDescription> const& guaSkeletalAnimationPassDescription)
    : PipelinePassDescription(guaSkeletalAnimationPassDescription)
    , m_guaSkeletalAnimationPassDescription(guaSkeletalAnimationPassDescription)
{

}

void
av::gua::SkeletalAnimationPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SkeletalAnimationPassDescription, true);

        SFSkeletalAnimationPassDescription::initClass("av::gua::SFSkeletalAnimationPassDescription", "av::Field");
        MFSkeletalAnimationPassDescription::initClass("av::gua::MFSkeletalAnimationPassDescription", "av::Field");
    }
}


std::shared_ptr< ::gua::SkeletalAnimationPassDescription> const&
av::gua::SkeletalAnimationPassDescription::getGuaSkeletalAnimationPassDescription() const
{
    return m_guaSkeletalAnimationPassDescription;
}


