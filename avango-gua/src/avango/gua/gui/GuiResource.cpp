#include <avango/gua/gui/GuiResource.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::GuiResource"));
}

AV_FC_DEFINE(av::gua::GuiResource);

AV_FIELD_DEFINE(av::gua::SFGuiResource);
AV_FIELD_DEFINE(av::gua::MFGuiResource);

av::gua::GuiResource::GuiResource(std::shared_ptr< ::gua::GuiResource> guaGuiResource)
  : av::FieldContainer(),
    m_guaGuiResource(guaGuiResource)
{}

av::gua::GuiResource::~GuiResource()
{}

void
av::gua::GuiResource::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::GuiResource, true);

        SFGuiResource::initClass("av::gua::SFGuiResource", "av::Field");
        MFGuiResource::initClass("av::gua::MFGuiResource", "av::Field");
    }
}

std::shared_ptr< ::gua::GuiResource>
av::gua::GuiResource::getGuaGuiResource() const
{
    return m_guaGuiResource;
}


void
av::gua::GuiResource::init(std::string const& name, std::string const& url,
                           ::gua::math::vec2 const& size) {
  m_guaGuiResource->init(name, url, size);
}
