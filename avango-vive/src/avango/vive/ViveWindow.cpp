#include <avango/vive/ViveWindow.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::vive::ViveWindow"));
}

AV_FC_DEFINE(av::vive::ViveWindow);

AV_FIELD_DEFINE(av::vive::SFViveWindow);
AV_FIELD_DEFINE(av::vive::MFViveWindow);

av::vive::ViveWindow::ViveWindow(
  std::shared_ptr< ::gua::ViveWindow> const& guaViveWindow
) : av::gua::GlfwWindow(guaViveWindow),
    m_guaViveWindow(guaViveWindow)
{
  // store hmd params in according fields
  AV_FC_ADD_FIELD(HMDSensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(Controller0SensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(Controller1SensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(TrackingReference0SensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(TrackingReference1SensorOrientation, ::gua::math::mat4());
  AV_FC_ADD_FIELD(Resolution,m_guaViveWindow->get_window_resolution());
  AV_FC_ADD_FIELD(EyeResolution,m_guaViveWindow->get_window_resolution());
  AV_FC_ADD_FIELD(LeftScreenSize, m_guaViveWindow->get_left_screen_size());
  AV_FC_ADD_FIELD(RightScreenSize, m_guaViveWindow->get_right_screen_size());
  AV_FC_ADD_FIELD(LeftScreenTranslation, m_guaViveWindow->get_left_screen_translation());
  AV_FC_ADD_FIELD(RightScreenTranslation, m_guaViveWindow->get_right_screen_translation());
  AV_FC_ADD_FIELD(EyeDistance, m_guaViveWindow->get_IPD());

  // needs to evaluate every frame to update sensor orientation
  alwaysEvaluate(true);
}

av::vive::ViveWindow::~ViveWindow()
{}

void
av::vive::ViveWindow::initClass()
{

  if (!isTypeInitialized())
  {
    av::gua::GlfwWindow::initClass();

    AV_FC_INIT(av::gua::GlfwWindow, av::vive::ViveWindow, true);

    SFViveWindow::initClass("av::vive::SFViveWindow", "av::Field");
    MFViveWindow::initClass("av::vive::MFViveWindow", "av::Field");
  }
}

std::shared_ptr< ::gua::ViveWindow> const&
av::vive::ViveWindow::getGuaViveWindow() const {
  return m_guaViveWindow;
}

void
av::vive::ViveWindow::evaluate()
{
  m_guaViveWindow->update_sensor_orientations();

  HMDSensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::HMD));
  Controller0SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::CONTROLLER_0));
  Controller1SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::CONTROLLER_1));
  TrackingReference0SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::TRACKING_REFERENCE_0));
  TrackingReference0SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::TRACKING_REFERENCE_1));
}
