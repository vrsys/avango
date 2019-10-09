#include <avango/gua/lod/renderer/AuxAtlasTile.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>



AV_FC_DEFINE(av::gua::lod::AuxAtlasTile);

AV_FIELD_DEFINE(av::gua::lod::SFAuxAtlasTile);
AV_FIELD_DEFINE(av::gua::lod::MFAuxAtlasTile);

av::gua::lod::AuxAtlasTile::AuxAtlasTile(std::shared_ptr< ::gua::Auxi::atlas_tile> guanode)
    : m_guaAuxAtlasTile(guanode)
{}

void
av::gua::lod::AuxAtlasTile::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::AuxAtlasTile, true);

        SFAuxAtlasTile::initClass("av::gua::SFAuxAtlasTile", "av::Field");
        MFAuxAtlasTile::initClass("av::gua::MFAuxAtlasTile", "av::Field");
    }
}


void 
av::gua::lod::AuxAtlasTile::getAtlasTileIdCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlasTile->atlas_tile_id_;
}
        
void 
av::gua::lod::AuxAtlasTile::getXCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlasTile->x_;
}

void 
av::gua::lod::AuxAtlasTile::getYCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlasTile->y_;
}

void 
av::gua::lod::AuxAtlasTile::getWidthCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlasTile->width_;
}

void 
av::gua::lod::AuxAtlasTile::getHeightCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlasTile->height_;
}



std::shared_ptr< ::gua::Auxi::atlas_tile>
av::gua::lod::AuxAtlasTile::getGuaAuxAtlasTile() const {
  return m_guaAuxAtlasTile;
}