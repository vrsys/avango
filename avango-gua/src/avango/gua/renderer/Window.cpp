#include <avango/gua/renderer/Window.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Window"));
}

AV_FC_DEFINE(av::gua::Window);

AV_FIELD_DEFINE(av::gua::SFWindow);
AV_FIELD_DEFINE(av::gua::MFWindow);

av::gua::Window::Window(::gua::Window* guaWindow)
    : m_guaWindow(guaWindow)
{

  AV_FC_ADD_ADAPTOR_FIELD(Size,
                      boost::bind(&Window::getSizeCB, this, _1),
                      boost::bind(&Window::setSizeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Title,
                      boost::bind(&Window::getTitleCB, this, _1),
                      boost::bind(&Window::setTitleCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Display,
                      boost::bind(&Window::getDisplayCB, this, _1),
                      boost::bind(&Window::setDisplayCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(StereoMode,
                      boost::bind(&Window::getStereoModeCB, this, _1),
                      boost::bind(&Window::setStereoModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftResolution,
                      boost::bind(&Window::getLeftResolutionCB, this, _1),
                      boost::bind(&Window::setLeftResolutionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftPosition,
                      boost::bind(&Window::getLeftPositionCB, this, _1),
                      boost::bind(&Window::setLeftPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightResolution,
                      boost::bind(&Window::getRightResolutionCB, this, _1),
                      boost::bind(&Window::setRightResolutionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightPosition,
                      boost::bind(&Window::getRightPositionCB, this, _1),
                      boost::bind(&Window::setRightPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableVsync,
                      boost::bind(&Window::getEnableVsyncCB, this, _1),
                      boost::bind(&Window::setEnableVsyncCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixRedRight,
                      boost::bind(&Window::getWarpMatrixRedRight, this, _1),
                      boost::bind(&Window::setWarpMatrixRedRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixGreenRight,
                      boost::bind(&Window::getWarpMatrixGreenRight, this, _1),
                      boost::bind(&Window::setWarpMatrixGreenRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixBlueRight,
                      boost::bind(&Window::getWarpMatrixBlueRight, this, _1),
                      boost::bind(&Window::setWarpMatrixBlueRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixRedLeft,
                      boost::bind(&Window::getWarpMatrixRedLeft, this, _1),
                      boost::bind(&Window::setWarpMatrixRedLeft, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixGreenLeft,
                      boost::bind(&Window::getWarpMatrixGreenLeft, this, _1),
                      boost::bind(&Window::setWarpMatrixGreenLeft, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixBlueLeft,
                      boost::bind(&Window::getWarpMatrixBlueLeft, this, _1),
                      boost::bind(&Window::setWarpMatrixBlueLeft, this, _1));

}

av::gua::Window::~Window()
{}

void
av::gua::Window::open() {
  m_guaWindow->open();
}

void
av::gua::Window::close() {
  m_guaWindow->close();
}

void
av::gua::Window::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Window, true);

        SFWindow::initClass("av::gua::SFWindow", "av::Field");
        MFWindow::initClass("av::gua::MFWindow", "av::Field");
    }
}

::gua::Window*
av::gua::Window::getGuaWindow() const
{
    return m_guaWindow;
}

void
av::gua::Window::getSizeCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_size();
}

void
av::gua::Window::setSizeCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindow->config.set_size(event.getValue());
}

void
av::gua::Window::getTitleCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_title();
}

void
av::gua::Window::setTitleCB(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_title(event.getValue());
}

void
av::gua::Window::getDisplayCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_display_name();
}

void
av::gua::Window::setDisplayCB(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_display_name(event.getValue());
}

void
av::gua::Window::getStereoModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaWindow->config.get_stereo_mode());
}

void
av::gua::Window::setStereoModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaWindow->config.set_stereo_mode(static_cast< ::gua::StereoMode>(event.getValue()));
}

void
av::gua::Window::getLeftResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_left_resolution();
}

void
av::gua::Window::setLeftResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindow->config.set_left_resolution(event.getValue());
}

void
av::gua::Window::getLeftPositionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_left_position();
}

void
av::gua::Window::setLeftPositionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindow->config.set_left_position(event.getValue());
}

void
av::gua::Window::getRightResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_right_resolution();
}

void
av::gua::Window::setRightResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindow->config.set_right_resolution(event.getValue());
}

void
av::gua::Window::getRightPositionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_right_position();
}

void
av::gua::Window::setRightPositionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindow->config.set_right_position(event.getValue());
}

void
av::gua::Window::getEnableVsyncCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_enable_vsync();
}

void
av::gua::Window::setEnableVsyncCB(const SFBool::SetValueEvent& event)
{
  m_guaWindow->config.set_enable_vsync(event.getValue());
}

void
av::gua::Window::getWarpMatrixRedRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_red_right();
}

void
av::gua::Window::setWarpMatrixRedRight(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_red_right(event.getValue());
}

void
av::gua::Window::getWarpMatrixGreenRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_green_right();
}

void
av::gua::Window::setWarpMatrixGreenRight(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_green_right(event.getValue());
}

void
av::gua::Window::getWarpMatrixBlueRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_blue_right();
}

void
av::gua::Window::setWarpMatrixBlueRight(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_blue_right(event.getValue());
}

void
av::gua::Window::getWarpMatrixRedLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_red_left();
}

void
av::gua::Window::setWarpMatrixRedLeft(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_red_left(event.getValue());
}

void
av::gua::Window::getWarpMatrixGreenLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_green_left();
}

void
av::gua::Window::setWarpMatrixGreenLeft(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_green_left(event.getValue());
}

void
av::gua::Window::getWarpMatrixBlueLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindow->config.get_warp_matrix_blue_left();
}

void
av::gua::Window::setWarpMatrixBlueLeft(const SFString::SetValueEvent& event)
{
  m_guaWindow->config.set_warp_matrix_blue_left(event.getValue());
}
