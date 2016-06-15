#include <avango/gua/renderer/Window.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Window"));
}

AV_FC_DEFINE(av::gua::Window);

AV_FIELD_DEFINE(av::gua::SFWindow);
AV_FIELD_DEFINE(av::gua::MFWindow);

av::gua::Window::Window(std::shared_ptr< ::gua::Window> const& guaWindow)
    : av::gua::WindowBase(guaWindow)
    , m_guaWindow(guaWindow)
{}

void
av::gua::Window::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::WindowBase::initClass();

        AV_FC_INIT(av::gua::WindowBase, av::gua::Window, true);

        SFWindow::initClass("av::gua::SFWindow", "av::Field");
        MFWindow::initClass("av::gua::MFWindow", "av::Field");
    }
}

std::shared_ptr< ::gua::Window> const&
av::gua::Window::getGuaWindow() const
{
    return m_guaWindow;
}
