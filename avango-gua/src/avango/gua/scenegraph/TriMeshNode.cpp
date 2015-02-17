#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TriMeshNode);

AV_FIELD_DEFINE(av::gua::SFTriMeshNode);
AV_FIELD_DEFINE(av::gua::MFTriMeshNode);

av::gua::TriMeshNode::TriMeshNode(std::shared_ptr< ::gua::node::TriMeshNode> guanode)
    : GeometryNode(guanode)
    , m_guaTriMeshNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&TriMeshNode::getGeometryCB, this, _1),
                        boost::bind(&TriMeshNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      boost::bind(&TriMeshNode::getMaterialCB, this, _1),
                      boost::bind(&TriMeshNode::setMaterialCB, this, _1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

//av::gua::TriMeshNode::~TriMeshNode()
//{}

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

std::shared_ptr< ::gua::node::TriMeshNode>
av::gua::TriMeshNode::getGuaTriMeshNode() const {
  return m_guaTriMeshNode;
}
