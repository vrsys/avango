#include <avango/Base.h>
#include <avango/gua/nrp/NRPNode.hpp>
#include <functional>

AV_FC_DEFINE(av::gua::nrp::NRPNode);

AV_FIELD_DEFINE(av::gua::nrp::SFNRPNode);
AV_FIELD_DEFINE(av::gua::nrp::MFNRPNode);

av::gua::nrp::NRPNode::NRPNode(::gua::nrp::NRPNode* guanode) : TransformNode(std::shared_ptr< ::gua::nrp::NRPNode>(guanode)){
    _nrp_node_ptr = guanode;
}

void av::gua::nrp::NRPNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::nrp::NRPNode, true);

        SFNRPNode::initClass("av::gua::nrp::SFNRPNode", "av::Field");
        MFNRPNode::initClass("av::gua::nrp::MFNRPNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
