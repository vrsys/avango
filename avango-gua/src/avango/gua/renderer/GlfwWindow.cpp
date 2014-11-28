#include <avango/gua/renderer/GlfwWindow.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::GlfwWindow"));
}

AV_FC_DEFINE(av::gua::GlfwWindow);

AV_FIELD_DEFINE(av::gua::SFGlfwWindow);
AV_FIELD_DEFINE(av::gua::MFGlfwWindow);

av::gua::GlfwWindow::GlfwWindow(std::shared_ptr< ::gua::GlfwWindow> const& guaGlfwWindow)
    : av::gua::WindowBase(guaGlfwWindow)
    , m_guaGlfwWindow(guaGlfwWindow)
{}

void
av::gua::GlfwWindow::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::WindowBase::initClass();

        AV_FC_INIT(av::gua::WindowBase, av::gua::GlfwWindow, true);

        SFGlfwWindow::initClass("av::gua::SFGlfwWindow", "av::Field");
        MFGlfwWindow::initClass("av::gua::MFGlfwWindow", "av::Field");
    }
}

std::shared_ptr< ::gua::GlfwWindow> const&
av::gua::GlfwWindow::getGuaGlfwWindow() const
{
    return m_guaGlfwWindow;
}
