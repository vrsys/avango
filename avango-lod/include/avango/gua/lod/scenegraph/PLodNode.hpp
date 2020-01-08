#ifndef AVANGO_GUA_LOD_PLOD_NODE_HPP
#define AVANGO_GUA_LOD_PLOD_NODE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/node/PLodNode.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/renderer/Material.hpp>

namespace av
{
namespace gua
{
namespace lod
{
/**
 * Wrapper for ::gua::PLodNode
 *
 * \ingroup av_gua
 */
class AV_GUA_LOD_DLL PLodNode : public av::gua::GeometryNode
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::PLodNode is created.
     * Otherwise, the given ::gua::PLodNode is used.
     */
    PLodNode(std::shared_ptr<::gua::node::PLodNode> guanode = std::shared_ptr<::gua::node::PLodNode>(new ::gua::node::PLodNode("")));

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    virtual void on_distribute(av::gua::NetTransform& netNode);
    virtual void on_undistribute(av::gua::NetTransform& netNode);
#endif

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    //      virtual ~PLodNode();

  public:
    /**
     * Get the wrapped ::gua::PLodNode.
     */
    std::shared_ptr<::gua::node::PLodNode> getGuaPLodNode() const;

    SFString Geometry;
    SFMaterial Material;
    SFFloat RadiusScale;
    SFFloat MaxSurfelRadius;
    SFFloat ErrorThreshold;
    SFBool EnableBackfaceCulling;

    SFBool EnableTimeSeriesColoring;
    SFBool EnableTimeSeriesDeformation;
    SFBool EnableAutomaticPlayback;
    SFBool EnableTemporalInterpolation;

    SFFloat TimeSeriesPlaybackSpeed;
    SFFloat TimeSeriesDeformFactor;
    SFFloat TimeCursorPosition;

    SFFloat AttributeColorToMixInFactor;

    SFInt AttributeToVisualizeIndex;
    SFInt ActiveTimeSeriesIndex;

    virtual void setGeometryCB(const SFString::SetValueEvent& event);
    virtual void getGeometryCB(const SFString::GetValueEvent& event);

    virtual void setMaterialCB(const SFMaterial::SetValueEvent& event);
    virtual void getMaterialCB(const SFMaterial::GetValueEvent& event);

    virtual void setRadiusScaleCB(const SFFloat::SetValueEvent& event);
    virtual void getRadiusScaleCB(const SFFloat::GetValueEvent& event);

    virtual void setMaxSurfelRadiusCB(const SFFloat::SetValueEvent& event);
    virtual void getMaxSurfelRadiusCB(const SFFloat::GetValueEvent& event);

    virtual void setErrorThresholdCB(const SFFloat::SetValueEvent& event);
    virtual void getErrorThresholdCB(const SFFloat::GetValueEvent& event);

    virtual void setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event);
    virtual void getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event);

    // time series visualization related
    virtual void setEnableTimeSeriesColoringCB(const SFBool::SetValueEvent& event);
    virtual void getEnableTimeSeriesColoringCB(const SFBool::GetValueEvent& event);

    virtual void setEnableTimeSeriesDeformationCB(const SFBool::SetValueEvent& event);
    virtual void getEnableTimeSeriesDeformationCB(const SFBool::GetValueEvent& event);

    virtual void setEnableAutomaticPlaybackCB(const SFBool::SetValueEvent& event);
    virtual void getEnableAutomaticPlaybackCB(const SFBool::GetValueEvent& event);

    virtual void setEnableTemporalInterpolationCB(const SFBool::SetValueEvent& event);
    virtual void getEnableTemporalInterpolationCB(const SFBool::GetValueEvent& event);

    virtual void setTimeSeriesPlaybackSpeedCB(const SFFloat::SetValueEvent& event);
    virtual void getTimeSeriesPlaybackSpeedCB(const SFFloat::GetValueEvent& event);

    virtual void setTimeCursorPositionCB(const SFFloat::SetValueEvent& event);
    virtual void getTimeCursorPositionCB(const SFFloat::GetValueEvent& event);

    virtual void setTimeSeriesDeformFactorCB(const SFFloat::SetValueEvent& event);
    virtual void getTimeSeriesDeformFactorCB(const SFFloat::GetValueEvent& event);

    virtual void setAttributeColorToMixInFactorCB(const SFFloat::SetValueEvent& event);
    virtual void getAttributeColorToMixInFactorCB(const SFFloat::GetValueEvent& event);

    virtual void setAttributeToVisualizeIndexCB(const SFInt::SetValueEvent& event);
    virtual void getAttributeToVisualizeIndexCB(const SFInt::GetValueEvent& event);

    virtual void setActiveTimeSeriesIndexCB(const SFInt::SetValueEvent& event);
    virtual void getActiveTimeSeriesIndexCB(const SFInt::GetValueEvent& event);


  private:
    std::shared_ptr<::gua::node::PLodNode> m_guaPLodNode;
    av::Link<av::gua::Material> m_Material;

    PLodNode(const PLodNode&);
    PLodNode& operator=(const PLodNode&);


  public:
    void update_cursor_position(float elapsed_frametime_ms) const;
};

typedef SingleField<Link<PLodNode>> SFPLodNode;
typedef MultiField<Link<PLodNode>> MFPLodNode;
} // namespace lod

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::PLodNode>>;
template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::PLodNode>>;
#endif

} // namespace av

#endif // AVANGO_GUA_LOD_PLOD_NODE_HPP
