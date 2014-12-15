#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::TriMeshNode);

AV_FIELD_DEFINE(av::gua::SFTriMeshNode);
AV_FIELD_DEFINE(av::gua::MFTriMeshNode);

av::gua::TriMeshNode::TriMeshNode(std::shared_ptr< ::gua::node::TriMeshNode> guanode)
    : GeometryNode(guanode)
{

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      boost::bind(&TriMeshNode::getMaterialCB, this, _1),
                      boost::bind(&TriMeshNode::setMaterialCB, this, _1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

//av::gua::TriMeshNode::~TriMeshNode()
//{}

void
av::gua::TriMeshNode::initClass()
{
  if (!isTypeInitialized())
  {
      av::gua::GeometryNode::initClass();

      AV_FC_INIT(av::gua::GeometryNode, av::gua::TriMeshNode, true);

      SFTriMeshNode::initClass("av::gua::SFTriMeshNode", "av::Field");
      MFTriMeshNode::initClass("av::gua::MFTriMeshNode", "av::Field");

      sClassTypeId.setDistributable(true);
  }
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
  m_Material = event.getValue();
  m_guaTriMeshNode->set_material(m_Material->getGuaMaterial());
}

std::shared_ptr< ::gua::node::TriMeshNode>
av::gua::TriMeshNode::getGuaTriMeshNode() const {
  return m_guaTriMeshNode;
}
