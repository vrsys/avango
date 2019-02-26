#ifndef AVANGO_GUA_NRP_CAM_NODE_HPP
#define AVANGO_GUA_NRP_CAM_NODE_HPP

#include <gua/nrp/nrp_cam_node.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>

namespace av
{
namespace gua
{
namespace nrp
{
class AV_GUA_DLL NRPCameraNode : public av::gua::CameraNode
{
    AV_FC_DECLARE();

  public:
    NRPCameraNode(::gua::nrp::NRPCameraNode *guanode = new ::gua::nrp::NRPCameraNode(""));

  private:
    ::gua::nrp::NRPCameraNode *_nrp_node_ptr;
};

using SFNRPCameraNode = av::SingleField<av::Link<av::gua::nrp::NRPCameraNode>>;
using MFNRPCameraNode = av::MultiField<av::Link<av::gua::nrp::NRPCameraNode>>;
} // namespace nrp
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPCameraNode>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPCameraNode>>;
#endif
} // namespace av

#endif // AVANGO_GUA_NRP_CAM_NODE_HPP
