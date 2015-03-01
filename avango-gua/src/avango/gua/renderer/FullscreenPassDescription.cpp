#include <avango/gua/renderer/FullscreenPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
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
                    boost::bind(&FullscreenPassDescription::getSourceCB, this, _1),
                    boost::bind(&FullscreenPassDescription::setSourceCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SourceFile,
                    boost::bind(&FullscreenPassDescription::getSourceFileCB, this, _1),
                    boost::bind(&FullscreenPassDescription::setSourceFileCB, this, _1));
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


