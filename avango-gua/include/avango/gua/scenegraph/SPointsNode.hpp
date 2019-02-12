#ifndef AVANGO_GUA_SPOINTS_NODE_HPP
#define AVANGO_GUA_SPOINTS_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */
#include <gua/spoints/spoints_geometry/NetKinectArray.hpp>
#include <gua/spoints/SPointsNode.hpp>

#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
  namespace gua
  {


    struct AV_GUA_DLL SPointsStats {

      SPointsStats() 
        : m_num_received_triangles(0),
          m_timestamp_ms(-1.0f),
          m_reconstruction_time_ms(0.0f),
          m_request_reply_latency_ms(-1.0f),
          m_total_message_payload_in_byte(0)
      {}

      explicit SPointsStats(spoints::SPointsStats const& gua_spoints_stats) 
        : m_num_received_triangles(gua_spoints_stats.m_received_triangles),
          m_timestamp_ms(gua_spoints_stats.m_received_timestamp_ms),
          m_reconstruction_time_ms(gua_spoints_stats.m_received_reconstruction_time_ms),
          m_request_reply_latency_ms(gua_spoints_stats.m_request_reply_latency_ms),
          m_total_message_payload_in_byte(gua_spoints_stats.m_total_message_payload_in_byte)
      {}

      uint32_t m_num_received_triangles = 0;
      float    m_timestamp_ms = 0.0f;
      float    m_reconstruction_time_ms = -1.0f;
      float    m_request_reply_latency_ms = -1.0f;
      uint32_t m_total_message_payload_in_byte = 0;
    };

    /**
     * Wrapper for ::gua::SPointsNode
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SPointsNode : public av::gua::GeometryNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SPointsNode
       * is created.
       * Otherwise, the given ::gua::SPointsNode is used.
       */
      SPointsNode(std::shared_ptr< ::gua::node::SPointsNode> guanode =
          std::shared_ptr< ::gua::node::SPointsNode>(new ::gua::node::SPointsNode("")));

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);


    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~SPointsNode();

    public:

      SFString   Geometry;
      SFMaterial Material;
      SFBool     RenderToGBuffer;
      SFBool     RenderToStencilBuffer;

      SPointsStats LatestSPointsStats;

      virtual void getGeometryCB(const SFString::GetValueEvent& event);
      virtual void setGeometryCB(const SFString::SetValueEvent& event);

      virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);
      virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);

      virtual void getRenderToGBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToGBufferCB(const SFBool::SetValueEvent& event);

      virtual void getRenderToStencilBufferCB(const SFBool::GetValueEvent& event);
      virtual void setRenderToStencilBufferCB(const SFBool::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::SPointsNode.
       */
      std::shared_ptr< ::gua::node::SPointsNode> getGuaSPointsNode() const;

      void fetchLatestSPointsStats() {
        if(m_guaSPointsNode) {
          LatestSPointsStats = SPointsStats(m_guaSPointsNode->get_latest_spoints_stats());
        } else {
          LatestSPointsStats = SPointsStats();
        }
      };

      int getStatNumTrianglesReceived() const { return LatestSPointsStats.m_num_received_triangles;}
      float getStatTimestampReceived() const { return LatestSPointsStats.m_timestamp_ms;}
      float getStatReconTimeReceived() const { return LatestSPointsStats.m_reconstruction_time_ms;}
      float getStatRequestReplyLatencyReceived() const { return LatestSPointsStats.m_request_reply_latency_ms;}
      int getStatTotaMessagePayloadInByteReceived() const { return LatestSPointsStats.m_total_message_payload_in_byte;}

    private:

      std::shared_ptr< ::gua::node::SPointsNode> m_guaSPointsNode;
      av::Link< av::gua::Material> m_Material;

      SPointsNode(const SPointsNode&);
      SPointsNode& operator=(const SPointsNode&);
    };

    using SFSPointsNode = SingleField<Link<SPointsNode> >;
    using MFSPointsNode = MultiField<Link<SPointsNode> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SPointsNode> >;
  template class AV_GUA_DLL MultiField<Link<gua::SPointsNode> >;
#endif

}

#endif //AVANGO_GUA_SPOINTS_NODE_HPP
