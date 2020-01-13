#include <avango/gua/renderer/PipelinePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::PipelinePassDescription"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::PipelinePassDescription);

AV_FIELD_DEFINE(av::gua::SFPipelinePassDescription);
AV_FIELD_DEFINE(av::gua::MFPipelinePassDescription);

av::gua::PipelinePassDescription::PipelinePassDescription(std::shared_ptr<::gua::PipelinePassDescription> const& guaPipelinePassDescription) : m_guaPipelinePassDescription(guaPipelinePassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(
        EnableDepthSorting, std::bind(&PipelinePassDescription::getEnableDepthSortingCB, this, std::placeholders::_1), std::bind(&PipelinePassDescription::setEnableDepthSortingCB, this, std::placeholders::_1));
    
}

void av::gua::PipelinePassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::PipelinePassDescription, true);

        SFPipelinePassDescription::initClass("av::gua::SFPipelinePassDescription", "av::Field");
        MFPipelinePassDescription::initClass("av::gua::MFPipelinePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void av::gua::PipelinePassDescription::getEnableDepthSortingCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPipelinePassDescription->get_enable_depth_sorting(); }

void av::gua::PipelinePassDescription::setEnableDepthSortingCB(const SFBool::SetValueEvent& event) { m_guaPipelinePassDescription->set_enable_depth_sorting(event.getValue()); }


std::shared_ptr<::gua::PipelinePassDescription> const& av::gua::PipelinePassDescription::getGuaPipelinePassDescription() const { return m_guaPipelinePassDescription; }
