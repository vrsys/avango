#include <avango/gua/math/LineStripVertex.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

namespace
{
  av::Logger& logger(av::getLogger("av::gua::LineStripVertex"));
}

AV_FC_DEFINE(av::gua::LineStripVertex);

AV_FIELD_DEFINE(av::gua::SFLineStripVertex);
AV_FIELD_DEFINE(av::gua::MFLineStripVertex);

av::gua::LineStripVertex::LineStripVertex()
{
  /*
  AV_FC_ADD_ADAPTOR_FIELD(Position,
                        std::bind(&LineStripVertex::getPositionCB, this, std::placeholders::_1),
                        std::bind(&LineStripVertex::setPositionCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Color,
                        std::bind(&LineStripVertex::getColorCB, this, std::placeholders::_1),
                        std::bind(&LineStripVertex::setColorCB, this, std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Thickness,
                        std::bind(&LineStripVertex::getThicknessCB, this, std::placeholders::_1),
                        std::bind(&LineStripVertex::setThicknessCB, this, std::placeholders::_1));
  */



  
}

av::gua::LineStripVertex::~LineStripVertex()
{}

void 
av::gua::LineStripVertex::pushLineStripVertex(::gua::math::vec3f const& v_position,
                                              ::gua::math::vec3f const& v_color,
                                              float const& thickness) {

  ::gua::LineStrip::Vertex v{v_position[0], v_position[1], v_position[2],
                             v_color[0], v_color[1], v_color[2], 1.0,
                             thickness,
                             0.0, 1.0, 0.0};
  queued_vertices.push_back(v);
}

/*
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
void av::gua::LineStripVertex::on_distribute(av::gua::NetTransform& netNode) 
{

  if (m_materialShaderDescription.getValue().isValid()) {
    m_materialShaderDescription.getValue()->on_distribute(netNode);
    netNode.distributeFieldContainer(m_materialShaderDescription.getValue());
  }

  m_distributed = true;
  m_uniformsDirty.setValue(true);
}

void av::gua::LineStripVertex::on_undistribute(av::gua::NetTransform& netNode) 
{   
  if (m_materialShaderDescription.getValue().isValid()) {
    m_materialShaderDescription.getValue()->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_materialShaderDescription.getValue());
  m_distributed = false;
}
#endif
*/
void
av::gua::LineStripVertex::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::LineStripVertex, true);

        SFLineStripVertex::initClass("av::gua::SFLineStripVertex", "av::Field");
        MFLineStripVertex::initClass("av::gua::MFLineStripVertex", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

