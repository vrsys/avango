#include <avango/gua/math/Frustum.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
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
                        boost::bind(&Frustum::getClipNearCB, this, _1),
                        boost::bind(&Frustum::setClipNearCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ClipFar,
                        boost::bind(&Frustum::getClipFarCB, this, _1),
                        boost::bind(&Frustum::setClipFarCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(CameraTransform,
                        boost::bind(&Frustum::getCameraTransformCB, this, _1),
                        boost::bind(&Frustum::setCameraTransformCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenTransform,
                        boost::bind(&Frustum::getScreenTransformCB, this, _1),
                        boost::bind(&Frustum::setScreenTransformCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ViewMatrix,
                        boost::bind(&Frustum::getViewMatrixCB, this, _1),
                        boost::bind(&Frustum::setViewMatrixCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ProjectionMatrix,
                        boost::bind(&Frustum::getProjectionMatrixCB, this, _1),
                        boost::bind(&Frustum::setProjectionMatrixCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Corners,
                        boost::bind(&Frustum::getCornersCB, this, _1),
                        boost::bind(&Frustum::setCornersCB, this, _1));

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
av::gua::Frustum::getClipNearCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_clip_near();
}

/* virtual */ void 
av::gua::Frustum::setClipNearCB(const SFFloat::SetValueEvent& event)
{
  // std::cout << "Error in av::gua::Frustum: Frustum is read-only structure!" << std::endl;
}


/* virtual */ void 
av::gua::Frustum::getClipFarCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaFrustum->get_clip_far();

}

/* virtual */ void 
av::gua::Frustum::setClipFarCB(const SFFloat::SetValueEvent& event)
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

