#include <avango/Base.h>
#include <avango/gua/nrp/NRPCameraNode.hpp>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::nrp::NRPCameraNode);

AV_FIELD_DEFINE(av::gua::nrp::SFNRPCameraNode);
AV_FIELD_DEFINE(av::gua::nrp::MFNRPCameraNode);

av::gua::nrp::NRPCameraNode::NRPCameraNode(::gua::nrp::NRPCameraNode *guanode) : CameraNode(std::shared_ptr<::gua::nrp::NRPCameraNode>(guanode)) { _nrp_node_ptr = guanode; }

void av::gua::nrp::NRPCameraNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::CameraNode::initClass();

        AV_FC_INIT(av::gua::CameraNode, av::gua::nrp::NRPCameraNode, true);

        av::gua::nrp::SFNRPCameraNode::initClass("av::gua::nrp::SFNRPCameraNode", "av::Field");
        av::gua::nrp::MFNRPCameraNode::initClass("av::gua::nrp::MFNRPCameraNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}
