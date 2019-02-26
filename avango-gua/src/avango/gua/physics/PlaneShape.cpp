#include <avango/gua/physics/PlaneShape.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::PlaneShape"));
}

AV_FC_DEFINE(av::gua::PlaneShape);

AV_FIELD_DEFINE(av::gua::SFPlaneShape);
AV_FIELD_DEFINE(av::gua::MFPlaneShape);

av::gua::PlaneShape::PlaneShape(::gua::physics::PlaneShape* guashape) : CollisionShape(guashape), m_guaShape(reinterpret_cast<::gua::physics::PlaneShape*>(CollisionShape::getGuaShape()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Normal, std::bind(&PlaneShape::getNormalCB, this, std::placeholders::_1), std::bind(&PlaneShape::setNormalCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(PlaneConstant, std::bind(&PlaneShape::getPlaneConstantCB, this, std::placeholders::_1), std::bind(&PlaneShape::setPlaneConstantCB, this, std::placeholders::_1));
}

av::gua::PlaneShape::~PlaneShape() {}

void av::gua::PlaneShape::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::PlaneShape, true);

        SFPlaneShape::initClass("av::gua::SFPlaneShape", "av::Field");
        MFPlaneShape::initClass("av::gua::MFPlaneShape", "av::Field");
    }
}

::gua::physics::PlaneShape* av::gua::PlaneShape::getGuaShape() const { return m_guaShape; }

void av::gua::PlaneShape::getNormalCB(const SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = m_guaShape->get_normal(); }

void av::gua::PlaneShape::setNormalCB(const SFVec3::SetValueEvent& event) { m_guaShape->set_normal(event.getValue()); }

void av::gua::PlaneShape::getPlaneConstantCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaShape->get_plane_constant(); }

void av::gua::PlaneShape::setPlaneConstantCB(const SFFloat::SetValueEvent& event) { m_guaShape->set_plane_constant(event.getValue()); }
