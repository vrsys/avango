#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>
#include <avango/Base.h>
#include <functional>

#include <gua/databases/GeometryDescription.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SkeletalAnimationNode"));
}

AV_FC_DEFINE(av::gua::skelanim::SkeletalAnimationNode);

AV_FIELD_DEFINE(av::gua::skelanim::SFSkeletalAnimationNode);
AV_FIELD_DEFINE(av::gua::skelanim::MFSkeletalAnimationNode);

av::gua::skelanim::SkeletalAnimationNode::SkeletalAnimationNode(std::shared_ptr< ::gua::node::SkeletalAnimationNode> guanode)
  : GeometryNode(guanode)
  , m_guaSkeletalAnimationNode(std::dynamic_pointer_cast< ::gua::node::SkeletalAnimationNode>(GeometryNode::getGuaNode()))
  , m_materialsUserDataHandle{0}
{
  // trimesh
  AV_FC_ADD_ADAPTOR_FIELD(Animations,
                      std::bind(&SkeletalAnimationNode::getAnimationsCB, this, std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setAnimationsCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Geometries,
                        std::bind(&SkeletalAnimationNode::getGeometriesCB, this,std::placeholders::_1),
                        std::bind(&SkeletalAnimationNode::setGeometriesCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&SkeletalAnimationNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&SkeletalAnimationNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&SkeletalAnimationNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

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
  // skelanim
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

av::gua::skelanim::SkeletalAnimationNode::~SkeletalAnimationNode() {
  if (m_guaSkeletalAnimationNode) {
    auto avGuaMaterials(static_cast<av::MultiField<av::Link<Material>>::ContainerType*>(m_guaSkeletalAnimationNode->get_user_data(m_materialsUserDataHandle)));

    if (avGuaMaterials) {
      avGuaMaterials->clear();
      delete avGuaMaterials;
    }
  }
} 

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::skelanim::SkeletalAnimationNode::on_distribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_distribute(netNode);

  auto avGuaMaterials(static_cast<av::MultiField<av::Link<Material>>::ContainerType*>(m_guaSkeletalAnimationNode->get_user_data(m_materialsUserDataHandle)));
  for(auto& material : *avGuaMaterials) {
    if (material.isValid()) {
      material->on_distribute(netNode);
      netNode.distributeFieldContainer(material);
    }
  }
}

void av::gua::skelanim::SkeletalAnimationNode::on_undistribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_undistribute(netNode);

  auto avGuaMaterials(static_cast<av::MultiField<av::Link<Material>>::ContainerType*>(m_guaSkeletalAnimationNode->get_user_data(m_materialsUserDataHandle)));
  for(auto& material : *avGuaMaterials) {
    if (material.isValid()) {
      material->on_undistribute(netNode);
      netNode.undistributeFieldContainer(material);
    }
  }
}
#endif

void
av::gua::skelanim::SkeletalAnimationNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::skelanim::SkeletalAnimationNode, true);

    SFSkeletalAnimationNode::initClass("av::gua::skelanim::SFSkeletalAnimationNode", "av::Field");
    MFSkeletalAnimationNode::initClass("av::gua::skelanim::MFSkeletalAnimationNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::skelanim::SkeletalAnimationNode::getGeometriesCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_geometry_descriptions();
}

void
av::gua::skelanim::SkeletalAnimationNode::setGeometriesCB(const MFString::SetValueEvent& event)
{ 
  m_guaSkeletalAnimationNode->set_geometry_descriptions(event.getValue());
  // if not just empry descritions are transferred, load skeleton from given file
  if (!event.getValue().empty()) {
    ::gua::GeometryDescription desc{event.getValue()[0]};
    m_guaSkeletalAnimationNode->set_skeleton_description(desc.filepath());
  }
}

void
av::gua::skelanim::SkeletalAnimationNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_render_to_gbuffer();
}

void
av::gua::skelanim::SkeletalAnimationNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_render_to_gbuffer(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSkeletalAnimationNode->get_render_to_stencil_buffer();
}

void
av::gua::skelanim::SkeletalAnimationNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaSkeletalAnimationNode->set_render_to_stencil_buffer(event.getValue());
}

void
av::gua::skelanim::SkeletalAnimationNode::loadAnimation(std::string const& file_name,
                                                 std::string const& animation_name)
{
  m_guaSkeletalAnimationNode->add_animations(file_name, animation_name);
  // add animation to list
  auto names = AnimationsInternal.getValue();
  names.push_back(file_name + "|" + animation_name);
  AnimationsInternal.setValue(names);
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

    av::gua::MFMaterial::ContainerType const& materials(event.getValue());

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
av::gua::skelanim::SkeletalAnimationNode::getAnimationsCB(const MFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = AnimationsInternal.getValue();
}

void
av::gua::skelanim::SkeletalAnimationNode::setAnimationsCB(const MFString::SetValueEvent& event)
{
  auto const& descriptions = event.getValue();
  for(auto const& description : descriptions) {
    std::size_t index_sep = description.find('|');
    std::string path = description.substr(0, index_sep);
    std::string name = description.substr(index_sep + 1);
    loadAnimation(path, name);
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
