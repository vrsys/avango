#ifndef AVANGO_GUA_DEPTH_MAP_NODE_HPP
#define AVANGO_GUA_DEPTH_MAP_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/CubemapNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::DepthMapNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL DepthMapNode : public  av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::DepthMapNode is created.
       * Otherwise, the given ::gua::DepthMapNode is used.
       */
      DepthMapNode(std::shared_ptr< ::gua::node::CubemapNode> guanode =
          std::shared_ptr< ::gua::node::CubemapNode>(new ::gua::node::CubemapNode("")));

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~DepthMapNode();

    public:
      SFBool Active;

      SFFloat MinDistance;
      SFVec3 MinDistanceWorldPosition;
      SFFloat WeightedMinDistance;

      SFString TextureName;

      SFFloat NearClip;
      SFFloat FarClip;

      SFInt Resolution;

      MFString WhiteList;
      MFString BlackList;

      SFInt ViewID;

      void create_weights(::gua::math::vec3 const& view_direction, ::gua::math::vec3 const& move_direction);
      ::gua::math::vec3 get_push_back(float radius, float softness);

      virtual void getActiveCB(const SFBool::GetValueEvent& event);
      virtual void setActiveCB(const SFBool::SetValueEvent& event);

      virtual void getMinDistanceCB(const SFFloat::GetValueEvent& event);
      virtual void setMinDistanceCB(const SFFloat::SetValueEvent& event);

      virtual void getMinDistanceWorldPositionCB(const SFVec3::GetValueEvent& event);
      virtual void setMinDistanceWorldPositionCB(const SFVec3::SetValueEvent& event);

      virtual void getWeightedMinDistanceCB(const SFFloat::GetValueEvent& event);
      virtual void setWeightedMinDistanceCB(const SFFloat::SetValueEvent& event);

      virtual void getTextureNameCB(const SFString::GetValueEvent& event);
      virtual void setTextureNameCB(const SFString::SetValueEvent& event);

      virtual void getNearClipCB(const SFFloat::GetValueEvent& event);
      virtual void setNearClipCB(const SFFloat::SetValueEvent& event);

      virtual void getFarClipCB(const SFFloat::GetValueEvent& event);
      virtual void setFarClipCB(const SFFloat::SetValueEvent& event);

      virtual void getResolutionCB(const SFInt::GetValueEvent& event);
      virtual void setResolutionCB(const SFInt::SetValueEvent& event);

      virtual void getWhiteListCB(const MFString::GetValueEvent& event);
      virtual void setWhiteListCB(const MFString::SetValueEvent& event);

      virtual void getBlackListCB(const MFString::GetValueEvent& event);
      virtual void setBlackListCB(const MFString::SetValueEvent& event);

      virtual void getViewIDCB(const SFInt::GetValueEvent& event);
      virtual void setViewIDCB(const SFInt::SetValueEvent& event);
      /**
       * Get the wrapped ::gua::DepthMapNode.
       */
      std::shared_ptr< ::gua::node::CubemapNode> getGuaDepthMapNode() const;

    private:

      std::shared_ptr< ::gua::node::CubemapNode> m_guaDepthMapNode;

      DepthMapNode(const DepthMapNode&);
      DepthMapNode& operator=(const DepthMapNode&);
    };

    using SFDepthMapNode = SingleField<Link<DepthMapNode> >;
    using MFDepthMapNode = MultiField<Link<DepthMapNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DepthMapNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::DepthMapNode> >;
#endif

}

#endif //AVANGO_GUA_DEPTH_MAP_NODE_HPP
