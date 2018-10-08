#include <avango/gua/lod/renderer/OctreeNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>



AV_FC_DEFINE(av::gua::lod::OctreeNode);

AV_FIELD_DEFINE(av::gua::lod::SFOctreeNode);
AV_FIELD_DEFINE(av::gua::lod::MFOctreeNode);

av::gua::lod::OctreeNode::OctreeNode(std::shared_ptr< ::gua::Aux::sparse_point> guanode)
    : m_guaOctreeNode(guanode)
{}

void
av::gua::lod::OctreeNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::lod::OctreeNode, true);

        SFOctreeNode::initClass("av::gua::SFOctreeNode", "av::Field");
        MFOctreeNode::initClass("av::gua::MFOctreeNode", "av::Field");
    }
}



void 
av::gua::lod::OctreeNode::getIdxCB(const SFVec3::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaOctreeNode->get_idx();
}

void 
av::gua::lod::OctreeNode::getChildMaskCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaOctreeNode->get_child_mask();
}
void 
av::gua::lod::OctreeNode::getChildIdxCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaOctreeNode->get_child_idx();
}
void 
av::gua::lod::OctreeNode::getMinCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaOctreeNode->get_min();
}
void 
av::gua::lod::OctreeNode::getMaxCB(const SFUInt::GetValueEvent& event){
  *(event.getValuePtr()) = m_guaOctreeNode->get_max();
}


std::shared_ptr< ::gua::Aux::sparse_point>
av::gua::lod::OctreeNode::getGuaOctreeNode() const {
  return m_guaOctreeNode;
}