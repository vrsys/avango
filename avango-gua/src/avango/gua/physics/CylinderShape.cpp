#include <avango/gua/physics/CylinderShape.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::CylinderShape"));
}

AV_FC_DEFINE(av::gua::CylinderShape);

AV_FIELD_DEFINE(av::gua::SFCylinderShape);
AV_FIELD_DEFINE(av::gua::MFCylinderShape);

av::gua::CylinderShape::CylinderShape(::gua::physics::CylinderShape* guashape)
  : CollisionShape(guashape),
    m_guaShape(reinterpret_cast< ::gua::physics::CylinderShape*>(CollisionShape::getGuaShape()))
{
    AV_FC_ADD_ADAPTOR_FIELD(HalfExtents,
                        boost::bind(&CylinderShape::getHalfExtentsCB, this, _1),
                        boost::bind(&CylinderShape::setHalfExtentsCB, this, _1));

}

av::gua::CylinderShape::~CylinderShape()
{}

void
av::gua::CylinderShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::CylinderShape, true);

        SFCylinderShape::initClass("av::gua::SFCylinderShape", "av::Field");
        MFCylinderShape::initClass("av::gua::MFCylinderShape", "av::Field");
    }
}

::gua::physics::CylinderShape*
av::gua::CylinderShape::getGuaShape() const {
    return m_guaShape;
}

void
av::gua::CylinderShape::getHalfExtentsCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->get_half_extents();
}

void
av::gua::CylinderShape::setHalfExtentsCB(const SFVec3::SetValueEvent& event)
{
    m_guaShape->set_half_extents(event.getValue());
}
