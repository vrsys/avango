#ifndef AVANGO_GUA_SUN_LIGHT_NODE_HPP
#define AVANGO_GUA_SUN_LIGHT_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/SunLightNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SunLightNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SunLightNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SunLightNode is created.
       * Otherwise, the given ::gua::SunLightNode is used.
       */
      SunLightNode(std::shared_ptr< ::gua::node::SunLightNode> guanode = std::shared_ptr< ::gua::node::SunLightNode>(new ::gua::node::SunLightNode("")));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SunLightNode();

    public:

      SFColor Color;
      SFBool  EnableShadows;
      SFBool  EnableGodrays;
      SFBool  EnableDiffuseShading;
      SFBool  EnableSpecularShading;
      SFFloat ShadowOffset;
      SFUInt  ShadowMapSize;
      MFFloat ShadowCascadedSplits;
      SFFloat ShadowNearClippingInSunDirection;

      /**
       * Get the wrapped ::gua::SunLightNode.
       */
      std::shared_ptr< ::gua::node::SunLightNode> getGuaNode() const;

    public:

      virtual void getColorCB(const SFColor::GetValueEvent& event);
      virtual void setColorCB(const SFColor::SetValueEvent& event);

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


    private:

      std::shared_ptr< ::gua::node::SunLightNode> m_guaNode;

      SunLightNode(const SunLightNode&);
      SunLightNode& operator=(const SunLightNode&);
    };

    typedef SingleField<Link<SunLightNode> > SFSunLightNode;
    typedef MultiField<Link<SunLightNode> > MFSunLightNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SunLightNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SunLightNode> >;
#endif

}

#endif //AVANGO_GUA_SUN_LIGHT_NODE_HPP
