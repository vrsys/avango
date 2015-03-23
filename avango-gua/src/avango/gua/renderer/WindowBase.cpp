#include <avango/gua/renderer/WindowBase.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::WindowBase"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::WindowBase);

AV_FIELD_DEFINE(av::gua::SFWindowBase);
AV_FIELD_DEFINE(av::gua::MFWindowBase);

av::gua::WindowBase::WindowBase(std::shared_ptr< ::gua::WindowBase> const& guaWindowBase)
    : m_guaWindowBase(guaWindowBase)
{

  AV_FC_ADD_ADAPTOR_FIELD(Size,
                      boost::bind(&WindowBase::getSizeCB, this, _1),
                      boost::bind(&WindowBase::setSizeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Title,
                      boost::bind(&WindowBase::getTitleCB, this, _1),
                      boost::bind(&WindowBase::setTitleCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Display,
                      boost::bind(&WindowBase::getDisplayCB, this, _1),
                      boost::bind(&WindowBase::setDisplayCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(StereoMode,
                      boost::bind(&WindowBase::getStereoModeCB, this, _1),
                      boost::bind(&WindowBase::setStereoModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftResolution,
                      boost::bind(&WindowBase::getLeftResolutionCB, this, _1),
                      boost::bind(&WindowBase::setLeftResolutionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftPosition,
                      boost::bind(&WindowBase::getLeftPositionCB, this, _1),
                      boost::bind(&WindowBase::setLeftPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightResolution,
                      boost::bind(&WindowBase::getRightResolutionCB, this, _1),
                      boost::bind(&WindowBase::setRightResolutionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightPosition,
                      boost::bind(&WindowBase::getRightPositionCB, this, _1),
                      boost::bind(&WindowBase::setRightPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableVsync,
                      boost::bind(&WindowBase::getEnableVsyncCB, this, _1),
                      boost::bind(&WindowBase::setEnableVsyncCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixRedRight,
                      boost::bind(&WindowBase::getWarpMatrixRedRight, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixRedRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixGreenRight,
                      boost::bind(&WindowBase::getWarpMatrixGreenRight, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixGreenRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixBlueRight,
                      boost::bind(&WindowBase::getWarpMatrixBlueRight, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixBlueRight, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixRedLeft,
                      boost::bind(&WindowBase::getWarpMatrixRedLeft, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixRedLeft, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixGreenLeft,
                      boost::bind(&WindowBase::getWarpMatrixGreenLeft, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixGreenLeft, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(WarpMatrixBlueLeft,
                      boost::bind(&WindowBase::getWarpMatrixBlueLeft, this, _1),
                      boost::bind(&WindowBase::setWarpMatrixBlueLeft, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderingFPS,
                      boost::bind(&WindowBase::getRenderingFPSCB, this, _1),
                      boost::bind(&WindowBase::setRenderingFPSCB, this, _1));
}

void
av::gua::WindowBase::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::WindowBase, true);

        SFWindowBase::initClass("av::gua::SFWindowBase", "av::Field");
        MFWindowBase::initClass("av::gua::MFWindowBase", "av::Field");
    }
}

void
av::gua::WindowBase::open() {
  m_guaWindowBase->open();
}

bool
av::gua::WindowBase::is_open() const{
  return m_guaWindowBase->get_is_open();
}

bool
av::gua::WindowBase::should_close() const{
  return m_guaWindowBase->should_close();
}

void
av::gua::WindowBase::close() {
  m_guaWindowBase->close();
}

void
av::gua::WindowBase::process_events() {
  m_guaWindowBase->process_events();
}

std::shared_ptr< ::gua::WindowBase> const&
av::gua::WindowBase::getGuaWindowBase() const
{
    return m_guaWindowBase;
}

void
av::gua::WindowBase::getSizeCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_size();
}

void
av::gua::WindowBase::setSizeCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindowBase->config.set_size(event.getValue());
}

void
av::gua::WindowBase::getTitleCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_title();
}

void
av::gua::WindowBase::setTitleCB(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_title(event.getValue());
}

void
av::gua::WindowBase::getDisplayCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_display_name();
}

void
av::gua::WindowBase::setDisplayCB(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_display_name(event.getValue());
}

void
av::gua::WindowBase::getStereoModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaWindowBase->config.get_stereo_mode());
}

void
av::gua::WindowBase::setStereoModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaWindowBase->config.set_stereo_mode(static_cast< ::gua::StereoMode>(event.getValue()));
}

void
av::gua::WindowBase::getLeftResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_left_resolution();
}

void
av::gua::WindowBase::setLeftResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindowBase->config.set_left_resolution(event.getValue());
}

void
av::gua::WindowBase::getLeftPositionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_left_position();
}

void
av::gua::WindowBase::setLeftPositionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindowBase->config.set_left_position(event.getValue());
}

void
av::gua::WindowBase::getRightResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_right_resolution();
}

void
av::gua::WindowBase::setRightResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindowBase->config.set_right_resolution(event.getValue());
}

void
av::gua::WindowBase::getRightPositionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_right_position();
}

void
av::gua::WindowBase::setRightPositionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaWindowBase->config.set_right_position(event.getValue());
}

void
av::gua::WindowBase::getEnableVsyncCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_enable_vsync();
}

void
av::gua::WindowBase::setEnableVsyncCB(const SFBool::SetValueEvent& event)
{
  m_guaWindowBase->config.set_enable_vsync(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixRedRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_red_right();
}

void
av::gua::WindowBase::setWarpMatrixRedRight(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_red_right(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixGreenRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_green_right();
}

void
av::gua::WindowBase::setWarpMatrixGreenRight(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_green_right(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixBlueRight(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_blue_right();
}

void
av::gua::WindowBase::setWarpMatrixBlueRight(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_blue_right(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixRedLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_red_left();
}

void
av::gua::WindowBase::setWarpMatrixRedLeft(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_red_left(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixGreenLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_green_left();
}

void
av::gua::WindowBase::setWarpMatrixGreenLeft(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_green_left(event.getValue());
}

void
av::gua::WindowBase::getWarpMatrixBlueLeft(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->config.get_warp_matrix_blue_left();
}

void
av::gua::WindowBase::setWarpMatrixBlueLeft(const SFString::SetValueEvent& event)
{
  m_guaWindowBase->config.set_warp_matrix_blue_left(event.getValue());
}

void
av::gua::WindowBase::getRenderingFPSCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaWindowBase->get_rendering_fps();
}

void
av::gua::WindowBase::setRenderingFPSCB(const SFFloat::SetValueEvent& event)
{}
