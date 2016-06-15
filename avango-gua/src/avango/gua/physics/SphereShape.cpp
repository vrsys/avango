#include <avango/gua/physics/SphereShape.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SphereShape"));
}

AV_FC_DEFINE(av::gua::SphereShape);

AV_FIELD_DEFINE(av::gua::SFSphereShape);
AV_FIELD_DEFINE(av::gua::MFSphereShape);

av::gua::SphereShape::SphereShape(::gua::physics::SphereShape* guashape)
  : CollisionShape(guashape),
    m_guaShape(reinterpret_cast< ::gua::physics::SphereShape*>(CollisionShape::getGuaShape()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Radius,
                        std::bind(&SphereShape::getRadiusCB, this,std::placeholders::_1),
                        std::bind(&SphereShape::setRadiusCB, this,std::placeholders::_1));

}

av::gua::SphereShape::~SphereShape()
{}

void
av::gua::SphereShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::SphereShape, true);

        SFSphereShape::initClass("av::gua::SFSphereShape", "av::Field");
        MFSphereShape::initClass("av::gua::MFSphereShape", "av::Field");
    }
}

::gua::physics::SphereShape*
av::gua::SphereShape::getGuaShape() const {
    return m_guaShape;
}

void
av::gua::SphereShape::getRadiusCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->get_radius();
}

void
av::gua::SphereShape::setRadiusCB(const SFFloat::SetValueEvent& event)
{
    m_guaShape->set_radius(event.getValue());
}
