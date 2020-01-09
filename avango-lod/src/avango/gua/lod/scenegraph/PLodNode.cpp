#include <avango/gua/lod/scenegraph/PLodNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::lod::PLodNode);

AV_FIELD_DEFINE(av::gua::lod::SFPLodNode);
AV_FIELD_DEFINE(av::gua::lod::MFPLodNode);

av::gua::lod::PLodNode::PLodNode(std::shared_ptr<::gua::node::PLodNode> guanode) : GeometryNode(guanode), m_guaPLodNode(std::dynamic_pointer_cast<::gua::node::PLodNode>(GeometryNode::getGuaNode()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Geometry, std::bind(&PLodNode::getGeometryCB, this, std::placeholders::_1), std::bind(&PLodNode::setGeometryCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Material, std::bind(&PLodNode::getMaterialCB, this, std::placeholders::_1), std::bind(&PLodNode::setMaterialCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(RadiusScale, std::bind(&PLodNode::getRadiusScaleCB, this, std::placeholders::_1), std::bind(&PLodNode::setRadiusScaleCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(MaxSurfelRadius, std::bind(&PLodNode::getMaxSurfelRadiusCB, this, std::placeholders::_1), std::bind(&PLodNode::setMaxSurfelRadiusCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(ErrorThreshold, std::bind(&PLodNode::getErrorThresholdCB, this, std::placeholders::_1), std::bind(&PLodNode::setErrorThresholdCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableBackfaceCulling, std::bind(&PLodNode::getEnableBackfaceCullingCB, this, std::placeholders::_1), std::bind(&PLodNode::setEnableBackfaceCullingCB, this, std::placeholders::_1));
    
    AV_FC_ADD_ADAPTOR_FIELD(
        EnableTimeSeriesColoring, std::bind(&PLodNode::getEnableTimeSeriesColoringCB, this, std::placeholders::_1), std::bind(&PLodNode::setEnableTimeSeriesColoringCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableTimeSeriesDeformation, std::bind(&PLodNode::getEnableTimeSeriesDeformationCB, this, std::placeholders::_1), std::bind(&PLodNode::setEnableTimeSeriesDeformationCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableAutomaticPlayback, std::bind(&PLodNode::getEnableAutomaticPlaybackCB, this, std::placeholders::_1), std::bind(&PLodNode::setEnableAutomaticPlaybackCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        EnableTemporalInterpolation, std::bind(&PLodNode::getEnableTemporalInterpolationCB, this, std::placeholders::_1), std::bind(&PLodNode::setEnableTemporalInterpolationCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        TimeSeriesPlaybackSpeed, std::bind(&PLodNode::getTimeSeriesPlaybackSpeedCB, this, std::placeholders::_1), std::bind(&PLodNode::setTimeSeriesPlaybackSpeedCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        TimeSeriesDeformFactor, std::bind(&PLodNode::getTimeSeriesDeformFactorCB, this, std::placeholders::_1), std::bind(&PLodNode::setTimeSeriesDeformFactorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        AttributeColorToMixInFactor, std::bind(&PLodNode::getAttributeColorToMixInFactorCB, this, std::placeholders::_1), std::bind(&PLodNode::setAttributeColorToMixInFactorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        AttributeToVisualizeIndex, std::bind(&PLodNode::getAttributeToVisualizeIndexCB, this, std::placeholders::_1), std::bind(&PLodNode::setAttributeToVisualizeIndexCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        ActiveTimeSeriesIndex, std::bind(&PLodNode::getActiveTimeSeriesIndexCB, this, std::placeholders::_1), std::bind(&PLodNode::setActiveTimeSeriesIndexCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        TimeCursorPosition, std::bind(&PLodNode::getTimeCursorPositionCB, this, std::placeholders::_1), std::bind(&PLodNode::setTimeCursorPositionCB, this, std::placeholders::_1));


    if(guanode->get_material())
    {
        m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
    }
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::lod::PLodNode::on_distribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_distribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_distribute(netNode);
    }
    netNode.distributeFieldContainer(m_Material);
}

void av::gua::lod::PLodNode::on_undistribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_undistribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_undistribute(netNode);
    }
    netNode.undistributeFieldContainer(m_Material);
}
#endif

void av::gua::lod::PLodNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::lod::PLodNode, true);

        SFPLodNode::initClass("av::gua::lod::SFPLodNode", "av::Field");
        MFPLodNode::initClass("av::gua::lod::MFPLodNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::lod::PLodNode::getGeometryCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_geometry_description(); }

void av::gua::lod::PLodNode::setGeometryCB(const SFString::SetValueEvent& event) { m_guaPLodNode->set_geometry_description(event.getValue()); }

void av::gua::lod::PLodNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
    if(m_Material.isValid())
    {
        *(event.getValuePtr()) = m_Material;
    }
}

void av::gua::lod::PLodNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
    if(event.getValue().isValid())
    {
        m_Material = event.getValue();
        m_guaPLodNode->set_material(m_Material->getGuaMaterial());
    }
}

void av::gua::lod::PLodNode::getRadiusScaleCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_radius_scale(); }

void av::gua::lod::PLodNode::setRadiusScaleCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_radius_scale(event.getValue()); }

void av::gua::lod::PLodNode::getMaxSurfelRadiusCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_max_surfel_radius(); }

void av::gua::lod::PLodNode::setMaxSurfelRadiusCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_max_surfel_radius(event.getValue()); }

void av::gua::lod::PLodNode::getErrorThresholdCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_error_threshold(); }

void av::gua::lod::PLodNode::setErrorThresholdCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_error_threshold(event.getValue()); }

void av::gua::lod::PLodNode::getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_enable_backface_culling_by_normal(); }

void av::gua::lod::PLodNode::setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event) { m_guaPLodNode->set_enable_backface_culling_by_normal(event.getValue()); }

// time series visualization related

void av::gua::lod::PLodNode::getEnableTimeSeriesColoringCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_enable_time_series_coloring(); }

void av::gua::lod::PLodNode::setEnableTimeSeriesColoringCB(const SFBool::SetValueEvent& event) { m_guaPLodNode->set_enable_time_series_coloring(event.getValue()); }

void av::gua::lod::PLodNode::getEnableTimeSeriesDeformationCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_enable_time_series_deformation(); }

void av::gua::lod::PLodNode::setEnableTimeSeriesDeformationCB(const SFBool::SetValueEvent& event) { m_guaPLodNode->set_enable_time_series_deformation(event.getValue()); }

void av::gua::lod::PLodNode::getEnableAutomaticPlaybackCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_enable_automatic_playback(); }

void av::gua::lod::PLodNode::setEnableAutomaticPlaybackCB(const SFBool::SetValueEvent& event) { m_guaPLodNode->set_enable_automatic_playback(event.getValue()); }

void av::gua::lod::PLodNode::getEnableTemporalInterpolationCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_enable_temporal_interpolation(); }

