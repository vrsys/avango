#ifndef AVANGO_GUA_SCREEN_NODE_HPP
#define AVANGO_GUA_SCREEN_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/ScreenNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::ScreenNode
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL ScreenNode : public av::gua::Node
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::ScreenNode is created.
     * Otherwise, the given ::gua::ScreenNode is used.
     */
    ScreenNode(std::shared_ptr<::gua::node::ScreenNode> guanode = std::shared_ptr<::gua::node::ScreenNode>(new ::gua::node::ScreenNode("")));

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ScreenNode();

  public:
    /**
     * Children field is read only!
     */
    SFFloat Width;
    SFFloat Height;

    SFMatrix ScaledWorldTransform;

    /**
     * Get the wrapped ::gua::ScreenNode.
     */
    std::shared_ptr<::gua::node::ScreenNode> getGuaNode() const;

  public:
    virtual void getWidthCB(const SFFloat::GetValueEvent& event);
    virtual void setWidthCB(const SFFloat::SetValueEvent& event);

    virtual void getHeightCB(const SFFloat::GetValueEvent& event);
    virtual void setHeightCB(const SFFloat::SetValueEvent& event);

    virtual void getScaledWorldTransformCB(const SFMatrix::GetValueEvent& event);
    virtual void setScaledWorldTransformCB(const SFMatrix::SetValueEvent& event);

  private:
    std::shared_ptr<::gua::node::ScreenNode> m_guaNode;

    ScreenNode(const ScreenNode&);
    ScreenNode& operator=(const ScreenNode&);
};

using SFScreenNode = SingleField<Link<ScreenNode>>;
using MFScreenNode = MultiField<Link<ScreenNode>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::ScreenNode>>;
template class AV_GUA_DLL MultiField<Link<gua::ScreenNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_SCREEN_NODE_HPP
