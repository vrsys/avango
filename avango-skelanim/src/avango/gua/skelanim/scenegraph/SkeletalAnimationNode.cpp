#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::skelanim::SkeletalAnimationNode);

AV_FIELD_DEFINE(av::gua::skelanim::SFSkeletalAnimationNode);
AV_FIELD_DEFINE(av::gua::skelanim::MFSkeletalAnimationNode);

av::gua::skelanim::SkeletalAnimationNode::SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode)
    : GeometryNode(guanode)
    , m_guaSkeletalAnimationNode(std::dynamic_pointer_cast< ::gua::node::SkeletalAnimationNode>(GeometryNode::getGuaNode()))
{

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      boost::bind(&SkeletalAnimationNode::getMaterialCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(AnimationMode,
                      boost::bind(&SkeletalAnimationNode::getAnimationModeCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setAnimationModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendingMode,
                      boost::bind(&SkeletalAnimationNode::getBlendingModeCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setBlendingModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Animation,
                      boost::bind(&SkeletalAnimationNode::getAnimationCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setAnimationCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendingFactor,
                      boost::bind(&SkeletalAnimationNode::getBlendingFactorCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setBlendingFactorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendingDuration,
                      boost::bind(&SkeletalAnimationNode::getBlendingDurationCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setBlendingDurationCB, this, _1));

//  if (guanode->get_material()) {
//    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
//  }

}

//av::gua::skelanim::SkeletalAnimationNode::~SkeletalAnimationNode()
//{}

void
av::gua::skelanim::SkeletalAnimationNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::skelanim::SkeletalAnimationNode, true);

        SFSkeletalAnimationNode::initClass("av::gua::skelanim::SFSkeletalAnimationNode", "av::Field");
        MFSkeletalAnimationNode::initClass("av::gua::skelanim::MFSkeletalAnimationNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}


void
av::gua::skelanim::SkeletalAnimationNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::skelanim::SkeletalAnimationNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  m_Material = event.getValue();
//  m_guaSkeletalAnimationNode->set_material(m_Material->getGuaMaterial());
}

void
av::gua::skelanim::SkeletalAnimationNode::getAnimationModeCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_animation_mode();
}

void
av::gua::skelanim::SkeletalAnimationNode::setAnimationModeCB(const SFInt::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_animation_mode(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getBlendingModeCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_blending_mode();
}

void
av::gua::skelanim::SkeletalAnimationNode::setBlendingModeCB(const SFInt::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_blending_mode(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getAnimationCB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_animation();
}

void
av::gua::skelanim::SkeletalAnimationNode::setAnimationCB(const SFString::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_animation(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getBlendingFactorCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_blending_factor();
}

void
av::gua::skelanim::SkeletalAnimationNode::setBlendingFactorCB(const SFFloat::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_blending_factor(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getBlendingDurationCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_blending_duration();
}

void
av::gua::skelanim::SkeletalAnimationNode::setBlendingDurationCB(const SFFloat::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_blending_duration(event.getValue());
}
