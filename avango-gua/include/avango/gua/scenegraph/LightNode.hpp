#ifndef AVANGO_GUA_LIGHT_NODE_HPP
#define AVANGO_GUA_LIGHT_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/LightNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::LightNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LightNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      enum LightTypeEnum {
        POINT = static_cast<unsigned>(::gua::node::LightNode::Type::POINT),
        SPOT = static_cast<unsigned>(::gua::node::LightNode::Type::SPOT),
        SUN = static_cast<unsigned>(::gua::node::LightNode::Type::SUN)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::LightNode is created.
       * Otherwise, the given ::gua::LightNode is used.
       */
      LightNode(std::shared_ptr< ::gua::node::LightNode> guanode = std::shared_ptr< ::gua::node::LightNode>(new ::gua::node::LightNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~LightNode() {}

    public:

      SFUInt  Type;
      SFColor Color;
      SFFloat Brightness;
      SFFloat Falloff;
      SFFloat Softness;
      SFBool  EnableShadows;
      SFBool  EnableGodrays;
      SFBool  EnableDiffuseShading;
      SFBool  EnableSpecularShading;
      SFFloat ShadowOffset;
      SFUInt  ShadowMapSize;

      // new
      MFFloat ShadowCascadedSplits;
      SFFloat ShadowNearClippingInSunDirection;
      SFFloat ShadowFarClippingInSunDirection;

      /**
       * Get the wrapped ::gua::LightNode.
       */
      std::shared_ptr< ::gua::node::LightNode> getGuaNode() const;

    public:

      virtual void getTypeCB(const SFUInt::GetValueEvent& event);
      virtual void setTypeCB(const SFUInt::SetValueEvent& event);

      virtual void getColorCB(const SFColor::GetValueEvent& event);
      virtual void setColorCB(const SFColor::SetValueEvent& event);

      virtual void getBrightnessCB(const SFFloat::GetValueEvent& event);
      virtual void setBrightnessCB(const SFFloat::SetValueEvent& event);

      virtual void getFallofCB(const SFFloat::GetValueEvent& event);
      virtual void setFallofCB(const SFFloat::SetValueEvent& event);

      virtual void getSoftnessCB(const SFFloat::GetValueEvent& event);
      virtual void setSoftnessCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableShadowsCB(const SFBool::GetValueEvent& event);
      virtual void setEnableShadowsCB(const SFBool::SetValueEvent& event);

      virtual void getEnableGodraysCB(const SFBool::GetValueEvent& event);
      virtual void setEnableGodraysCB(const SFBool::SetValueEvent& event);

      virtual void getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event);
      virtual void setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event);

      virtual void getEnableSpecularShadingCB(const SFBool::GetValueEvent& event);
      virtual void setEnableSpecularShadingCB(const SFBool::SetValueEvent& event);

      virtual void getShadowOffsetCB(const SFFloat::GetValueEvent& event);
      virtual void setShadowOffsetCB(const SFFloat::SetValueEvent& event);

      virtual void getShadowMapSizeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowMapSizeCB(const SFUInt::SetValueEvent& event);
      
      virtual void getShadowCascadedSplitsCB(const MFFloat::GetValueEvent& event);
      virtual void setShadowCascadedSplitsCB(const MFFloat::SetValueEvent& event);

      virtual void getShadowNearClippingInSunDirectionCB(const SFFloat::GetValueEvent& event);
      virtual void setShadowNearClippingInSunDirectionCB(const SFFloat::SetValueEvent& event);

      virtual void getShadowFarClippingInSunDirectionCB(const SFFloat::GetValueEvent& event);
      virtual void setShadowFarClippingInSunDirectionCB(const SFFloat::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::node::LightNode> m_guaNode;

      LightNode(const LightNode&);
      LightNode& operator=(const LightNode&);
    };

    typedef SingleField<Link<LightNode> > SFLightNode;
    typedef MultiField<Link<LightNode> > MFLightNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LightNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::LightNode> >;
#endif

}

#endif //AVANGO_GUA_LIGHT_NODE_HPP