void av::gua::lod::PLodNode::setEnableTemporalInterpolationCB(const SFBool::SetValueEvent& event) { m_guaPLodNode->set_enable_temporal_interpolation(event.getValue()); }

void av::gua::lod::PLodNode::getTimeSeriesPlaybackSpeedCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_time_series_playback_speed(); }

void av::gua::lod::PLodNode::setTimeSeriesPlaybackSpeedCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_time_series_playback_speed(event.getValue()); }

void av::gua::lod::PLodNode::getTimeSeriesDeformFactorCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_time_series_deform_factor(); }

void av::gua::lod::PLodNode::setTimeSeriesDeformFactorCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_time_series_deform_factor(event.getValue()); }

void av::gua::lod::PLodNode::getAttributeColorToMixInFactorCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_attribute_color_mix_in_factor(); }

void av::gua::lod::PLodNode::setAttributeColorToMixInFactorCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_attribute_color_mix_in_factor(event.getValue()); }

void av::gua::lod::PLodNode::getAttributeToVisualizeIndexCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_attribute_to_visualize_index(); }

void av::gua::lod::PLodNode::setAttributeToVisualizeIndexCB(const SFInt::SetValueEvent& event) { m_guaPLodNode->set_attribute_to_visualize_index(event.getValue()); }

void av::gua::lod::PLodNode::getActiveTimeSeriesIndexCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_active_time_series_index(); }

void av::gua::lod::PLodNode::setActiveTimeSeriesIndexCB(const SFInt::SetValueEvent& event) { m_guaPLodNode->set_active_time_series_index(event.getValue()); }


void av::gua::lod::PLodNode::getTimeCursorPositionCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaPLodNode->get_time_cursor_position(); }

void av::gua::lod::PLodNode::setTimeCursorPositionCB(const SFFloat::SetValueEvent& event) { m_guaPLodNode->set_time_cursor_position(event.getValue()); }

std::shared_ptr<::gua::node::PLodNode> av::gua::lod::PLodNode::getGuaPLodNode() const { return m_guaPLodNode; }

void av::gua::lod::PLodNode::update_cursor_position(float elapsed_frametime_ms) const
{
    m_guaPLodNode->update_time_cursor(elapsed_frametime_ms);
}

av::gua::SFMatrix* av::gua::lod::PLodNode::get_active_time_series_transform() const {
    auto gua_matrix = ::gua::math::mat4d(m_guaPLodNode->get_active_time_series_transform());

    auto result(new av::gua::SFMatrix4());
    
    result->setValue( gua_matrix );
 
    return result;
}

av::SFFloat* av::gua::lod::PLodNode::get_current_time_step() const {
    float current_time_step = m_guaPLodNode->get_current_time_step();

    auto result(new av::SFFloat());
    result->setValue(current_time_step);

    return result;
}

av::SFInt* av::gua::lod::PLodNode::get_number_of_simulation_positions() const {
    int number_of_simulation_positions = m_guaPLodNode->get_number_of_simulation_positions();

    auto result(new av::SFInt());
    result->setValue(number_of_simulation_positions);

    return result;
}

av::gua::MFVec3* av::gua::lod::PLodNode::get_current_simulation_positions() const {
    auto const& current_positions = m_guaPLodNode->get_current_simulation_positions();

    auto results(new av::gua::MFVec3());
    
    for(auto const& position : current_positions) {
        results->add1Value( scm::math::vec3d(position) );
    }

    return results;
}