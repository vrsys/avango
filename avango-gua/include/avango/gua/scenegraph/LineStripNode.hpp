#ifndef AVANGO_GUA_LINE_STRIP_NODE_HPP
#define AVANGO_GUA_LINE_STRIP_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/LineStripNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::LineStripNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LineStripNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::LineStripNode is created.
       * Otherwise, the given ::gua::LineStripNode is used.
       */
      LineStripNode(std::shared_ptr< ::gua::node::LineStripNode> guanode =
          std::shared_ptr< ::gua::node::LineStripNode>(new ::gua::node::LineStripNode("")));
      
      void clearVertices() const;

      void popBackVertex() const;

      void popFrontVertex() const;

      void pushVertex(float x, float y, float z,
                      float col_r = 0.0f, float col_g = 0.0f, float col_b = 0.0f, float col_a = 1.0f,
                      float thickness = 1.0f) const;

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);
#endif
    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~LineStripNode();

    public:

      SFString          Geometry;
      SFMaterial        Material;
      SFBool            RenderToGBuffer;
      SFBool            RenderToStencilBuffer;
      SFBool            RenderVolumetric;
      SFBool            RenderAsPoints;
      SFFloat           ScreenSpaceLineWidth;
      SFBool            WasCreatedEmpty;

      SFBool            TriggerUpdate;
      SFString          PrivateLineStripData;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      virtual void getRenderToGBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToGBufferCB(const SFBool::SetValueEvent& event);

      virtual void getRenderToStencilBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToStencilBufferCB(const SFBool::SetValueEvent& event);

      virtual void getRenderVolumetricCB(const SFBool::GetValueEvent& event);
      virtual void setRenderVolumetricCB(const SFBool::SetValueEvent& event);

      virtual void getRenderAsPointsCB(const SFBool::GetValueEvent& event);
      virtual void setRenderAsPointsCB(const SFBool::SetValueEvent& event);

      virtual void getScreenSpaceLineWidthCB(const SFFloat::GetValueEvent& event);
      virtual void setScreenSpaceLineWidthCB(const SFFloat::SetValueEvent& event);

      virtual void getWasCreatedEmptyCB(const SFBool::GetValueEvent& event);
      virtual void setWasCreatedEmptyCB(const SFBool::SetValueEvent& event);

      virtual void getTriggerUpdateCB(const SFBool::GetValueEvent& event);
      virtual void setTriggerUpdateCB(const SFBool::SetValueEvent& event);

      virtual void getPrivateLineStripDataCB(const SFString::GetValueEvent& event);
      virtual void setPrivateLineStripDataCB(const SFString::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::LineStripNode.
       */
      std::shared_ptr< ::gua::node::LineStripNode> getGuaLineStripNode() const;

    private:

      std::string privateLineStripData = "";

      //0 = server, 1 = client, 2 = unidentified
      int32_t role_server_client_unidentified = 2;
      std::shared_ptr< ::gua::node::LineStripNode> m_guaLineStripNode;
      av::Link< av::gua::Material> m_Material;

      LineStripNode(const LineStripNode&);
      LineStripNode& operator=(const LineStripNode&);
    };

    using SFLineStripNode = SingleField<Link<LineStripNode> >;
    using MFLineStripNode = MultiField<Link<LineStripNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LineStripNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::LineStripNode> >;
#endif

}

#endif //AVANGO_GUA_LINE_STRIP_NODE_HPP
