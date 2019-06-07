#include <avango/gua/lod/scenegraph/MLodNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::lod::MLodNode);

AV_FIELD_DEFINE(av::gua::lod::SFMLodNode);
AV_FIELD_DEFINE(av::gua::lod::MFMLodNode);

av::gua::lod::MLodNode::MLodNode(std::shared_ptr<::gua::node::MLodNode> guanode) : GeometryNode(guanode), m_guaMLodNode(std::dynamic_pointer_cast<::gua::node::MLodNode>(GeometryNode::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Geometry, std::bind(&MLodNode::getGeometryCB, this, std::placeholders::_1), std::bind(&MLodNode::setGeometryCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Material, std::bind(&MLodNode::getMaterialCB, this, std::placeholders::_1), std::bind(&MLodNode::setMaterialCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ErrorThreshold, std::bind(&MLodNode::getErrorThresholdCB, this, std::placeholders::_1), std::bind(&MLodNode::setErrorThresholdCB, this, std::placeholders::_1));

    if(guanode->get_material())
    {
        m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
    }
}
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::lod::MLodNode::on_distribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_distribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::lod::MLodNode::on_undistribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_undistribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}
#endif

void av::gua::lod::MLodNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::lod::MLodNode, true);

        SFMLodNode::initClass("av::gua::lod::SFMLodNode", "av::Field");
        MFMLodNode::initClass("av::gua::lod::MFMLodNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::lod::MLodNode::getGeometryCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaMLodNode->get_geometry_description(); }

void av::gua::lod::MLodNode::setGeometryCB(const SFString::SetValueEvent& event) { m_guaMLodNode->set_geometry_description(event.getValue()); }

void av::gua::lod::MLodNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
    if(m_Material.isValid())
    {
        *(event.getValuePtr()) = m_Material;
    }
}

void av::gua::lod::MLodNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
    if(event.getValue().isValid())
    {
        m_Material = event.getValue();
        m_guaMLodNode->set_material(m_Material->getGuaMaterial());
    }
}

void av::gua::lod::MLodNode::getErrorThresholdCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaMLodNode->get_error_threshold(); }

void av::gua::lod::MLodNode::setErrorThresholdCB(const SFFloat::SetValueEvent& event) { m_guaMLodNode->set_error_threshold(event.getValue()); }

std::shared_ptr<::gua::node::MLodNode> av::gua::lod::MLodNode::getGuaMLodNode() const { return m_guaMLodNode; }
