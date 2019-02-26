#include <avango/gua/renderer/LightVisibilityPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::LightVisibilityPassDescription"));
}

AV_FC_DEFINE(av::gua::LightVisibilityPassDescription);

AV_FIELD_DEFINE(av::gua::SFLightVisibilityPassDescription);
AV_FIELD_DEFINE(av::gua::MFLightVisibilityPassDescription);

av::gua::LightVisibilityPassDescription::LightVisibilityPassDescription(std::shared_ptr<::gua::LightVisibilityPassDescription> const& guaLightVisibilityPassDescription)
    : PipelinePassDescription(guaLightVisibilityPassDescription), m_guaLightVisibilityPassDescription(guaLightVisibilityPassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(RasterizationMode,
                            std::bind(&LightVisibilityPassDescription::getRasterizationModeCB, this, std::placeholders::_1),
                            std::bind(&LightVisibilityPassDescription::setRasterizationModeCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        TilePower, std::bind(&LightVisibilityPassDescription::getTilePowerCB, this, std::placeholders::_1), std::bind(&LightVisibilityPassDescription::setTilePowerCB, this, std::placeholders::_1));
}

void av::gua::LightVisibilityPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::LightVisibilityPassDescription, true);

        SFLightVisibilityPassDescription::initClass("av::gua::SFLightVisibilityPassDescription", "av::Field");
        MFLightVisibilityPassDescription::initClass("av::gua::MFLightVisibilityPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void av::gua::LightVisibilityPassDescription::getRasterizationModeCB(const SFUInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = static_cast<unsigned>(m_guaLightVisibilityPassDescription->rasterization_mode());
}

void av::gua::LightVisibilityPassDescription::setRasterizationModeCB(const SFUInt::SetValueEvent& event)
{
    m_guaLightVisibilityPassDescription->rasterization_mode(static_cast<::gua::LightVisibilityPassDescription::RasterizationMode>(event.getValue()));
}

void av::gua::LightVisibilityPassDescription::getTilePowerCB(const SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLightVisibilityPassDescription->tile_power(); }

void av::gua::LightVisibilityPassDescription::setTilePowerCB(const SFUInt::SetValueEvent& event) { m_guaLightVisibilityPassDescription->tile_power(event.getValue()); }

std::shared_ptr<::gua::LightVisibilityPassDescription> const& av::gua::LightVisibilityPassDescription::getGuaLightVisibilityPassDescription() const { return m_guaLightVisibilityPassDescription; }
