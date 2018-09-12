#include <avango/gua/lod/renderer/AuxAtlas.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>



AV_FC_DEFINE(av::gua::lod::AuxAtlas);

AV_FIELD_DEFINE(av::gua::lod::SFAuxAtlas);
AV_FIELD_DEFINE(av::gua::lod::MFAuxAtlas);

av::gua::lod::AuxAtlas::AuxAtlas(std::shared_ptr< ::gua::Aux::atlas> guanode)
    : m_guaAuxAtlas(guanode)
{
  
}

void
av::gua::lod::AuxAtlas::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::AuxAtlas, true);

        SFAuxAtlas::initClass("av::gua::SFAuxAtlas", "av::Field");
        MFAuxAtlas::initClass("av::gua::MFAuxAtlas", "av::Field");
    }
}

void 
av::gua::lod::AuxAtlas::getNumAtlasTilesCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlas->num_atlas_tiles_;
}
        
void 
av::gua::lod::AuxAtlas::getAtlasWidthCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlas->atlas_width_;
}
        
void 
av::gua::lod::AuxAtlas::getAtlasHeightCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlas->atlas_height_;
}
        
void 
av::gua::lod::AuxAtlas::getRotatedCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaAuxAtlas->rotated_;
}



std::shared_ptr< ::gua::Aux::atlas>
av::gua::lod::AuxAtlas::getGuaAuxAtlas() const {
  return m_guaAuxAtlas;
}