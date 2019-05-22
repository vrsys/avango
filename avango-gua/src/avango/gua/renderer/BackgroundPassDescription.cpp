#include <avango/gua/renderer/BackgroundPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::BackgroundPassDescription"));
}

AV_FC_DEFINE(av::gua::BackgroundPassDescription);

AV_FIELD_DEFINE(av::gua::SFBackgroundPassDescription);
AV_FIELD_DEFINE(av::gua::MFBackgroundPassDescription);

av::gua::BackgroundPassDescription::BackgroundPassDescription(std::shared_ptr<::gua::BackgroundPassDescription> const& guaBackgroundPassDescription)
    : PipelinePassDescription(guaBackgroundPassDescription), m_guaBackgroundPassDescription(guaBackgroundPassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(Color, std::bind(&BackgroundPassDescription::getColorCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setColorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        Texture, std::bind(&BackgroundPassDescription::getTextureCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setTextureCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Mode, std::bind(&BackgroundPassDescription::getModeCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setModeCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableFog, std::bind(&BackgroundPassDescription::getEnableFogCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setEnableFogCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        FogStart, std::bind(&BackgroundPassDescription::getFogStartCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setFogStartCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FogEnd, std::bind(&BackgroundPassDescription::getFogEndCB, this, std::placeholders::_1), std::bind(&BackgroundPassDescription::setFogEndCB, this, std::placeholders::_1));
}

void av::gua::BackgroundPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::BackgroundPassDescription, true);

        SFBackgroundPassDescription::initClass("av::gua::SFBackgroundPassDescription", "av::Field");
        MFBackgroundPassDescription::initClass("av::gua::MFBackgroundPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void av::gua::BackgroundPassDescription::getColorCB(const SFColor::GetValueEvent& event) { *(event.getValuePtr()) = m_guaBackgroundPassDescription->color(); }

void av::gua::BackgroundPassDescription::setColorCB(const SFColor::SetValueEvent& event) { m_guaBackgroundPassDescription->color(event.getValue()); }

void av::gua::BackgroundPassDescription::getTextureCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaBackgroundPassDescription->texture(); }

void av::gua::BackgroundPassDescription::setTextureCB(const SFString::SetValueEvent& event) { m_guaBackgroundPassDescription->texture(event.getValue()); }

void av::gua::BackgroundPassDescription::getModeCB(const SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = static_cast<unsigned>(m_guaBackgroundPassDescription->mode()); }

void av::gua::BackgroundPassDescription::setModeCB(const SFUInt::SetValueEvent& event)
{
    m_guaBackgroundPassDescription->mode(static_cast<::gua::BackgroundPassDescription::BackgroundMode>(event.getValue()));
}

void av::gua::BackgroundPassDescription::getEnableFogCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaBackgroundPassDescription->enable_fog(); }

void av::gua::BackgroundPassDescription::setEnableFogCB(const SFBool::SetValueEvent& event) { m_guaBackgroundPassDescription->enable_fog(event.getValue()); }

void av::gua::BackgroundPassDescription::getFogStartCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaBackgroundPassDescription->fog_start(); }

void av::gua::BackgroundPassDescription::setFogStartCB(const SFFloat::SetValueEvent& event) { m_guaBackgroundPassDescription->fog_start(event.getValue()); }

void av::gua::BackgroundPassDescription::getFogEndCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaBackgroundPassDescription->fog_end(); }

void av::gua::BackgroundPassDescription::setFogEndCB(const SFFloat::SetValueEvent& event) { m_guaBackgroundPassDescription->fog_end(event.getValue()); }

std::shared_ptr<::gua::BackgroundPassDescription> const& av::gua::BackgroundPassDescription::getGuaBackgroundPassDescription() const { return m_guaBackgroundPassDescription; }
