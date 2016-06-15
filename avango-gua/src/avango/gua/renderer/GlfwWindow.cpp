#include <avango/gua/renderer/GlfwWindow.hpp>
#include <avango/Base.h>
#include <functional>
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
{
  AV_FC_ADD_ADAPTOR_FIELD(CursorMode,
                    std::bind(&GlfwWindow::getCursorModeCB, this,std::placeholders::_1),
                    std::bind(&GlfwWindow::setCursorModeCB, this,std::placeholders::_1)); 
}

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

void av::gua::GlfwWindow::on_resize(std::function<void( ::gua::math::vec2ui const&)> const& callback) const {
    m_guaGlfwWindow->on_resize.connect(callback);
}

void av::gua::GlfwWindow::on_key_press(std::function<void(int, int, int, int)> const& callback) const {
    m_guaGlfwWindow->on_key_press.connect(callback);
}

void av::gua::GlfwWindow::on_char(std::function<void(unsigned)> const& callback) const {
    m_guaGlfwWindow->on_char.connect(callback);
}

void av::gua::GlfwWindow::on_button_press(std::function<void(int, int, int)> const& callback) const {
    m_guaGlfwWindow->on_button_press.connect(callback);
}

void av::gua::GlfwWindow::on_move_cursor(std::function<void( ::gua::math::vec2 const&)> const& callback) const {
    m_guaGlfwWindow->on_move_cursor.connect(callback);
}

void av::gua::GlfwWindow::on_scroll(std::function<void( ::gua::math::vec2 const&)> const& callback) const {
    m_guaGlfwWindow->on_scroll.connect(callback);
}

void av::gua::GlfwWindow::on_enter(std::function<void(bool)> const& callback) const {
    m_guaGlfwWindow->on_enter.connect(callback);
}

void av::gua::GlfwWindow::getCursorModeCB(const SFUInt::GetValueEvent& event) {
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaGlfwWindow->cursor_mode());
}

void av::gua::GlfwWindow::setCursorModeCB(const SFUInt::SetValueEvent& event) {
  m_guaGlfwWindow->cursor_mode(static_cast< ::gua::GlfwWindow::CursorMode>(event.getValue()));
}
