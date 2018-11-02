#ifndef AVANGO_GUA_NRP_NODE_HPP
#define AVANGO_GUA_NRP_NODE_HPP

#include <gua/nrp/nrp_node.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>

namespace av
{
namespace gua
{
namespace nrp
{
class AV_GUA_DLL NRPNode : public av::gua::TransformNode
{
    AV_FC_DECLARE();

  public:
    NRPNode(::gua::nrp::NRPNode *guanode = new ::gua::nrp::NRPNode(""));

  private:
    ::gua::nrp::NRPNode *_nrp_node_ptr;
};

using SFNRPNode = av::SingleField<av::Link<av::gua::nrp::NRPNode>>;
using MFNRPNode = av::MultiField<av::Link<av::gua::nrp::NRPNode>>;
}
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPNode>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPNode>>;
#endif
}

#endif // AVANGO_GUA_NRP_NODE_HPP
