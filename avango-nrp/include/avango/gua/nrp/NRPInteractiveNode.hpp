#ifndef AVANGO_GUA_NRP_INTERACTIVE_NODE_HPP
#define AVANGO_GUA_NRP_INTERACTIVE_NODE_HPP

#include <gua/nrp/nrp_interactive_node.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>

namespace av
{
namespace gua
{
namespace nrp
{
class AV_GUA_DLL NRPInteractiveNode : public av::gua::TransformNode
{
    AV_FC_DECLARE();

  public:
    NRPInteractiveNode(::gua::nrp::NRPInteractiveNode *guanode = new ::gua::nrp::NRPInteractiveNode(""));

  private:
    ::gua::nrp::NRPInteractiveNode *_nrp_node_ptr;
};

using SFNRPInteractiveNode = av::SingleField<av::Link<av::gua::nrp::NRPInteractiveNode>>;
using MFNRPInteractiveNode = av::MultiField<av::Link<av::gua::nrp::NRPInteractiveNode>>;
} // namespace nrp
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL av::SingleField<av::Link<gua::nrp::NRPInteractiveNode>>;
template class AV_GUA_DLL av::MultiField<av::Link<gua::nrp::NRPInteractiveNode>>;
#endif
} // namespace av

#endif // AVANGO_GUA_NRP_INTERACTIVE_NODE_HPP
