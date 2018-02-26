#include <avango/gua/nurbs/renderer/NURBSPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::nurbs::NURBSPassDescription"));
}

AV_FC_DEFINE(av::gua::nurbs::NURBSPassDescription);

AV_FIELD_DEFINE(av::gua::nurbs::SFNURBSPassDescription);
AV_FIELD_DEFINE(av::gua::nurbs::MFNURBSPassDescription);

av::gua::nurbs::NURBSPassDescription::NURBSPassDescription(
  std::shared_ptr< ::gua::NURBSPassDescription> const& guaNURBSPassDescription)
    : PipelinePassDescription(guaNURBSPassDescription)
    , m_guaNURBSPassDescription(guaNURBSPassDescription)
{

}

void
av::gua::nurbs::NURBSPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::nurbs::NURBSPassDescription, true);

        SFNURBSPassDescription::initClass("av::gua::nurbs::SFNURBSPassDescription", "av::Field");
        MFNURBSPassDescription::initClass("av::gua::nurbs::MFNURBSPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::NURBSPassDescription> const&
av::gua::nurbs::NURBSPassDescription::getGuaNURBSPassDescription() const
{
    return m_guaNURBSPassDescription;
}

void
av::gua::nurbs::NURBSPassDescription::getPretessellationCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNURBSPassDescription->enable_pretessellation();
}

void
av::gua::nurbs::NURBSPassDescription::setPretessellationCB(const SFBool::SetValueEvent& event)
{
  m_guaNURBSPassDescription->enable_pretessellation(event.getValue());
}

