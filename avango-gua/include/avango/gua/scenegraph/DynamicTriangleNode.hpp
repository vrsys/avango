#ifndef AVANGO_GUA_DYNAMIC_TRIANGLE_NODE_HPP
#define AVANGO_GUA_DYNAMIC_TRIANGLE_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/DynamicTriangleNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::DynamicTriangleNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL DynamicTriangleNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::DynamicTriangleNode is created.
       * Otherwise, the given ::gua::DynamicTriangleNode is used.
       */
      DynamicTriangleNode(std::shared_ptr< ::gua::node::DynamicTriangleNode> guanode =
          std::shared_ptr< ::gua::node::DynamicTriangleNode>(new ::gua::node::DynamicTriangleNode("")));
      
      void clearVertices() const;

      void enqueueVertex(float pos_x, float pos_y, float pos_z,
                         float col_r = 0.0f, float col_g = 0.0f, float col_b = 0.0f, float col_a = 1.0f,
                         float thickness = 1.0f) const;

      void popBackVertex() const;

      void popFrontVertex() const;

      void pushVertex(float pos_x, float pos_y, float pos_z,
                      float col_r = 0.0f, float col_g = 0.0f, float col_b = 0.0f, float col_a = 1.0f,
                      float thickness = 1.0f,
                      float u = 0.0f, float v = 0.0f) const;

      void updateVertex(int vertex_id, float pos_x, float pos_y, float pos_z,
                      float col_r = 0.0f, float col_g = 0.0f, float col_b = 0.0f, float col_a = 1.0f,
                      float thickness = 1.0f, 
                      float u = 0.0f, float v = 0.0f) const;

      void fillWithParabola(float a, float b, float c,
                float sample_distance, float maximal_distance,
                float col_r, float col_g, float col_b);

      void fillWithBezier(::gua::math::vec3 const& p0, 
                        ::gua::math::vec3 const& p1, 
                        ::gua::math::vec3 const& p2,
                        float col_r, float col_g, float col_b,
                        int num_segments);

      //void submitVertices();

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);
#endif
    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~DynamicTriangleNode();

    public:

      SFString          Geometry;
      SFMaterial        Material;
      SFBool            RenderToGBuffer;
      SFBool            RenderToStencilBuffer;
      SFBool            RenderVolumetric;
      SFBool            RenderAsPoints;
      SFFloat           ScreenSpaceLineWidth;
      SFBool            WasCreatedEmpty;
      SFBool            UseParabolaShape;
      MFFloat           ParabolaParameters;
      SFBool            UseBezierShape;
      MFFloat           BezierParameters;

      SFBool            TriggerUpdate;


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

      virtual void getParabolaParametersCB(const MFFloat::GetValueEvent& event);
      virtual void setParabolaParametersCB(const MFFloat::SetValueEvent& event);

      virtual void getBezierParametersCB(const MFFloat::GetValueEvent& event);
      virtual void setBezierParametersCB(const MFFloat::SetValueEvent& event);

      virtual void getTriggerUpdateCB(const SFBool::GetValueEvent& event);
      virtual void setTriggerUpdateCB(const SFBool::SetValueEvent& event);



      /**
       * Get the wrapped ::gua::DynamicTriangleNode.
       */
      std::shared_ptr< ::gua::node::DynamicTriangleNode> getGuaDynamicTriangleNode() const;

      /**
       * Wrapper to clear the vertices of the server side node in a clean way
         from the application side
       */
      void startVertexList();
      /**
       * Wrapper to trigger the setPrivateLineStripDataStringCB in a clean way 
         from the application side
       */
      void endVertexList();

    private:
      /* the "PrivateLineStripDataString" should not be set by a user! it is only used
       * as mechanism to distribute the dynamical vertex data encoded as a string containing
       * the original binary data
       */

      SFString          PrivateLineStripDataString;
      virtual void getPrivateLineStripDataStringCB(const SFString::GetValueEvent& event);
      virtual void setPrivateLineStripDataStringCB(const SFString::SetValueEvent& event);

      std::string privateLineStripData = "";

      //0 = server, 1 = client, 2 = unidentified
      int32_t role_server_client_unidentified = 2;
      std::shared_ptr< ::gua::node::DynamicTriangleNode> m_guaDynamicTriangleNode;
      av::Link< av::gua::Material> m_Material;

      DynamicTriangleNode(const DynamicTriangleNode&);
      DynamicTriangleNode& operator=(const DynamicTriangleNode&);
    };

    using SFDynamicTriangleNode = SingleField<Link<DynamicTriangleNode> >;
    using MFDynamicTriangleNode = MultiField<Link<DynamicTriangleNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DynamicTriangleNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::DynamicTriangleNode> >;
#endif

}

#endif //AVANGO_GUA_DYNAMIC_TRIANGLE_NODE_HPP
