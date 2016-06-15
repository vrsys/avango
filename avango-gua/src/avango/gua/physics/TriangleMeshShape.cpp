#include <avango/gua/physics/TriangleMeshShape.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TriangleMeshShape"));
}

AV_FC_DEFINE(av::gua::TriangleMeshShape);

AV_FIELD_DEFINE(av::gua::SFTriangleMeshShape);
AV_FIELD_DEFINE(av::gua::MFTriangleMeshShape);

av::gua::TriangleMeshShape::TriangleMeshShape(::gua::physics::TriangleMeshShape* guashape)
  : CollisionShape(guashape),
    m_guaShape(reinterpret_cast< ::gua::physics::TriangleMeshShape*>(CollisionShape::getGuaShape()))
{
    AV_FC_ADD_ADAPTOR_FIELD(Scaling,
                        std::bind(&TriangleMeshShape::getScalingCB, this,std::placeholders::_1),
                        std::bind(&TriangleMeshShape::setScalingCB, this,std::placeholders::_1));

}

av::gua::TriangleMeshShape::~TriangleMeshShape()
{}

void
av::gua::TriangleMeshShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::TriangleMeshShape, true);

        SFTriangleMeshShape::initClass("av::gua::SFTriangleMeshShape", "av::Field");
        MFTriangleMeshShape::initClass("av::gua::MFTriangleMeshShape", "av::Field");
    }
}

::gua::physics::TriangleMeshShape*
av::gua::TriangleMeshShape::getGuaShape() const {
    return m_guaShape;
}

void
av::gua::TriangleMeshShape::getScalingCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->get_scaling();
}

void
av::gua::TriangleMeshShape::setScalingCB(const SFVec3::SetValueEvent& event)
{
    m_guaShape->set_scaling(event.getValue());
}
