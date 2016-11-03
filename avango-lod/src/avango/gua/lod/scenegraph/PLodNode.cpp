#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::lod::PLodNode);

AV_FIELD_DEFINE(av::gua::lod::SFPLodNode);
AV_FIELD_DEFINE(av::gua::lod::MFPLodNode);

av::gua::lod::PLodNode::PLodNode(std::shared_ptr< ::gua::node::PLodNode> guanode)
    : GeometryNode(guanode)
    , m_guaPLodNode(std::dynamic_pointer_cast< ::gua::node::PLodNode>(GeometryNode::getGuaNode()))
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&PLodNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&PLodNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&PLodNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&PLodNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RadiusScale,
                      std::bind(&PLodNode::getRadiusScaleCB, this,std::placeholders::_1),
                      std::bind(&PLodNode::setRadiusScaleCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(MaxSurfelRadius,
                      std::bind(&PLodNode::getMaxSurfelRadiusCB, this,std::placeholders::_1),
                      std::bind(&PLodNode::setMaxSurfelRadiusCB, this,std::placeholders::_1));
 
  AV_FC_ADD_ADAPTOR_FIELD(ErrorThreshold,
                      std::bind(&PLodNode::getErrorThresholdCB, this,std::placeholders::_1),
                      std::bind(&PLodNode::setErrorThresholdCB, this,std::placeholders::_1));
  
  AV_FC_ADD_ADAPTOR_FIELD(EnableBackfaceCulling,
                      std::bind(&PLodNode::getEnableBackfaceCullingCB, this,std::placeholders::_1),
                      std::bind(&PLodNode::setEnableBackfaceCullingCB, this,std::placeholders::_1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }

}

void
av::gua::lod::PLodNode::on_distribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_distribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_distribute(netNode);
  }
  netNode.distributeFieldContainer(m_Material);
}

void
av::gua::lod::PLodNode::on_undistribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_undistribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_Material);
}

void
av::gua::lod::PLodNode::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::lod::PLodNode, true);

        SFPLodNode::initClass("av::gua::lod::SFPLodNode", "av::Field");
        MFPLodNode::initClass("av::gua::lod::MFPLodNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void
av::gua::lod::PLodNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLodNode->get_geometry_description();
}

void
av::gua::lod::PLodNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaPLodNode->set_geometry_description(event.getValue());
}

void
av::gua::lod::PLodNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::lod::PLodNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaPLodNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::lod::PLodNode::getRadiusScaleCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLodNode->get_radius_scale();
}

void
av::gua::lod::PLodNode::setRadiusScaleCB(const SFFloat::SetValueEvent& event)
{
  m_guaPLodNode->set_radius_scale(event.getValue());
}

void
av::gua::lod::PLodNode::getMaxSurfelRadiusCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLodNode->get_max_surfel_radius();
}

void
av::gua::lod::PLodNode::setMaxSurfelRadiusCB(const SFFloat::SetValueEvent& event)
{
  m_guaPLodNode->set_max_surfel_radius(event.getValue());
}


void
av::gua::lod::PLodNode::getErrorThresholdCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLodNode->get_error_threshold();
}

void
av::gua::lod::PLodNode::setErrorThresholdCB(const SFFloat::SetValueEvent& event)
{
  m_guaPLodNode->set_error_threshold(event.getValue());
}

void
av::gua::lod::PLodNode::getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLodNode->get_enable_backface_culling_by_normal();
}

void
av::gua::lod::PLodNode::setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaPLodNode->set_enable_backface_culling_by_normal(event.getValue());
}


std::shared_ptr< ::gua::node::PLodNode>
av::gua::lod::PLodNode::getGuaPLodNode() const {
  return m_guaPLodNode;
}

