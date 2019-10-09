#include <avango/gua/lod/renderer/AuxFeature.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>



AV_FC_DEFINE(av::gua::lod::AuxFeature);

AV_FIELD_DEFINE(av::gua::lod::SFAuxFeature);
AV_FIELD_DEFINE(av::gua::lod::MFAuxFeature);

av::gua::lod::AuxFeature::AuxFeature(std::shared_ptr< ::gua::Auxi::feature> guanode)
    : m_guaAuxFeature(guanode)
{}

void
av::gua::lod::AuxFeature::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::AuxFeature, true);

        SFAuxFeature::initClass("av::gua::SFAuxFeature", "av::Field");
        MFAuxFeature::initClass("av::gua::MFAuxFeature", "av::Field");
    }
}


void 
av::gua::lod::AuxFeature::getCameraIdCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxFeature->camera_id_;
}

void 
av::gua::lod::AuxFeature::getUsingCountCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxFeature->using_count_;
}

void 
av::gua::lod::AuxFeature::getCoordsCB(const SFVec2::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxFeature->coords_;
}

void 
av::gua::lod::AuxFeature::getErrorCB(const SFVec2::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxFeature->error_;
}



std::shared_ptr< ::gua::Auxi::feature>
av::gua::lod::AuxFeature::getGuaAuxFeature() const {
  return m_guaAuxFeature;
}