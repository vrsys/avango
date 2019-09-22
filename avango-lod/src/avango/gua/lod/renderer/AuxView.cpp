#include <avango/gua/lod/renderer/AuxView.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>



AV_FC_DEFINE(av::gua::lod::AuxView);

AV_FIELD_DEFINE(av::gua::lod::SFAuxView);
AV_FIELD_DEFINE(av::gua::lod::MFAuxView);

av::gua::lod::AuxView::AuxView(std::shared_ptr< ::gua::Auxi::view> guanode)
    : m_guaAuxView(guanode)
{}

void
av::gua::lod::AuxView::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::AuxView, true);

        SFAuxView::initClass("av::gua::SFAuxView", "av::Field");
        MFAuxView::initClass("av::gua::MFAuxView", "av::Field");
    }
}


void 
av::gua::lod::AuxView::getCameraIdCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->camera_id_;
}
void 
av::gua::lod::AuxView::getPositionCB(const SFVec3::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->position_;
}
void 
av::gua::lod::AuxView::getTransformCB(const SFMatrix4::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->transform_;
}
void 
av::gua::lod::AuxView::getFocalValueXCB(const SFFloat::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->focal_value_x_;
}
void 
av::gua::lod::AuxView::getFocalValueYCB(const SFFloat::GetValueEvent& event){
  *(event.getValuePtr()) =  m_guaAuxView->focal_value_y_;
}

void 
av::gua::lod::AuxView::getCenterXCB(const SFFloat::GetValueEvent& event){
  *(event.getValuePtr()) =  m_guaAuxView->center_x_;
}


void 
av::gua::lod::AuxView::getCenterYCB(const SFFloat::GetValueEvent& event){
  *(event.getValuePtr()) =  m_guaAuxView->center_y_;
}


void 
av::gua::lod::AuxView::getDistortionCB(const SFFloat::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->distortion_;
}
void 
av::gua::lod::AuxView::getImageWidthCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->image_width_;
}
void 
av::gua::lod::AuxView::getImageHeightCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->image_height_;
}
void 
av::gua::lod::AuxView::getAtlasTileIdCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->atlas_tile_id_;
}
void 
av::gua::lod::AuxView::getImageFileCB(const SFString::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxView->image_file_;
}


std::shared_ptr< ::gua::Auxi::view>
av::gua::lod::AuxView::getGuaAuxView() const {
  return m_guaAuxView;
}