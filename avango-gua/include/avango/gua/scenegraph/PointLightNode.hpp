#ifndef AVANGO_GUA_POINT_LIGHT_NODE_HPP
#define AVANGO_GUA_POINT_LIGHT_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/PointLightNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PointLightNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PointLightNode : public av::gua::Node
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PointLightNode is created.
       * Otherwise, the given ::gua::PointLightNode is used.
       */
      PointLightNode(std::shared_ptr< ::gua::PointLightNode> guanode =
                     std::make_shared< ::gua::PointLightNode>(""));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PointLightNode();

    public:

      SFColor Color;
      SFFloat Falloff;
      SFBool EnableShadows;
      SFBool EnableGodrays;
      SFBool EnableDiffuseShading;
      SFBool EnableSpecularShading;
      SFUInt ShadowMapSize;

      /**
       * Get the wrapped ::gua::PointLightNode.
       */
      std::shared_ptr< ::gua::PointLightNode> getGuaNode() const;

    public:

      virtual void getColorCB(const SFColor::GetValueEvent& event);
      virtual void setColorCB(const SFColor::SetValueEvent& event);

      virtual void getFallofCB(const SFFloat::GetValueEvent& event);
      virtual void setFallofCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableShadowsCB(const SFBool::GetValueEvent& event);
      virtual void setEnableShadowsCB(const SFBool::SetValueEvent& event);

      virtual void getEnableGodraysCB(const SFBool::GetValueEvent& event);
      virtual void setEnableGodraysCB(const SFBool::SetValueEvent& event);

      virtual void getEnableDiffuseShadingCB(const SFBool::GetValueEvent& event);
      virtual void setEnableDiffuseShadingCB(const SFBool::SetValueEvent& event);

      virtual void getEnableSpecularShadingCB(const SFBool::GetValueEvent& event);
      virtual void setEnableSpecularShadingCB(const SFBool::SetValueEvent& event);

      virtual void getShadowMapSizeCB(const SFUInt::GetValueEvent& event);
      virtual void setShadowMapSizeCB(const SFUInt::SetValueEvent& event);

    private:

      std::shared_ptr< ::gua::PointLightNode> m_guaNode;

      PointLightNode(const PointLightNode&);
      PointLightNode& operator=(const PointLightNode&);
    };

    typedef SingleField<Link<PointLightNode> > SFPointLightNode;
    typedef MultiField<Link<PointLightNode> > MFPointLightNode;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PointLightNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::PointLightNode> >;
#endif

}

#endif //AVANGO_GUA_POINT_LIGHT_NODE_HPP
