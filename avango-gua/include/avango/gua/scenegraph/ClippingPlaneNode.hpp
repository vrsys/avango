#ifndef AVANGO_GUA_CLIPPING_PLANE_NODE_HPP
#define AVANGO_GUA_CLIPPING_PLANE_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/ClippingPlaneNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::ClippingPlaneNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL ClippingPlaneNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::ClippingPlaneNode is created.
       * Otherwise, the given ::gua::ClippingPlaneNode is used.
       */
      ClippingPlaneNode(std::shared_ptr< ::gua::node::ClippingPlaneNode> guanode = std::shared_ptr< ::gua::node::ClippingPlaneNode>(new ::gua::node::ClippingPlaneNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ClippingPlaneNode();

    public:

      MFInt ViewIDs;

      /**
       * Get the wrapped ::gua::ClippingPlaneNode.
       */
      std::shared_ptr< ::gua::node::ClippingPlaneNode> getGuaNode() const;

    public:
      virtual void getViewIDsCB(const MFInt::GetValueEvent& event);
      virtual void setViewIDsCB(const MFInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::node::ClippingPlaneNode> m_guaNode;

      ClippingPlaneNode(const ClippingPlaneNode&);
      ClippingPlaneNode& operator=(const ClippingPlaneNode&);
    };

    using SFClippingPlaneNode = SingleField<Link<ClippingPlaneNode> >;
    using MFClippingPlaneNode = MultiField<Link<ClippingPlaneNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::ClippingPlaneNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::ClippingPlaneNode> >;
#endif

}

#endif //AVANGO_GUA_CLIPPING_PLANE_NODE_HPP
