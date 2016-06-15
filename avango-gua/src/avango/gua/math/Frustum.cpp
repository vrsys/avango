#include <avango/gua/math/Frustum.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Frustum"));
}

AV_FC_DEFINE(av::gua::Frustum);

AV_FIELD_DEFINE(av::gua::SFFrustum);
AV_FIELD_DEFINE(av::gua::MFFrustum);

av::gua::Frustum::Frustum(::gua::Frustum* guaFrustum)
  : m_guaFrustum(guaFrustum)
{

  AV_FC_ADD_ADAPTOR_FIELD(ClipNear,
                        std::bind(&Frustum::getClipNearCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setClipNearCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ClipFar,
                        std::bind(&Frustum::getClipFarCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setClipFarCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(CameraTransform,
                        std::bind(&Frustum::getCameraTransformCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setCameraTransformCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenTransform,
                        std::bind(&Frustum::getScreenTransformCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setScreenTransformCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ViewMatrix,
                        std::bind(&Frustum::getViewMatrixCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setViewMatrixCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ProjectionMatrix,
                        std::bind(&Frustum::getProjectionMatrixCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setProjectionMatrixCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Corners,
                        std::bind(&Frustum::getCornersCB, this, std::placeholders::_1),
                        std::bind(&Frustum::setCornersCB, this, std::placeholders::_1));

}

av::gua::Frustum::~Frustum()
{}

void
av::gua::Frustum::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Frustum, true);

        SFFrustum::initClass("av::gua::SFFrustum", "av::Field");
        MFFrustum::initClass("av::gua::MFFrustum", "av::Field");
    }
}

::gua::Frustum*
av::gua::Frustum::getGuaFrustum() const
{
    return m_guaFrustum;
}

/* virtual */ void
av::gua::Frustum::getClipNearCB(const SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_clip_near();
}

/* virtual */ void
av::gua::Frustum::setClipNearCB(const SFDouble::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getClipFarCB(const SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_clip_far();

}

/* virtual */ void
av::gua::Frustum::setClipFarCB(const SFDouble::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getCameraTransformCB(const SFMatrix4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_camera_transform();
}

/* virtual */ void
av::gua::Frustum::setCameraTransformCB(const SFMatrix4::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getScreenTransformCB(const SFMatrix4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_screen_transform();
}

/* virtual */ void
av::gua::Frustum::setScreenTransformCB(const SFMatrix4::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getViewMatrixCB(const SFMatrix4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_view();
}

/* virtual */ void
av::gua::Frustum::setViewMatrixCB(const SFMatrix4::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getProjectionMatrixCB(const SFMatrix4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_projection();
}

/* virtual */ void
av::gua::Frustum::setProjectionMatrixCB(const SFMatrix4::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void
av::gua::Frustum::getCornersCB(const MFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_corners();
}

/* virtual */ void
av::gua::Frustum::setCornersCB(const MFVec3::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}

