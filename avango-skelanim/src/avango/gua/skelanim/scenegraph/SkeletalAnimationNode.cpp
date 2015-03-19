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

  AV_FC_ADD_ADAPTOR_FIELD(Animation1,
                      boost::bind(&SkeletalAnimationNode::getAnimation1CB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setAnimation1CB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Animation2,
                      boost::bind(&SkeletalAnimationNode::getAnimation2CB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setAnimation2CB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendFactor,
                      boost::bind(&SkeletalAnimationNode::getBlendFactorCB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setBlendFactorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Time1,
                      boost::bind(&SkeletalAnimationNode::getTime1CB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setTime1CB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Time2,
                      boost::bind(&SkeletalAnimationNode::getTime2CB, this, _1),
                      boost::bind(&SkeletalAnimationNode::setTime2CB, this, _1));

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

float
av::gua::skelanim::SkeletalAnimationNode::getAnimDuration(std::string const& name) const
{
    return m_guaSkeletalAnimationNode->get_duration(name);
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
av::gua::skelanim::SkeletalAnimationNode::getAnimation1CB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_animation_1();
}

void
av::gua::skelanim::SkeletalAnimationNode::setAnimation1CB(const SFString::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_animation_1(event.getValue());
}
void
av::gua::skelanim::SkeletalAnimationNode::getAnimation2CB(const SFString::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_animation_2();
}

void
av::gua::skelanim::SkeletalAnimationNode::setAnimation2CB(const SFString::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_animation_2(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getBlendFactorCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_blend_factor();
}

void
av::gua::skelanim::SkeletalAnimationNode::setBlendFactorCB(const SFFloat::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_blend_factor(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getTime1CB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_time_1();
}

void
av::gua::skelanim::SkeletalAnimationNode::setTime1CB(const SFFloat::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_time_1(event.getValue());
}
void
av::gua::skelanim::SkeletalAnimationNode::getTime2CB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_time_2();
}

void
av::gua::skelanim::SkeletalAnimationNode::setTime2CB(const SFFloat::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_time_2(event.getValue());
}
