#include <avango/gua/scenegraph/TriMeshNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::TriMeshNode);

AV_FIELD_DEFINE(av::gua::SFTriMeshNode);
AV_FIELD_DEFINE(av::gua::MFTriMeshNode);

av::gua::TriMeshNode::TriMeshNode(std::shared_ptr< ::gua::node::TriMeshNode> guanode)
    : GeometryNode(guanode)
    , m_guaTriMeshNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&TriMeshNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&TriMeshNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&TriMeshNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&TriMeshNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&TriMeshNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&TriMeshNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&TriMeshNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&TriMeshNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::TriMeshNode::on_distribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::TriMeshNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}
#endif

void
av::gua::TriMeshNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::TriMeshNode, true);

    SFTriMeshNode::initClass("av::gua::SFTriMeshNode", "av::Field");
    MFTriMeshNode::initClass("av::gua::MFTriMeshNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::TriMeshNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTriMeshNode->get_geometry_description();
}

void
av::gua::TriMeshNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaTriMeshNode->set_geometry_description(event.getValue());
}

void
av::gua::TriMeshNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::TriMeshNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaTriMeshNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::TriMeshNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTriMeshNode->get_render_to_gbuffer();
}

void
av::gua::TriMeshNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaTriMeshNode->set_render_to_gbuffer(event.getValue());
}

void
av::gua::TriMeshNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTriMeshNode->get_render_to_stencil_buffer();
}

void
av::gua::TriMeshNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaTriMeshNode->set_render_to_stencil_buffer(event.getValue());
}

std::shared_ptr< ::gua::node::TriMeshNode>
av::gua::TriMeshNode::getGuaTriMeshNode() const {
  return m_guaTriMeshNode;
}
