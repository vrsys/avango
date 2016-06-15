#include <avango/gua/physics/SliderConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <functional>
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
                        std::bind(&SliderConstraint::getFrameACB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setFrameACB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FrameB,
                        std::bind(&SliderConstraint::getFrameBCB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setFrameBCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(LowerLinearLimit,
                        std::bind(&SliderConstraint::getLowerLinearLimitCB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setLowerLinearLimitCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(UpperLinearLimit,
                        std::bind(&SliderConstraint::getUpperLinearLimitCB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setUpperLinearLimitCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(LowerAngularLimit,
                        std::bind(&SliderConstraint::getLowerAngularLimitCB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setLowerAngularLimitCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(UpperAngularLimit,
                        std::bind(&SliderConstraint::getUpperAngularLimitCB, this,std::placeholders::_1),
                        std::bind(&SliderConstraint::setUpperAngularLimitCB, this,std::placeholders::_1));


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
