#include <avango/gua/physics/BoxShape.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::BoxShape"));
}

AV_FC_DEFINE(av::gua::BoxShape);

AV_FIELD_DEFINE(av::gua::SFBoxShape);
AV_FIELD_DEFINE(av::gua::MFBoxShape);

av::gua::BoxShape::BoxShape(::gua::physics::BoxShape* guashape)
  : CollisionShape(guashape),
    m_guaShape(reinterpret_cast< ::gua::physics::BoxShape*>(CollisionShape::getGuaShape()))
{
    AV_FC_ADD_ADAPTOR_FIELD(HalfExtents,
                        boost::bind(&BoxShape::getHalfExtentsCB, this, _1),
                        boost::bind(&BoxShape::setHalfExtentsCB, this, _1));

}

av::gua::BoxShape::~BoxShape()
{}

void
av::gua::BoxShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::BoxShape, true);

        SFBoxShape::initClass("av::gua::SFBoxShape", "av::Field");
        MFBoxShape::initClass("av::gua::MFBoxShape", "av::Field");
    }
}

::gua::physics::BoxShape*
av::gua::BoxShape::getGuaShape() const {
    return m_guaShape;
}

void
av::gua::BoxShape::getHalfExtentsCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->get_half_extents();
}

void
av::gua::BoxShape::setHalfExtentsCB(const SFVec3::SetValueEvent& event)
{
    m_guaShape->set_half_extents(event.getValue());
}
