#include <avango/gua/physics/FixedConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::FixedConstraint"));
}

AV_FC_DEFINE(av::gua::FixedConstraint);

AV_FIELD_DEFINE(av::gua::SFFixedConstraint);
AV_FIELD_DEFINE(av::gua::MFFixedConstraint);

av::gua::FixedConstraint::FixedConstraint(::gua::physics::FixedConstraint* guaconstraint)
  : Constraint(guaconstraint),
    m_guaConstraint(reinterpret_cast< ::gua::physics::FixedConstraint*>(Constraint::getGuaConstraint()))
{
    AV_FC_ADD_ADAPTOR_FIELD(FrameA,
                        std::bind(&FixedConstraint::getFrameACB, this,std::placeholders::_1),
                        std::bind(&FixedConstraint::setFrameACB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(FrameB,
                        std::bind(&FixedConstraint::getFrameBCB, this,std::placeholders::_1),
                        std::bind(&FixedConstraint::setFrameBCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(LockRotation,
                        std::bind(&FixedConstraint::getLockRotationCB, this,std::placeholders::_1),
                        std::bind(&FixedConstraint::setLockRotationCB, this,std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(LockTranslation,
                        std::bind(&FixedConstraint::getLockTranslationCB, this,std::placeholders::_1),
                        std::bind(&FixedConstraint::setLockTranslationCB, this,std::placeholders::_1));
}

av::gua::FixedConstraint::~FixedConstraint()
{}

void
av::gua::FixedConstraint::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::Constraint::initClass();

        AV_FC_INIT(av::gua::Constraint, av::gua::FixedConstraint, true);

        SFFixedConstraint::initClass("av::gua::SFFixedConstraint", "av::Field");
        MFFixedConstraint::initClass("av::gua::MFFixedConstraint", "av::Field");
    }
}

::gua::physics::FixedConstraint*
av::gua::FixedConstraint::getGuaConstraint() const
{
    return m_guaConstraint;
}

void
av::gua::FixedConstraint::getFrameACB(const SFMatrix4::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->frame_a();
}

void
av::gua::FixedConstraint::setFrameACB(const SFMatrix4::SetValueEvent& event)
{
    m_guaConstraint->set_frame_a(event.getValue());
}

void
av::gua::FixedConstraint::getFrameBCB(const SFMatrix4::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->frame_b();
}

void
av::gua::FixedConstraint::setFrameBCB(const SFMatrix4::SetValueEvent& event)
{
    m_guaConstraint->set_frame_b(event.getValue());
}

void
av::gua::FixedConstraint::getLockRotationCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->is_rotation_locked();
}

void
av::gua::FixedConstraint::setLockRotationCB(const SFBool::SetValueEvent& event)
{
    m_guaConstraint->lock_rotation(event.getValue());
}

void
av::gua::FixedConstraint::getLockTranslationCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaConstraint->is_translation_locked();
}

void
av::gua::FixedConstraint::setLockTranslationCB(const SFBool::SetValueEvent& event)
{
    m_guaConstraint->lock_translation(event.getValue());
}

