#include <avango/gua/physics/Point2PointConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Point2PointConstraint"));
}

AV_FC_DEFINE(av::gua::Point2PointConstraint);

AV_FIELD_DEFINE(av::gua::SFPoint2PointConstraint);
AV_FIELD_DEFINE(av::gua::MFPoint2PointConstraint);

av::gua::Point2PointConstraint::Point2PointConstraint(::gua::physics::Point2PointConstraint* guaconstraint)
  : Constraint(guaconstraint),
    m_guaConstraint(reinterpret_cast< ::gua::physics::Point2PointConstraint*>(Constraint::getGuaConstraint()))
{
    AV_FC_ADD_ADAPTOR_FIELD(PivotA,
                        boost::bind(&Point2PointConstraint::getPivotACB, this, _1),
                        boost::bind(&Point2PointConstraint::setPivotACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(PivotB,
                        boost::bind(&Point2PointConstraint::getPivotBCB, this, _1),
                        boost::bind(&Point2PointConstraint::setPivotBCB, this, _1));
}

av::gua::Point2PointConstraint::~Point2PointConstraint()
{}

void
av::gua::Point2PointConstraint::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Constraint::initClass();

        AV_FC_INIT(av::gua::Constraint, av::gua::Point2PointConstraint, true);

        SFPoint2PointConstraint::initClass("av::gua::SFPoint2PointConstraint", "av::Field");
        MFPoint2PointConstraint::initClass("av::gua::MFPoint2PointConstraint", "av::Field");
    }
}

::gua::physics::Point2PointConstraint*
av::gua::Point2PointConstraint::getGuaConstraint() const
{
    return m_guaConstraint;
}

void
av::gua::Point2PointConstraint::getPivotACB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->pivot_a();
}

void
av::gua::Point2PointConstraint::setPivotACB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_pivot_a(event.getValue());
}

void
av::gua::Point2PointConstraint::getPivotBCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->pivot_b();
}

void
av::gua::Point2PointConstraint::setPivotBCB(const SFVec3::SetValueEvent& event)
{
    m_guaConstraint->set_pivot_b(event.getValue());
}
