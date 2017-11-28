#include <avango/gua/scenegraph/LineStripNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::LineStripNode);

AV_FIELD_DEFINE(av::gua::SFLineStripNode);
AV_FIELD_DEFINE(av::gua::MFLineStripNode);

av::gua::LineStripNode::LineStripNode(std::shared_ptr< ::gua::node::LineStripNode> guanode)
    : GeometryNode(guanode)
    , m_guaLineStripNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&LineStripNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&LineStripNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&LineStripNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&LineStripNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&LineStripNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::LineStripNode::on_distribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::LineStripNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}
#endif

void
av::gua::LineStripNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::LineStripNode, true);

    SFLineStripNode::initClass("av::gua::SFLineStripNode", "av::Field");
    MFLineStripNode::initClass("av::gua::MFLineStripNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::LineStripNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_geometry_description();
}

void
av::gua::LineStripNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaLineStripNode->set_geometry_description(event.getValue());
}

void
av::gua::LineStripNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::LineStripNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaLineStripNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::LineStripNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_gbuffer();
}

void
av::gua::LineStripNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_gbuffer(event.getValue());
}

void
av::gua::LineStripNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_stencil_buffer();
}

void
av::gua::LineStripNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_stencil_buffer(event.getValue());
}

std::shared_ptr< ::gua::node::LineStripNode>
av::gua::LineStripNode::getGuaLineStripNode() const {
  return m_guaLineStripNode;
}
