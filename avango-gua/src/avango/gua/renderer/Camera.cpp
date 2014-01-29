#include <avango/gua/renderer/Camera.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Camera"));
}

AV_FC_DEFINE(av::gua::Camera);

AV_FIELD_DEFINE(av::gua::SFCamera);
AV_FIELD_DEFINE(av::gua::MFCamera);

av::gua::Camera::Camera(::gua::Camera* guaCamera)
    : m_guaCamera(guaCamera)
{

  AV_FC_ADD_ADAPTOR_FIELD(LeftEye,
                      boost::bind(&Camera::getLeftEyeCB, this, _1),
                      boost::bind(&Camera::setLeftEyeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightEye,
                      boost::bind(&Camera::getRightEyeCB, this, _1),
                      boost::bind(&Camera::setRightEyeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(LeftScreen,
                      boost::bind(&Camera::getLeftScreenCB, this, _1),
                      boost::bind(&Camera::setLeftScreenCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RightScreen,
                      boost::bind(&Camera::getRightScreenCB, this, _1),
                      boost::bind(&Camera::setRightScreenCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SceneGraph,
                      boost::bind(&Camera::getSceneGraphCB, this, _1),
                      boost::bind(&Camera::setSceneGraphCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderMask,
                      boost::bind(&Camera::getRenderMaskCB, this, _1),
                      boost::bind(&Camera::setRenderMaskCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Mode,
                      boost::bind(&Camera::getModeCB, this, _1),
                      boost::bind(&Camera::setModeCB, this, _1));
}

av::gua::Camera::~Camera()
{}

void
av::gua::Camera::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Camera, true);

        SFCamera::initClass("av::gua::SFCamera", "av::Field");
        MFCamera::initClass("av::gua::MFCamera", "av::Field");
    }
}

::gua::Camera*
av::gua::Camera::getGuaCamera() const
{
    return m_guaCamera;
}

void
av::gua::Camera::getLeftEyeCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->eye_l;
}

void
av::gua::Camera::setLeftEyeCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->eye_l = event.getValue();
}

void
av::gua::Camera::getRightEyeCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->eye_r;
}

void
av::gua::Camera::setRightEyeCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->eye_r = event.getValue();
}

void
av::gua::Camera::getLeftScreenCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->screen_l;
}

void
av::gua::Camera::setLeftScreenCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->screen_l = event.getValue();
}

void
av::gua::Camera::getRightScreenCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->screen_r;
}

void
av::gua::Camera::setRightScreenCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->screen_r = event.getValue();
}

void
av::gua::Camera::getSceneGraphCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->scene_graph;
}

void
av::gua::Camera::setSceneGraphCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->scene_graph = event.getValue();
}

void
av::gua::Camera::getRenderMaskCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaCamera->render_mask;
}

void
av::gua::Camera::setRenderMaskCB(const SFString::SetValueEvent& event)
{
  m_guaCamera->render_mask = event.getValue();
}


void
av::gua::Camera::getModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaCamera->mode);
}

void
av::gua::Camera::setModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaCamera->mode = static_cast< ::gua::Camera::ProjectionMode>(event.getValue());
}
