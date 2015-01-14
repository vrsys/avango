#include <avango/gua/renderer/ResolvePassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ResolvePassDescription"));
}

AV_FC_DEFINE(av::gua::ResolvePassDescription);

AV_FIELD_DEFINE(av::gua::SFResolvePassDescription);
AV_FIELD_DEFINE(av::gua::MFResolvePassDescription);

av::gua::ResolvePassDescription::ResolvePassDescription(
  std::shared_ptr< ::gua::ResolvePassDescription> const& guaResolvePassDescription)
    : PipelinePassDescription(guaResolvePassDescription)
    , m_guaResolvePassDescription(guaResolvePassDescription)
{
  AV_FC_ADD_ADAPTOR_FIELD(BackgroundColor,
                    boost::bind(&ResolvePassDescription::getBackgroundColorCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundColorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundTexture,
                    boost::bind(&ResolvePassDescription::getBackgroundTextureCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundTextureCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundMode,
                    boost::bind(&ResolvePassDescription::getBackgroundModeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableFog,
                    boost::bind(&ResolvePassDescription::getEnableFogCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnableFogCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(FogStart,
                    boost::bind(&ResolvePassDescription::getFogStartCB, this, _1),
                    boost::bind(&ResolvePassDescription::setFogStartCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(FogEnd,
                    boost::bind(&ResolvePassDescription::getFogEndCB, this, _1),
                    boost::bind(&ResolvePassDescription::setFogEndCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ToneMappingMode,
                    boost::bind(&ResolvePassDescription::getToneMappingModeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setToneMappingModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Exposure,
                    boost::bind(&ResolvePassDescription::getExposureCB, this, _1),
                    boost::bind(&ResolvePassDescription::setExposureCB, this, _1));

}

void
av::gua::ResolvePassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::ResolvePassDescription, true);

        SFResolvePassDescription::initClass("av::gua::SFResolvePassDescription", "av::Field");
        MFResolvePassDescription::initClass("av::gua::MFResolvePassDescription", "av::Field");
    }
}

void
av::gua::ResolvePassDescription::getBackgroundColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->color();
}

void
av::gua::ResolvePassDescription::setBackgroundColorCB(const SFColor::SetValueEvent& event)
{
  m_guaResolvePassDescription->color(event.getValue());
}

void
av::gua::ResolvePassDescription::getBackgroundTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->texture();
}

void
av::gua::ResolvePassDescription::setBackgroundTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getBackgroundModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaResolvePassDescription->mode());
}

void
av::gua::ResolvePassDescription::setBackgroundModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaResolvePassDescription->mode(static_cast< ::gua::ResolvePassDescription::BackgroundMode>(event.getValue()));
}

void
av::gua::ResolvePassDescription::getEnableFogCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->enable_fog();
}

void
av::gua::ResolvePassDescription::setEnableFogCB(const SFBool::SetValueEvent& event)
{
  m_guaResolvePassDescription->enable_fog(event.getValue());
}

void
av::gua::ResolvePassDescription::getFogStartCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->fog_start();
}

void
av::gua::ResolvePassDescription::setFogStartCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->fog_start(event.getValue());
}

void
av::gua::ResolvePassDescription::getFogEndCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->fog_end();
}

void
av::gua::ResolvePassDescription::setFogEndCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->fog_end(event.getValue());
}

void
av::gua::ResolvePassDescription::getToneMappingModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaResolvePassDescription->tone_mapping_method());
}

void
av::gua::ResolvePassDescription::setToneMappingModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaResolvePassDescription->tone_mapping_method(static_cast< ::gua::ResolvePassDescription::ToneMappingMethod>(event.getValue()));
}

void
av::gua::ResolvePassDescription::getExposureCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->tone_mapping_exposure();
}

void
av::gua::ResolvePassDescription::setExposureCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->tone_mapping_exposure(event.getValue());
}

std::shared_ptr< ::gua::ResolvePassDescription> const&
av::gua::ResolvePassDescription::getGuaResolvePassDescription() const
{
    return m_guaResolvePassDescription;
}


