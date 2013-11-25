#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::CollisionShape"));
}

AV_FC_DEFINE_ABSTRACT(av::gua::CollisionShape);

AV_FIELD_DEFINE(av::gua::SFCollisionShape);
AV_FIELD_DEFINE(av::gua::MFCollisionShape);

av::gua::CollisionShape::CollisionShape(::gua::physics::CollisionShape* guashape)
  : m_guaShape(guashape)
{
    AV_FC_ADD_ADAPTOR_FIELD(IsDynamicShape,
                        boost::bind(&CollisionShape::getIsDynamicShapeCB, this, _1),
                        boost::bind(&CollisionShape::setIsDynamicShapeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(IsStaticShape,
                        boost::bind(&CollisionShape::getIsStaticShapeCB, this, _1),
                        boost::bind(&CollisionShape::setIsStaticShapeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(HasIdenticalShapeConstructor,
                        boost::bind(&CollisionShape::getHasIdenticalShapeConstructorCB, this, _1),
                        boost::bind(&CollisionShape::setHasIdenticalShapeConstructorCB, this, _1));
}

av::gua::CollisionShape::~CollisionShape()
{}

void
av::gua::CollisionShape::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT_ABSTRACT(av::FieldContainer, av::gua::CollisionShape, true);

        SFCollisionShape::initClass("av::gua::SFCollisionShape", "av::Field");
        MFCollisionShape::initClass("av::gua::MFCollisionShape", "av::Field");
    }
}

::gua::physics::CollisionShape*
av::gua::CollisionShape::getGuaShape() const
{
    return m_guaShape;
}

void
av::gua::CollisionShape::getIsDynamicShapeCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->is_dynamic_shape();
}

void
av::gua::CollisionShape::setIsDynamicShapeCB(const SFBool::SetValueEvent& event)
{
    m_guaShape->set_is_dynamic_shape(event.getValue());
}

void
av::gua::CollisionShape::getIsStaticShapeCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->is_static_shape();
}

void
av::gua::CollisionShape::setIsStaticShapeCB(const SFBool::SetValueEvent& event)
{
    m_guaShape->set_is_static_shape(event.getValue());
}

void
av::gua::CollisionShape::getHasIdenticalShapeConstructorCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaShape->has_identical_shape_constructor();
}

void
av::gua::CollisionShape::setHasIdenticalShapeConstructorCB(const SFBool::SetValueEvent& event)
{
    m_guaShape->set_has_identical_shape_constructor(event.getValue());
}
