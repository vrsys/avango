#include <avango/gua/physics/FixedConstraint.hpp>

#include <avango/gua/Types.hpp>

#include <avango/Base.h>
#include <boost/bind.hpp>
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
                        boost::bind(&FixedConstraint::getFrameACB, this, _1),
                        boost::bind(&FixedConstraint::setFrameACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FrameB,
                        boost::bind(&FixedConstraint::getFrameBCB, this, _1),
                        boost::bind(&FixedConstraint::setFrameBCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LockRotation,
                        boost::bind(&FixedConstraint::getLockRotationCB, this, _1),
                        boost::bind(&FixedConstraint::setLockRotationCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LockTranslation,
                        boost::bind(&FixedConstraint::getLockTranslationCB, this, _1),
                        boost::bind(&FixedConstraint::setLockTranslationCB, this, _1));
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

