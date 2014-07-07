#ifndef AVANGO_GUA_SPOT_LIGHT_NODE_HPP
#define AVANGO_GUA_SPOT_LIGHT_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/SpotLightNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SpotLightNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SpotLightNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SpotLightNode is created.
       * Otherwise, the given ::gua::SpotLightNode is used.
       */
      SpotLightNode(std::shared_ptr< ::gua::node::SpotLightNode> guanode = std::shared_ptr< ::gua::node::SpotLightNode>(new ::gua::node::SpotLightNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SpotLightNode();

    public:

      SFColor Color;
      SFFloat Falloff;
      SFFloat Softness;
      SFBool  EnableShadows;
      SFBool  EnableGodrays;
      SFBool  EnableDiffuseShading;
      SFBool  EnableSpecularShading;
      SFFloat ShadowOffset;
      SFUInt  ShadowMapSize;

      /**
       * Get the wrapped ::gua::SpotLightNode.
       */
      std::shared_ptr< ::gua::node::SpotLightNode> getGuaNode() const;

    public:

      virtual void getColorCB(const SFColor::GetValueEvent& event);
      virtual void setColorCB(const SFColor::SetValueEvent& event);

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

    private:

      std::shared_ptr< ::gua::node::SpotLightNode> m_guaNode;

      SpotLightNode(const SpotLightNode&);
      SpotLightNode& operator=(const SpotLightNode&);
    };

    typedef SingleField<Link<SpotLightNode> > SFSpotLightNode;
    typedef MultiField<Link<SpotLightNode> > MFSpotLightNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SpotLightNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SpotLightNode> >;
#endif

}

#endif //AVANGO_GUA_SPOT_LIGHT_NODE_HPP
