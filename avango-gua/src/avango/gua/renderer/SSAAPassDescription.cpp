#include <avango/gua/renderer/SSAAPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SSAAPassDescription"));
}

AV_FC_DEFINE(av::gua::SSAAPassDescription);

AV_FIELD_DEFINE(av::gua::SFSSAAPassDescription);
AV_FIELD_DEFINE(av::gua::MFSSAAPassDescription);

av::gua::SSAAPassDescription::SSAAPassDescription(
  std::shared_ptr< ::gua::SSAAPassDescription> const& guaSSAAPassDescription)
    : PipelinePassDescription(guaSSAAPassDescription)
    , m_guaSSAAPassDescription(guaSSAAPassDescription)
{
  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                    std::bind(&SSAAPassDescription::getModeCB, this,std::placeholders::_1),
                    std::bind(&SSAAPassDescription::setModeCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(FxaaQualitySubpix,
                    std::bind(&SSAAPassDescription::getFxaaQualitySubpixCB, this,std::placeholders::_1),
                    std::bind(&SSAAPassDescription::setFxaaQualitySubpixCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(FxaaEdgeThreshold,
                    std::bind(&SSAAPassDescription::getFxaaEdgeThresholdCB, this,std::placeholders::_1),
                    std::bind(&SSAAPassDescription::setFxaaEdgeThresholdCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(FxaaThresholdMin,
                    std::bind(&SSAAPassDescription::getFxaaThresholdMinCB, this,std::placeholders::_1),
                    std::bind(&SSAAPassDescription::setFxaaThresholdMinCB, this,std::placeholders::_1));
  AV_FC_ADD_ADAPTOR_FIELD(PinholeErrorCorrection,
                    std::bind(&SSAAPassDescription::getPinholeErrorCorrectionCB, this,std::placeholders::_1),
                    std::bind(&SSAAPassDescription::setPinholeErrorCorrectionCB, this,std::placeholders::_1));
}

void
av::gua::SSAAPassDescription::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::PipelinePassDescription::initClass();

    AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SSAAPassDescription, true);

    SFSSAAPassDescription::initClass("av::gua::SFSSAAPassDescription", "av::Field");
    MFSSAAPassDescription::initClass("av::gua::MFSSAAPassDescription", "av::Field");
    sClassTypeId.setDistributable(true);
  }
}


std::shared_ptr< ::gua::SSAAPassDescription> const&
av::gua::SSAAPassDescription::getGuaSSAAPassDescription() const
{
  return m_guaSSAAPassDescription;
}

void
av::gua::SSAAPassDescription::getModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaSSAAPassDescription->mode());
}

void
av::gua::SSAAPassDescription::setModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaSSAAPassDescription->mode(static_cast< ::gua::SSAAPassDescription::SSAAMode>(event.getValue()));
}

void
av::gua::SSAAPassDescription::getFxaaQualitySubpixCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSSAAPassDescription->fxaa_quality_subpix();
}

void
av::gua::SSAAPassDescription::setFxaaQualitySubpixCB(const SFFloat::SetValueEvent& event)
{
  m_guaSSAAPassDescription->fxaa_quality_subpix(event.getValue());
}

void
av::gua::SSAAPassDescription::getFxaaEdgeThresholdCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSSAAPassDescription->fxaa_edge_threshold();
}

void
av::gua::SSAAPassDescription::setFxaaEdgeThresholdCB(const SFFloat::SetValueEvent& event)
{
  m_guaSSAAPassDescription->fxaa_edge_threshold(event.getValue());
}

void
av::gua::SSAAPassDescription::getFxaaThresholdMinCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSSAAPassDescription->fxaa_threshold_min();
}

void
av::gua::SSAAPassDescription::setFxaaThresholdMinCB(const SFFloat::SetValueEvent& event)
{
  m_guaSSAAPassDescription->fxaa_threshold_min(event.getValue());
}

void
av::gua::SSAAPassDescription::getPinholeErrorCorrectionCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSSAAPassDescription->enable_pinhole_correction();
}

void
av::gua::SSAAPassDescription::setPinholeErrorCorrectionCB(const SFBool::SetValueEvent& event)
{
  m_guaSSAAPassDescription->enable_pinhole_correction(event.getValue());
}