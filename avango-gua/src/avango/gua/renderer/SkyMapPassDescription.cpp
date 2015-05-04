#include <avango/gua/renderer/SkyMapPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SkyMapPassDescription"));
}

AV_FC_DEFINE(av::gua::SkyMapPassDescription);

AV_FIELD_DEFINE(av::gua::SFSkyMapPassDescription);
AV_FIELD_DEFINE(av::gua::MFSkyMapPassDescription);

av::gua::SkyMapPassDescription::SkyMapPassDescription(
  std::shared_ptr< ::gua::SkyMapPassDescription> const& guaSkyMapPassDescription)
    : PipelinePassDescription(guaSkyMapPassDescription)
    , m_guaSkyMapPassDescription(guaSkyMapPassDescription)
{
  AV_FC_ADD_ADAPTOR_FIELD(OutputTextureName,
                    boost::bind(&SkyMapPassDescription::getOutputTextureNameCB, this, _1),
                    boost::bind(&SkyMapPassDescription::setOutputTextureNameCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LightColor,
                    boost::bind(&SkyMapPassDescription::getLightColorCB, this, _1),
                    boost::bind(&SkyMapPassDescription::setLightColorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LightDirection,
                    boost::bind(&SkyMapPassDescription::getLightDirectionCB, this, _1),
                    boost::bind(&SkyMapPassDescription::setLightDirectionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(GroundColor,
                    boost::bind(&SkyMapPassDescription::getGroundColorCB, this, _1),
                    boost::bind(&SkyMapPassDescription::setGroundColorCB, this, _1));
}

void
av::gua::SkyMapPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SkyMapPassDescription, true);

        SFSkyMapPassDescription::initClass("av::gua::SFSkyMapPassDescription", "av::Field");
        MFSkyMapPassDescription::initClass("av::gua::MFSkyMapPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void
av::gua::SkyMapPassDescription::getOutputTextureNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSkyMapPassDescription->output_texture_name();
}

void
av::gua::SkyMapPassDescription::setOutputTextureNameCB(const SFString::SetValueEvent& event)
{
  m_guaSkyMapPassDescription->output_texture_name(event.getValue());
}

void
av::gua::SkyMapPassDescription::getLightColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = ::gua::utils::Color3f(m_guaSkyMapPassDescription->light_color());
}

void
av::gua::SkyMapPassDescription::setLightColorCB(const SFColor::SetValueEvent& event)
{
  m_guaSkyMapPassDescription->light_color(event.getValue().vec3f());
}

void
av::gua::SkyMapPassDescription::getLightDirectionCB(const SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSkyMapPassDescription->light_direction();
}

void
av::gua::SkyMapPassDescription::setLightDirectionCB(const SFVec3::SetValueEvent& event)
{
  m_guaSkyMapPassDescription->light_direction(::gua::math::vec3f(event.getValue()));
}

void
av::gua::SkyMapPassDescription::getGroundColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = ::gua::utils::Color3f(m_guaSkyMapPassDescription->ground_color());
}

void
av::gua::SkyMapPassDescription::setGroundColorCB(const SFColor::SetValueEvent& event)
{
  m_guaSkyMapPassDescription->ground_color(event.getValue().vec3f());
}
