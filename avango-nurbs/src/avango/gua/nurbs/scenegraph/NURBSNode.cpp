#include <avango/gua/nurbs/scenegraph/NURBSNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::nurbs::NURBSNode);

AV_FIELD_DEFINE(av::gua::nurbs::SFNURBSNode);
AV_FIELD_DEFINE(av::gua::nurbs::MFNURBSNode);

av::gua::nurbs::NURBSNode::NURBSNode(std::shared_ptr< ::gua::node::NURBSNode> guanode)
    : GeometryNode(guanode)
    , m_guaNURBSNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&NURBSNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&NURBSNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&NURBSNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&NURBSNode::setMaterialCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }
}

//av::gua::nurbs::NURBSNode::~NURBSNode()
//{}

void
av::gua::nurbs::NURBSNode::on_distribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_distribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_distribute(netNode);
  }
  netNode.distributeFieldContainer(m_Material);
}

void
av::gua::nurbs::NURBSNode::on_undistribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_undistribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_Material);
}

void
av::gua::nurbs::NURBSNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::nurbs::NURBSNode, true);

    SFNURBSNode::initClass("av::gua::nurbs::SFNURBSNode", "av::Field");
    MFNURBSNode::initClass("av::gua::nurbs::MFNURBSNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::nurbs::NURBSNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNURBSNode->get_geometry_description();
}

void
av::gua::nurbs::NURBSNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaNURBSNode->set_geometry_description(event.getValue());
}

void
av::gua::nurbs::NURBSNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::nurbs::NURBSNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaNURBSNode->set_material(m_Material->getGuaMaterial());
  }
}

std::shared_ptr< ::gua::node::NURBSNode>
av::gua::nurbs::NURBSNode::getGuaNURBSNode() const {
  return m_guaNURBSNode;
}

void av::gua::nurbs::NURBSNode::getPixelErrorToleranceCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaNURBSNode->max_tesselation_error();
}

void av::gua::nurbs::NURBSNode::setPixelErrorToleranceCB(const SFFloat::SetValueEvent& event)
{
  m_guaNURBSNode->max_tesselation_error(event.getValue());
}