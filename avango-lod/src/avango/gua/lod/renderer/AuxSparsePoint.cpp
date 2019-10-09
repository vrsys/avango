#include <avango/gua/lod/renderer/AuxSparsePoint.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

#include <avango/gua/lod/renderer/AuxFeature.hpp>



AV_FC_DEFINE(av::gua::lod::AuxSparsePoint);

AV_FIELD_DEFINE(av::gua::lod::SFAuxSparsePoint);
AV_FIELD_DEFINE(av::gua::lod::MFAuxSparsePoint);

av::gua::lod::AuxSparsePoint::AuxSparsePoint(std::shared_ptr< ::gua::Auxi::sparse_point> guanode)
    : m_guaAuxSparsePoint(guanode)
{}

void
av::gua::lod::AuxSparsePoint::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::AuxSparsePoint, true);

        SFAuxSparsePoint::initClass("av::gua::SFAuxSparsePoint", "av::Field");
        MFAuxSparsePoint::initClass("av::gua::MFAuxSparsePoint", "av::Field");
    }
}



void 
av::gua::lod::AuxSparsePoint::getPositionCB(const SFVec3::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxSparsePoint->pos_;
}

void 
av::gua::lod::AuxSparsePoint::getRedCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxSparsePoint->r_;
}
void 
av::gua::lod::AuxSparsePoint::getGreenCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxSparsePoint->g_;
}
void 
av::gua::lod::AuxSparsePoint::getBlueCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxSparsePoint->b_;
}
void 
av::gua::lod::AuxSparsePoint::getAlphaCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxSparsePoint->a_;
}

av::Link<av::gua::lod::AuxFeature> 
av::gua::lod::AuxSparsePoint::getFeatureById(uint32_t id) const {
if (m_guaAuxSparsePoint){
  return av::Link<av::gua::lod::AuxFeature>(new av::gua::lod::AuxFeature(m_guaAuxSparsePoint->getFeatureById(id)));
}
  // return m_guaAuxSparsePoint->features_.at(id-1);
else
  return av::Link<av::gua::lod::AuxFeature>(new av::gua::lod::AuxFeature(m_guaAuxSparsePoint->getFeatureById(-1)));
}

std::shared_ptr< ::gua::Auxi::sparse_point>
av::gua::lod::AuxSparsePoint::getGuaAuxSparsePoint() const {
  return m_guaAuxSparsePoint;
}