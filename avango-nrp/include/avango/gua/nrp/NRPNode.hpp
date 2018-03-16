#ifndef AVANGO_GUA_NRP_NODE_HPP
#define AVANGO_GUA_NRP_NODE_HPP

#include <gua/math/math.hpp>
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
    NRPNode(std::shared_ptr<::gua::nrp::NRPNode> guanode = std::shared_ptr<::gua::nrp::NRPNode>(new ::gua::nrp::NRPNode("")));

  private:
    std::shared_ptr<::gua::nrp::NRPNode> _nrp_node_ptr;
};

using SFNRPNode = SingleField<Link<NRPNode>>;
using MFNRPNode = MultiField<Link<NRPNode>>;
}
}

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::NRPNode>>;
template class AV_GUA_DLL MultiField<Link<gua::NRPNode>>;
#endif
}

#endif // AVANGO_GUA_NRP_NODE_HPP
