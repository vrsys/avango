#include <avango/gua/physics/SliderConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SliderConstraint"));
}

AV_FC_DEFINE(av::gua::SliderConstraint);

AV_FIELD_DEFINE(av::gua::SFSliderConstraint);
AV_FIELD_DEFINE(av::gua::MFSliderConstraint);

av::gua::SliderConstraint::SliderConstraint(::gua::physics::SliderConstraint* guaconstraint)
  : Constraint(guaconstraint),
    m_guaConstraint(reinterpret_cast< ::gua::physics::SliderConstraint*>(Constraint::getGuaConstraint()))
{
    AV_FC_ADD_ADAPTOR_FIELD(FrameA,
                        boost::bind(&SliderConstraint::getFrameACB, this, _1),
                        boost::bind(&SliderConstraint::setFrameACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FrameB,
                        boost::bind(&SliderConstraint::getFrameBCB, this, _1),
                        boost::bind(&SliderConstraint::setFrameBCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LowerLinearLimit,
                        boost::bind(&SliderConstraint::getLowerLinearLimitCB, this, _1),
                        boost::bind(&SliderConstraint::setLowerLinearLimitCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(UpperLinearLimit,
                        boost::bind(&SliderConstraint::getUpperLinearLimitCB, this, _1),
                        boost::bind(&SliderConstraint::setUpperLinearLimitCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LowerAngularLimit,
                        boost::bind(&SliderConstraint::getLowerAngularLimitCB, this, _1),
                        boost::bind(&SliderConstraint::setLowerAngularLimitCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(UpperAngularLimit,
                        boost::bind(&SliderConstraint::getUpperAngularLimitCB, this, _1),
                        boost::bind(&SliderConstraint::setUpperAngularLimitCB, this, _1));


}

av::gua::SliderConstraint::~SliderConstraint()
{}

void
av::gua::SliderConstraint::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Constraint::initClass();

        AV_FC_INIT(av::gua::Constraint, av::gua::SliderConstraint, true);

        SFSliderConstraint::initClass("av::gua::SFSliderConstraint", "av::Field");
        MFSliderConstraint::initClass("av::gua::MFSliderConstraint", "av::Field");
    }
}

::gua::physics::SliderConstraint*
av::gua::SliderConstraint::getGuaConstraint() const
{
    return m_guaConstraint;
}

void
av::gua::SliderConstraint::getFrameACB(const SFMatrix4::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->frame_a();
}

void
av::gua::SliderConstraint::setFrameACB(const SFMatrix4::SetValueEvent& event)
{
    m_guaConstraint->set_frame_a(event.getValue());
}

void
av::gua::SliderConstraint::getFrameBCB(const SFMatrix4::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->frame_b();
}

void
av::gua::SliderConstraint::setFrameBCB(const SFMatrix4::SetValueEvent& event)
{
    m_guaConstraint->set_frame_b(event.getValue());
}

void
av::gua::SliderConstraint::getLowerLinearLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->lower_linear_limit();
}

void
av::gua::SliderConstraint::setLowerLinearLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_lower_linear_limit(event.getValue());
}

void
av::gua::SliderConstraint::getUpperLinearLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->upper_linear_limit();
}

void
av::gua::SliderConstraint::setUpperLinearLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_upper_linear_limit(event.getValue());
}

void
av::gua::SliderConstraint::getLowerAngularLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->lower_angular_limit();
}

void
av::gua::SliderConstraint::setLowerAngularLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_lower_angular_limit(event.getValue());
}

void
av::gua::SliderConstraint::getUpperAngularLimitCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->upper_angular_limit();
}

void
av::gua::SliderConstraint::setUpperAngularLimitCB(const SFFloat::SetValueEvent& event)
{
    m_guaConstraint->set_upper_angular_limit(event.getValue());
}
