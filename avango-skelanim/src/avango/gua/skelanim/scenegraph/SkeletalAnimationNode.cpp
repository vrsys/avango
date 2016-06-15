#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::skelanim::SkeletalAnimationNode);

AV_FIELD_DEFINE(av::gua::skelanim::SFSkeletalAnimationNode);
AV_FIELD_DEFINE(av::gua::skelanim::MFSkeletalAnimationNode);

av::gua::skelanim::SkeletalAnimationNode::SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode)
    : GeometryNode(guanode)
    , m_materialsUserDataHandle{0}
    , m_guaSkeletalAnimationNode(std::dynamic_pointer_cast< ::gua::node::SkeletalAnimationNode>(GeometryNode::getGuaNode()))
{
  //crate user data for avango material links
  auto avGuaMaterials(new av::MultiField<av::Link<av::gua::Material>>::ContainerType());
  m_materialsUserDataHandle = m_guaSkeletalAnimationNode->add_user_data(avGuaMaterials);
  //fill field with gua materials
  for (auto const& material: m_guaSkeletalAnimationNode->get_materials()) {
        avGuaMaterials->push_back(av::Link<av::gua::Material>(new av::gua::Material(material)));
  }

  AV_FC_ADD_ADAPTOR_FIELD(Materials,
                      std::bind(&SkeletalAnimationNode::getMaterialsCB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setMaterialsCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Animation1,
                      std::bind(&SkeletalAnimationNode::getAnimation1CB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setAnimation1CB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Animation2,
                      std::bind(&SkeletalAnimationNode::getAnimation2CB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setAnimation2CB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(BlendFactor,
                      std::bind(&SkeletalAnimationNode::getBlendFactorCB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setBlendFactorCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Time1,
                      std::bind(&SkeletalAnimationNode::getTime1CB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setTime1CB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Time2,
                      std::bind(&SkeletalAnimationNode::getTime2CB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setTime2CB, this, std::placeholders::_1));
}

//av::gua::skelanim::SkeletalAnimationNode::~SkeletalAnimationNode()
//{}

void
av::gua::skelanim::SkeletalAnimationNode::loadAnimation(std::string const& file_name,
                                                 std::string const& animation_name) const
{
  m_guaSkeletalAnimationNode->add_animations(file_name, animation_name);
}

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

/* virtual */ void
av::gua::skelanim::SkeletalAnimationNode::getMaterialsCB(const av::gua::MFMaterial::GetValueEvent& event)
{
  *(event.getValuePtr()) = *static_cast<av::MultiField<av::Link<Material>>::ContainerType*>(m_guaSkeletalAnimationNode->get_user_data(m_materialsUserDataHandle));
}

/* virtual */ void
av::gua::skelanim::SkeletalAnimationNode::setMaterialsCB(const av::gua::MFMaterial::SetValueEvent& event)
{
  if (m_guaSkeletalAnimationNode) {

    m_guaSkeletalAnimationNode->clear_materials();
    auto avGuaMaterials(static_cast<av::MultiField<av::Link<Material>>::ContainerType*>(m_guaSkeletalAnimationNode->get_user_data(m_materialsUserDataHandle)));

    if (avGuaMaterials) avGuaMaterials->clear();

    const av::gua::MFMaterial::ContainerType &materials(event.getValue());

    for (auto& material: materials) {
      if (material->getGuaMaterial() != nullptr) {
        m_guaSkeletalAnimationNode->add_material(material->getGuaMaterial());
        if (avGuaMaterials) {
          avGuaMaterials->push_back(material);
        }
      }
    }

  } else {
    std::cout << "Cannot set materials of null node!" << std::endl;
  }
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
