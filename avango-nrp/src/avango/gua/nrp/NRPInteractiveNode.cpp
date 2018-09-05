#include <avango/Base.h>
#include <avango/gua/nrp/NRPInteractiveNode.hpp>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::nrp::NRPInteractiveNode);

AV_FIELD_DEFINE(av::gua::nrp::SFNRPInteractiveNode);
AV_FIELD_DEFINE(av::gua::nrp::MFNRPInteractiveNode);

av::gua::nrp::NRPInteractiveNode::NRPInteractiveNode(::gua::nrp::NRPInteractiveNode *guanode) : TransformNode(std::shared_ptr<::gua::nrp::NRPInteractiveNode>(guanode)) { _nrp_node_ptr = guanode; }

void av::gua::nrp::NRPInteractiveNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::nrp::NRPInteractiveNode, true);

        av::gua::nrp::SFNRPInteractiveNode::initClass("av::gua::nrp::SFNRPInteractiveNode", "av::Field");
        av::gua::nrp::MFNRPInteractiveNode::initClass("av::gua::nrp::MFNRPInteractiveNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
