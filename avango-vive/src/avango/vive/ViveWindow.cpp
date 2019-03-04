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

av::vive::ViveWindow::ViveWindow(std::shared_ptr<::gua::ViveWindow> const& guaViveWindow) : av::gua::GlfwWindow(guaViveWindow), m_guaViveWindow(guaViveWindow)
{
    // store hmd params in according fields
    // sensor orientations
    AV_FC_ADD_FIELD(HMDSensorOrientation, ::gua::math::mat4());
    AV_FC_ADD_FIELD(Controller0SensorOrientation, ::gua::math::mat4());
    AV_FC_ADD_FIELD(Controller1SensorOrientation, ::gua::math::mat4());
    AV_FC_ADD_FIELD(TrackingReference0SensorOrientation, ::gua::math::mat4());
    AV_FC_ADD_FIELD(TrackingReference1SensorOrientation, ::gua::math::mat4());
    // binary controller states
    AV_FC_ADD_FIELD(Controller0TriggerButtonPressed, false);
    AV_FC_ADD_FIELD(Controller1TriggerButtonPressed, false);
    AV_FC_ADD_FIELD(Controller0GripButtonPressed, false);
    AV_FC_ADD_FIELD(Controller1GripButtonPressed, false);
    AV_FC_ADD_FIELD(Controller0PadButtonPressed, false);
    AV_FC_ADD_FIELD(Controller1PadButtonPressed, false);
    AV_FC_ADD_FIELD(Controller0AppMenuButtonPressed, false);
    AV_FC_ADD_FIELD(Controller1AppMenuButtonPressed, false);
    AV_FC_ADD_FIELD(Controller0PadTouched, false);
    AV_FC_ADD_FIELD(Controller1PadTouched, false);
    // continuous controller states
    AV_FC_ADD_FIELD(Controller0PadXValue, 0.0f);
    AV_FC_ADD_FIELD(Controller1PadXValue, 0.0f);
    AV_FC_ADD_FIELD(Controller0PadYValue, 0.0f);
    AV_FC_ADD_FIELD(Controller1PadYValue, 0.0f);
    AV_FC_ADD_FIELD(Controller0TriggerValue, 0.0f);
    AV_FC_ADD_FIELD(Controller1TriggerValue, 0.0f);
    // general HMD parameters
    AV_FC_ADD_FIELD(Resolution, m_guaViveWindow->get_window_resolution());
    AV_FC_ADD_FIELD(EyeResolution, m_guaViveWindow->get_window_resolution());
    AV_FC_ADD_FIELD(LeftScreenSize, m_guaViveWindow->get_left_screen_size());
    AV_FC_ADD_FIELD(RightScreenSize, m_guaViveWindow->get_right_screen_size());
    AV_FC_ADD_FIELD(LeftScreenTranslation, m_guaViveWindow->get_left_screen_translation());
    AV_FC_ADD_FIELD(RightScreenTranslation, m_guaViveWindow->get_right_screen_translation());
    AV_FC_ADD_FIELD(EyeDistance, m_guaViveWindow->get_IPD());

    // needs to evaluate every frame to update sensor orientation
    m_lastHapticPulse = std::chrono::system_clock::now();
    alwaysEvaluate(true);
}

av::vive::ViveWindow::~ViveWindow() {}

void av::vive::ViveWindow::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::GlfwWindow::initClass();

        AV_FC_INIT(av::gua::GlfwWindow, av::vive::ViveWindow, true);

        SFViveWindow::initClass("av::vive::SFViveWindow", "av::Field");
        MFViveWindow::initClass("av::vive::MFViveWindow", "av::Field");
    }
}

std::shared_ptr<::gua::ViveWindow> const& av::vive::ViveWindow::getGuaViveWindow() const { return m_guaViveWindow; }

void av::vive::ViveWindow::triggerHapticPulse(unsigned int controllerId, float strength) const
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - m_lastHapticPulse;

    if(elapsed_seconds.count() > 0.05)
    {
        m_guaViveWindow->trigger_haptic_pulse(controllerId, strength);
        m_lastHapticPulse = now;
    }
}

void av::vive::ViveWindow::evaluate()
{
    // update the state all components of the system
    m_guaViveWindow->update_sensor_orientations();

    // forward the current sensor orientations
    HMDSensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::HMD));
    Controller0SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::CONTROLLER_0));
    Controller1SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::CONTROLLER_1));
    TrackingReference0SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::TRACKING_REFERENCE_0));
    TrackingReference1SensorOrientation.setValue(m_guaViveWindow->get_sensor_orientation(::gua::ViveWindow::DeviceID::TRACKING_REFERENCE_1));

    // forward the current binary controller states
    Controller0TriggerButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerBinaryStates::TRIGGER_BUTTON));
    Controller1TriggerButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerBinaryStates::TRIGGER_BUTTON));
    Controller0GripButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerBinaryStates::GRIP_BUTTON));
    Controller1GripButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerBinaryStates::GRIP_BUTTON));
    Controller0PadButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerBinaryStates::PAD_BUTTON));
    Controller1PadButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerBinaryStates::PAD_BUTTON));
    Controller0AppMenuButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerBinaryStates::APP_MENU_BUTTON));
    Controller1AppMenuButtonPressed.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerBinaryStates::APP_MENU_BUTTON));
    Controller0PadTouched.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerBinaryStates::PAD_TOUCH));
    Controller1PadTouched.setValue(m_guaViveWindow->get_controller_button_active(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerBinaryStates::PAD_TOUCH));

    // forward the current continuous controller states
    Controller0PadXValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerContinuousStates::PAD_X_VALUE));
    Controller1PadXValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerContinuousStates::PAD_X_VALUE));
    Controller0PadYValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerContinuousStates::PAD_Y_VALUE));
    Controller1PadYValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerContinuousStates::PAD_Y_VALUE));
    Controller0TriggerValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_0, ::gua::ViveWindow::ControllerContinuousStates::TRIGGER_VALUE));
    Controller1TriggerValue.setValue(m_guaViveWindow->get_controller_value(::gua::ViveWindow::DeviceID::CONTROLLER_1, ::gua::ViveWindow::ControllerContinuousStates::TRIGGER_VALUE));
}
