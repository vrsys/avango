#include <avango/gua/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::SkeletalAnimationNode);

AV_FIELD_DEFINE(av::gua::SFSkeletalAnimationNode);
AV_FIELD_DEFINE(av::gua::MFSkeletalAnimationNode);

av::gua::SkeletalAnimationNode::SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode)
    : GeometryNode(guanode)
{

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      boost::bind(&SkeletalAnimationNode::getMaterialCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setMaterialCB, this, _1));

//  if (guanode->get_material()) {
//    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
//  }

}

//av::gua::SkeletalAnimationNode::~SkeletalAnimationNode()
//{}

void
av::gua::SkeletalAnimationNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::SkeletalAnimationNode, true);

        SFSkeletalAnimationNode::initClass("av::gua::SFSkeletalAnimationNode", "av::Field");
        MFSkeletalAnimationNode::initClass("av::gua::MFSkeletalAnimationNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}


void
av::gua::SkeletalAnimationNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::SkeletalAnimationNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  m_Material = event.getValue();
//  m_guaSkeletalAnimationNode->set_material(m_Material->getGuaMaterial());
}

std::shared_ptr< ::gua::node::SkeletalAnimationNode>
av::gua::SkeletalAnimationNode::getGuaSkeletalAnimationNode() const {
  return m_guaSkeletalAnimationNode;
}
