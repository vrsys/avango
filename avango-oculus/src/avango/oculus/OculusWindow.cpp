#include <avango/oculus/OculusWindow.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::oculus::OculusWindow"));
}

AV_FC_DEFINE(av::oculus::OculusWindow);

AV_FIELD_DEFINE(av::oculus::SFOculusWindow);
AV_FIELD_DEFINE(av::oculus::MFOculusWindow);

av::oculus::OculusWindow::OculusWindow(
  std::shared_ptr< ::gua::OculusWindow> const& guaOculusWindow
) : av::gua::GlfwWindow(guaOculusWindow),
    m_guaOculusWindow(guaOculusWindow)
{
  // store hmd params in according fields
  AV_FC_ADD_FIELD(SensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(Resolution,m_guaOculusWindow->get_window_resolution());
  AV_FC_ADD_FIELD(EyeResolution,m_guaOculusWindow->get_window_resolution());
  AV_FC_ADD_FIELD(LeftScreenSize, m_guaOculusWindow->get_left_screen_size());
  AV_FC_ADD_FIELD(RightScreenSize, m_guaOculusWindow->get_right_screen_size());
  AV_FC_ADD_FIELD(LeftScreenTranslation, m_guaOculusWindow->get_left_screen_translation());
  AV_FC_ADD_FIELD(RightScreenTranslation, m_guaOculusWindow->get_right_screen_translation());
  AV_FC_ADD_FIELD(EyeDistance, m_guaOculusWindow->get_IPD());

  // needs to evaluate every frame to update sensor orientation
  alwaysEvaluate(true);
}

av::oculus::OculusWindow::~OculusWindow()
{}

void
av::oculus::OculusWindow::initClass()
{

  if (!isTypeInitialized())
  {
    av::gua::GlfwWindow::initClass();

    AV_FC_INIT(av::gua::GlfwWindow, av::oculus::OculusWindow, true);

    SFOculusWindow::initClass("av::oculus::SFOculusWindow", "av::Field");
    MFOculusWindow::initClass("av::oculus::MFOculusWindow", "av::Field");
  }
}

std::shared_ptr< ::gua::OculusWindow> const&
av::oculus::OculusWindow::getGuaOculusWindow() const {
  return m_guaOculusWindow;
}

void
av::oculus::OculusWindow::evaluate()
{
  SensorOrientation.setValue(m_guaOculusWindow->get_hmd_sensor_orientation());
}
