#include <avango/gua/scenegraph/LineStripNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::LineStripNode);

AV_FIELD_DEFINE(av::gua::SFLineStripNode);
AV_FIELD_DEFINE(av::gua::MFLineStripNode);

av::gua::LineStripNode::LineStripNode(std::shared_ptr<::gua::node::LineStripNode> guanode) : GeometryNode(guanode), m_guaLineStripNode(guanode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Geometry, std::bind(&LineStripNode::getGeometryCB, this, std::placeholders::_1), std::bind(&LineStripNode::setGeometryCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Material, std::bind(&LineStripNode::getMaterialCB, this, std::placeholders::_1), std::bind(&LineStripNode::setMaterialCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        RenderToGBuffer, std::bind(&LineStripNode::getRenderToGBufferCB, this, std::placeholders::_1), std::bind(&LineStripNode::setRenderToGBufferCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        RenderToStencilBuffer, std::bind(&LineStripNode::getRenderToStencilBufferCB, this, std::placeholders::_1), std::bind(&LineStripNode::setRenderToStencilBufferCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        RenderVolumetric, std::bind(&LineStripNode::getRenderVolumetricCB, this, std::placeholders::_1), std::bind(&LineStripNode::setRenderVolumetricCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(RenderAsPoints, std::bind(&LineStripNode::getRenderAsPointsCB, this, std::placeholders::_1), std::bind(&LineStripNode::setRenderAsPointsCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        ScreenSpaceLineWidth, std::bind(&LineStripNode::getScreenSpaceLineWidthCB, this, std::placeholders::_1), std::bind(&LineStripNode::setScreenSpaceLineWidthCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        WasCreatedEmpty, std::bind(&LineStripNode::getWasCreatedEmptyCB, this, std::placeholders::_1), std::bind(&LineStripNode::setWasCreatedEmptyCB, this, std::placeholders::_1));

    AV_FC_ADD_FIELD(UseParabolaShape, false);

    AV_FC_ADD_ADAPTOR_FIELD(
        ParabolaParameters, std::bind(&LineStripNode::getParabolaParametersCB, this, std::placeholders::_1), std::bind(&LineStripNode::setParabolaParametersCB, this, std::placeholders::_1));

    AV_FC_ADD_FIELD(UseBezierShape, false);

    AV_FC_ADD_ADAPTOR_FIELD(
        BezierParameters, std::bind(&LineStripNode::getBezierParametersCB, this, std::placeholders::_1), std::bind(&LineStripNode::setBezierParametersCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(TriggerUpdate, std::bind(&LineStripNode::getTriggerUpdateCB, this, std::placeholders::_1), std::bind(&LineStripNode::setTriggerUpdateCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(PrivateLineStripDataString,
                            std::bind(&LineStripNode::getPrivateLineStripDataStringCB, this, std::placeholders::_1),
                            std::bind(&LineStripNode::setPrivateLineStripDataStringCB, this, std::placeholders::_1));

    if(guanode->get_material())
    {
        m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
    }
}

void av::gua::LineStripNode::clearVertices() const { m_guaLineStripNode->clear_vertices(); }

void av::gua::LineStripNode::enqueueVertex(float pos_x, float pos_y, float pos_z, float col_r, float col_g, float col_b, float col_a, float thickness) const
{
    m_guaLineStripNode->enqueue_vertex(pos_x, pos_y, pos_z, col_r, col_g, col_b, col_a, thickness);
}

void av::gua::LineStripNode::popBackVertex() const { m_guaLineStripNode->pop_back_vertex(); }

void av::gua::LineStripNode::popFrontVertex() const { m_guaLineStripNode->pop_front_vertex(); }

void av::gua::LineStripNode::pushVertex(float pos_x, float pos_y, float pos_z, float col_r, float col_g, float col_b, float col_a, float thickness) const
{
    m_guaLineStripNode->push_vertex(pos_x, pos_y, pos_z, col_r, col_g, col_b, col_a, thickness);
}

void av::gua::LineStripNode::fillWithParabola(float a, float b, float c, float sample_distance, float maximal_distance, float col_r, float col_g, float col_b)
{
    startVertexList();
    float current_z = 0.0;

    do
    {
        float height = a * current_z * current_z + b * current_z + c;
        enqueueVertex(0.0, height, current_z, col_r, col_g, col_b, 1.0, 1.0);
        current_z -= sample_distance;
    } while(current_z > -maximal_distance);

    float final_height = a * maximal_distance * maximal_distance + -b * maximal_distance + c;
    enqueueVertex(0.0, final_height, -maximal_distance, col_r, col_g, col_b, 1.0, 1.0);

    endVertexList();
}

void av::gua::LineStripNode::fillWithBezier(::gua::math::vec3 const& p0, ::gua::math::vec3 const& p1, ::gua::math::vec3 const& p2, float col_r, float col_g, float col_b, int num_segments)
{
    startVertexList();

    for(int i = 0; i <= num_segments; ++i)
    {
        float t = (1.0 / num_segments) * i;
        ::gua::math::vec3 point = (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;
        enqueueVertex(point.x, point.y, point.z, col_r, col_g, col_b, 1.0, 1.0);
    }
    endVertexList();
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::LineStripNode::on_distribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_distribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_distribute(netNode);
    }

    netNode.distributeFieldContainer(m_Material);
}

void av::gua::LineStripNode::on_undistribute(av::gua::NetTransform& netNode)
{
    GeometryNode::on_undistribute(netNode);

    if(m_Material.isValid())
    {
        m_Material->on_undistribute(netNode);
    }

    netNode.undistributeFieldContainer(m_Material);
}
#endif

void av::gua::LineStripNode::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::GeometryNode::initClass();

        AV_FC_INIT(av::gua::GeometryNode, av::gua::LineStripNode, true);

        SFLineStripNode::initClass("av::gua::SFLineStripNode", "av::Field");
        MFLineStripNode::initClass("av::gua::MFLineStripNode", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

void av::gua::LineStripNode::getGeometryCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_geometry_description(); }

void av::gua::LineStripNode::setGeometryCB(const SFString::SetValueEvent& event) { m_guaLineStripNode->set_geometry_description(event.getValue()); }

void av::gua::LineStripNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
    if(m_Material.isValid())
    {
        *(event.getValuePtr()) = m_Material;
    }
}

void av::gua::LineStripNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
    if(event.getValue().isValid())
    {
        m_Material = event.getValue();
        m_guaLineStripNode->set_material(m_Material->getGuaMaterial());
    }
}

void av::gua::LineStripNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_gbuffer(); }

void av::gua::LineStripNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event) { m_guaLineStripNode->set_render_to_gbuffer(event.getValue()); }

void av::gua::LineStripNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_stencil_buffer(); }

void av::gua::LineStripNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event) { m_guaLineStripNode->set_render_to_stencil_buffer(event.getValue()); }

void av::gua::LineStripNode::getRenderVolumetricCB(const SFBool::GetValueEvent& event)
{
    //*(event.getValuePtr()) = m_guaLineStripNode->get_render_volumetric();
}

void av::gua::LineStripNode::setRenderVolumetricCB(const SFBool::SetValueEvent& event) { m_guaLineStripNode->set_render_volumetric(event.getValue()); }

void av::gua::LineStripNode::getRenderAsPointsCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_render_vertices_as_points(); }

void av::gua::LineStripNode::setRenderAsPointsCB(const SFBool::SetValueEvent& event) { m_guaLineStripNode->set_render_vertices_as_points(event.getValue()); }

void av::gua::LineStripNode::getScreenSpaceLineWidthCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_screen_space_line_width(); }

void av::gua::LineStripNode::setScreenSpaceLineWidthCB(const SFFloat::SetValueEvent& event) { m_guaLineStripNode->set_screen_space_line_width(event.getValue()); }

void av::gua::LineStripNode::getWasCreatedEmptyCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_was_created_empty(); }

void av::gua::LineStripNode::setWasCreatedEmptyCB(const SFBool::SetValueEvent& event) { m_guaLineStripNode->set_was_created_empty(event.getValue()); }

void av::gua::LineStripNode::getParabolaParametersCB(const MFFloat::GetValueEvent& event) {}

void av::gua::LineStripNode::setParabolaParametersCB(const MFFloat::SetValueEvent& event)
{
    if(UseParabolaShape.getValue())
    {
        const std::vector<float, std::allocator<float>> values = event.getValue();

        if(values.size() == 8)
        {
            fillWithParabola(values.at(0), values.at(1), values.at(2), values.at(3), values.at(4), values.at(5), values.at(6), values.at(7));
        }
        else
        {
            std::cout << "Error: ParabolaParameters must contain eight values:" << std::endl;
            std::cout << "a, b, c, sample_distance, maximal_distance, col_r, col_g, col_b" << std::endl;
        }
    }
}

void av::gua::LineStripNode::getBezierParametersCB(const MFFloat::GetValueEvent& event) {}

void av::gua::LineStripNode::setBezierParametersCB(const MFFloat::SetValueEvent& event)
{
    if(UseBezierShape.getValue())
    {
        const std::vector<float, std::allocator<float>> values = event.getValue();

        if(values.size() == 13)
        {
            fillWithBezier(::gua::math::vec3(values.at(0), values.at(1), values.at(2)),
                           ::gua::math::vec3(values.at(3), values.at(4), values.at(5)),
                           ::gua::math::vec3(values.at(6), values.at(7), values.at(8)),
                           values.at(9),
                           values.at(10),
                           values.at(11),
                           values.at(12));
        }
        else
        {
            std::cout << "Error: BezierParameters must contain 13 values:" << std::endl;
            std::cout << "x0, y0, z0, x1, y1, z1, x2, y2, z2, col_r, col_g, col_b, num_segments" << std::endl;
        }
    }
}

void av::gua::LineStripNode::getTriggerUpdateCB(const SFBool::GetValueEvent& event) { *(event.getValuePtr()) = m_guaLineStripNode->get_trigger_update(); }

void av::gua::LineStripNode::setTriggerUpdateCB(const SFBool::SetValueEvent& event)
{
    role_server_client_unidentified = event.getValue() == 1 ? 0 : 1;
    if(0 == role_server_client_unidentified)
    {
        m_guaLineStripNode->set_trigger_update(!event.getValue());

        if(!UseParabolaShape.getValue() && !UseBezierShape.getValue())
        {
            std::string compiled_buffer_string;
            m_guaLineStripNode->compile_buffer_string(compiled_buffer_string);
            privateLineStripData = compiled_buffer_string;
        }
    }
}

void av::gua::LineStripNode::getPrivateLineStripDataStringCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = privateLineStripData; }

void av::gua::LineStripNode::setPrivateLineStripDataStringCB(const SFString::SetValueEvent& event)
{
    if(1 == role_server_client_unidentified)
    {
        if(!UseParabolaShape.getValue() && !UseBezierShape.getValue())
        {
            std::string compiled_buffer_string = event.getValue();
            m_guaLineStripNode->uncompile_buffer_string(compiled_buffer_string);
        }
    }
}

std::shared_ptr<::gua::node::LineStripNode> av::gua::LineStripNode::getGuaLineStripNode() const { return m_guaLineStripNode; }

void av::gua::LineStripNode::startVertexList() { m_guaLineStripNode->clear_vertices(); }

void av::gua::LineStripNode::endVertexList()
{
    m_guaLineStripNode->forward_queued_vertices();
    TriggerUpdate.setValue(true);
    PrivateLineStripDataString.setValue("");
}