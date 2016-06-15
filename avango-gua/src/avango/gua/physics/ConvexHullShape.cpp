#include <avango/gua/physics/ConvexHullShape.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ConvexHullShape"));
}

AV_FC_DEFINE(av::gua::ConvexHullShape);

AV_FIELD_DEFINE(av::gua::SFConvexHullShape);
AV_FIELD_DEFINE(av::gua::MFConvexHullShape);

av::gua::ConvexHullShape::ConvexHullShape(::gua::physics::ConvexHullShape* guashape)
  : CollisionShape(guashape),
    m_guaShape(reinterpret_cast< ::gua::physics::ConvexHullShape*>(CollisionShape::getGuaShape()))
{

}

av::gua::ConvexHullShape::~ConvexHullShape()
{}

void
av::gua::ConvexHullShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::CollisionShape::initClass();

        AV_FC_INIT(av::gua::CollisionShape, av::gua::ConvexHullShape, true);

        SFConvexHullShape::initClass("av::gua::SFConvexHullShape", "av::Field");
        MFConvexHullShape::initClass("av::gua::MFConvexHullShape", "av::Field");
    }
}

::gua::physics::ConvexHullShape*
av::gua::ConvexHullShape::getGuaShape() const {
    return m_guaShape;
}
