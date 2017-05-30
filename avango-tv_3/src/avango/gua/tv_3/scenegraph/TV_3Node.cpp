#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::tv_3::TV_3Node);

AV_FIELD_DEFINE(av::gua::tv_3::SFTV_3Node);
AV_FIELD_DEFINE(av::gua::tv_3::MFTV_3Node);

av::gua::tv_3::TV_3Node::TV_3Node(std::shared_ptr< ::gua::node::TV_3Node> guanode)
    : GeometryNode(guanode)
    , m_guaTV_3Node(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&TV_3Node::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&TV_3Node::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&TV_3Node::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&TV_3Node::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderMode,
                      std::bind(&TV_3Node::getRenderModeCB, this,std::placeholders::_1),
                      std::bind(&TV_3Node::setRenderModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SpatialFilterMode,
                      std::bind(&TV_3Node::getSpatialFilterModeCB, this,std::placeholders::_1),
                      std::bind(&TV_3Node::enableSpatialLinearFilterCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(TemporalFilterMode,
                      std::bind(&TV_3Node::getTemporalFilterModeCB, this,std::placeholders::_1),
                      std::bind(&TV_3Node::enableTemporalLinearFilterCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(IsoValue,
                      std::bind(&TV_3Node::getIsoValueCB, this,std::placeholders::_1),
                      std::bind(&TV_3Node::setIsoValueCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }
}

//av::gua::tv_3::TV_3Node::~TV_3Node()
//{}

void
av::gua::tv_3::TV_3Node::on_distribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_distribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_distribute(netNode);
  }
  netNode.distributeFieldContainer(m_Material);
}

void
av::gua::tv_3::TV_3Node::on_undistribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_undistribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_Material);
}

void
av::gua::tv_3::TV_3Node::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::tv_3::TV_3Node, true);

    SFTV_3Node::initClass("av::gua::tv_3::SFTV_3Node", "av::Field");
    MFTV_3Node::initClass("av::gua::tv_3::MFTV_3Node", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::tv_3::TV_3Node::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTV_3Node->get_geometry_description();
}

void
av::gua::tv_3::TV_3Node::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaTV_3Node->set_geometry_description(event.getValue());
}

void
av::gua::tv_3::TV_3Node::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::tv_3::TV_3Node::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaTV_3Node->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::tv_3::TV_3Node::getRenderModeCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTV_3Node->get_render_mode();
}

void
av::gua::tv_3::TV_3Node::setRenderModeCB(const SFInt::SetValueEvent& event)
{


  m_guaTV_3Node->set_render_mode( static_cast<::gua::node::TV_3Node::RenderMode>(event.getValue()) );
}

void
av::gua::tv_3::TV_3Node::getSpatialFilterModeCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTV_3Node->get_spatial_filter_mode();
}

void
av::gua::tv_3::TV_3Node::enableSpatialLinearFilterCB(const SFInt::SetValueEvent& event)
{
  m_guaTV_3Node->enable_spatial_linear_filter(event.getValue());
}

void
av::gua::tv_3::TV_3Node::getTemporalFilterModeCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTV_3Node->get_temporal_filter_mode();
}

void
av::gua::tv_3::TV_3Node::enableTemporalLinearFilterCB(const SFInt::SetValueEvent& event)
{
  m_guaTV_3Node->enable_temporal_linear_filter(event.getValue());
}

void
av::gua::tv_3::TV_3Node::getIsoValueCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaTV_3Node->get_iso_value();
}

void
av::gua::tv_3::TV_3Node::setIsoValueCB(const SFFloat::SetValueEvent& event)
{
  m_guaTV_3Node->set_iso_value(event.getValue());
}

std::shared_ptr< ::gua::node::TV_3Node>
av::gua::tv_3::TV_3Node::getGuaTV_3Node() const {
  return m_guaTV_3Node;
}
