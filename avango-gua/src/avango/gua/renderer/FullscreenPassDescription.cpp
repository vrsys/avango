#include <avango/gua/renderer/FullscreenPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::FullscreenPassDescription"));
}

AV_FC_DEFINE(av::gua::FullscreenPassDescription);

AV_FIELD_DEFINE(av::gua::SFFullscreenPassDescription);
AV_FIELD_DEFINE(av::gua::MFFullscreenPassDescription);

av::gua::FullscreenPassDescription::FullscreenPassDescription(
  std::shared_ptr< ::gua::FullscreenPassDescription> const& guaFullscreenPassDescription)
    : PipelinePassDescription(guaFullscreenPassDescription)
    , m_guaFullscreenPassDescription(guaFullscreenPassDescription)
{

  AV_FC_ADD_ADAPTOR_FIELD(Source,
                    std::bind(&FullscreenPassDescription::getSourceCB, this,std::placeholders::_1),
                    std::bind(&FullscreenPassDescription::setSourceCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SourceFile,
                    std::bind(&FullscreenPassDescription::getSourceFileCB, this,std::placeholders::_1),
                    std::bind(&FullscreenPassDescription::setSourceFileCB, this,std::placeholders::_1));
}

void
av::gua::FullscreenPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::FullscreenPassDescription, true);

        SFFullscreenPassDescription::initClass("av::gua::SFFullscreenPassDescription", "av::Field");
        MFFullscreenPassDescription::initClass("av::gua::MFFullscreenPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void
av::gua::FullscreenPassDescription::getSourceCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFullscreenPassDescription->source();
}

void
av::gua::FullscreenPassDescription::setSourceCB(const SFString::SetValueEvent& event)
{
  m_guaFullscreenPassDescription->source(event.getValue());
}

void
av::gua::FullscreenPassDescription::getSourceFileCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFullscreenPassDescription->source_file();
}

void
av::gua::FullscreenPassDescription::setSourceFileCB(const SFString::SetValueEvent& event)
{
  m_guaFullscreenPassDescription->source_file(event.getValue());
}

std::shared_ptr< ::gua::FullscreenPassDescription> const&
av::gua::FullscreenPassDescription::getGuaFullscreenPassDescription() const
{
    return m_guaFullscreenPassDescription;
}


