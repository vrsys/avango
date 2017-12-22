#include <avango/gua/scenegraph/SPointsNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::SPointsNode);

AV_FIELD_DEFINE(av::gua::SFSPointsNode);
AV_FIELD_DEFINE(av::gua::MFSPointsNode);

av::gua::SPointsNode::SPointsNode(std::shared_ptr< ::gua::node::SPointsNode> guanode)
  : GeometryNode(guanode)
  , m_guaSPointsNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&SPointsNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&SPointsNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&SPointsNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&SPointsNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&SPointsNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&SPointsNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&SPointsNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&SPointsNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

void av::gua::SPointsNode::on_distribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::SPointsNode::on_undistribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}


void
av::gua::SPointsNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::SPointsNode, true);

    SFSPointsNode::initClass("av::gua::SFSPointsNode", "av::Field");
    MFSPointsNode::initClass("av::gua::MFSPointsNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::SPointsNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaSPointsNode->get_spoints_description();
}

void
av::gua::SPointsNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaSPointsNode->set_spoints_description(event.getValue());
}

void
av::gua::SPointsNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::SPointsNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaSPointsNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::SPointsNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
//  *(event.getValuePtr()) = m_guaSPointsNode->get_render_to_gbuffer();
}

void
av::gua::SPointsNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
//  m_guaSPointsNode->set_render_to_gbuffer(event.getValue());
}


void
av::gua::SPointsNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
//  *(event.getValuePtr()) = m_guaSPointsNode->get_render_to_stencil_buffer();
}

void
av::gua::SPointsNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
//  m_guaSPointsNode->set_render_to_stencil_buffer(event.getValue());
}

std::shared_ptr< ::gua::node::SPointsNode>
av::gua::SPointsNode::getGuaSPointsNode() const {
  return m_guaSPointsNode;
}
