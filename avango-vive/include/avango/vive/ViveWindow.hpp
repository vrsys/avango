#ifndef AVANGO_VIVE_VIVEWINDOW_HPP
#define AVANGO_VIVE_VIVEWINDOW_HPP

#include <gua/ViveWindow.hpp>

#include <avango/gua/renderer/GlfwWindow.hpp>

#include <gua/math/math.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>
#include <avango/gua/scenegraph/Node.hpp>

#include <avango/vive/windows_specific.hpp>
#include <chrono>

namespace av
{
namespace vive
{
/**
 * Wrapper for ::gua::ViveWindow
 */

class AV_VIVE_DLL ViveWindow : public av::gua::GlfwWindow
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::ViveRift is created.
     * Otherwise, the given ::gua::ViveRift is used.
     */

    ViveWindow(std::shared_ptr<::gua::ViveWindow> const& guaViveWindow = std::shared_ptr<::gua::ViveWindow>(new ::gua::ViveWindow(":0.0")));
    virtual ~ViveWindow();

  public:
    // sensor orientations
    ::av::gua::SFMatrix HMDSensorOrientation;
    ::av::gua::SFMatrix Controller0SensorOrientation;
    ::av::gua::SFMatrix Controller1SensorOrientation;
    ::av::gua::SFMatrix TrackingReference0SensorOrientation;
    ::av::gua::SFMatrix TrackingReference1SensorOrientation;
    // controller binary states
    ::av::SFBool Controller0TriggerButtonPressed;
    ::av::SFBool Controller1TriggerButtonPressed;
    ::av::SFBool Controller0GripButtonPressed;
    ::av::SFBool Controller1GripButtonPressed;
    ::av::SFBool Controller0PadButtonPressed;
    ::av::SFBool Controller1PadButtonPressed;
    ::av::SFBool Controller0AppMenuButtonPressed;
    ::av::SFBool Controller1AppMenuButtonPressed;
    ::av::SFBool Controller0PadTouched;
    ::av::SFBool Controller1PadTouched;
    // controller continuous states
    ::av::SFFloat Controller0PadXValue;
    ::av::SFFloat Controller1PadXValue;
    ::av::SFFloat Controller0PadYValue;
    ::av::SFFloat Controller1PadYValue;
    ::av::SFFloat Controller0TriggerValue;
    ::av::SFFloat Controller1TriggerValue;
    // general HMD properties
    ::av::gua::SFVec2ui Resolution;
    ::av::gua::SFVec2ui EyeResolution;
    ::av::gua::SFVec2 LeftScreenSize;
    ::av::gua::SFVec2 RightScreenSize;
    ::av::gua::SFVec3 LeftScreenTranslation;
    ::av::gua::SFVec3 RightScreenTranslation;
    ::av::SFFloat EyeDistance;

  public:
    /**
     * Get the wrapped ::gua::ViveRift
     */
    std::shared_ptr<::gua::ViveWindow> const& getGuaViveWindow() const;
    void triggerHapticPulse(unsigned int controllerId, float strength) const;
	void register_node(av::Link<av::gua::Node> n, int t);

  private:
    std::shared_ptr<::gua::ViveWindow> m_guaViveWindow;
    mutable std::chrono::system_clock::time_point m_lastHapticPulse;

    void evaluate() override;

    ViveWindow(const ViveWindow&);
    ViveWindow& operator=(const ViveWindow&);
};

using SFViveWindow = SingleField<Link<ViveWindow>>;
using MFViveWindow = MultiField<Link<ViveWindow>>;

} // namespace vive

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_VIVE_DLL SingleField<Link<vive::ViveWindow>>;
template class AV_VIVE_DLL MultiField<Link<vive::ViveWindow>>;
#endif
} // namespace av

#endif // AVANGO_VIVE_VIVEWINDOW_HPP
