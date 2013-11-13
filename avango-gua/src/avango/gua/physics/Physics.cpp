#include <avango/gua/physics/Physics.hpp>

#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Physics"));
}

AV_FC_DEFINE(av::gua::Physics);

AV_FIELD_DEFINE(av::gua::SFPhysics);
AV_FIELD_DEFINE(av::gua::MFPhysics);

av::gua::Physics::Physics(::gua::physics::Physics* guaPhysics)
  : m_guaPhysics(guaPhysics),
    m_state(RunningState::STOPPED),
    m_simulationRateReduced(false)
{
    AV_FC_ADD_ADAPTOR_FIELD(State,
                        boost::bind(&Physics::getStateCB, this, _1),
                        boost::bind(&Physics::setStateCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Gravity,
                        boost::bind(&Physics::getGravityCB, this, _1),
                        boost::bind(&Physics::setGravityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FixedTimeStep,
                        boost::bind(&Physics::getFixedTimeStepCB, this, _1),
                        boost::bind(&Physics::setFixedTimeStepCB, this, _1));

    AV_FC_ADD_FIELD(MaxSimTime, ::gua::physics::physics_default_max_sim_time);

    AV_FC_ADD_ADAPTOR_FIELD(ReduceSimulationRate,
                        boost::bind(&Physics::getReduceSimulationRateCB, this, _1),
                        boost::bind(&Physics::setReduceSimulationRateCB, this, _1));

}

av::gua::Physics::~Physics()
{}

void
av::gua::Physics::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Physics, true);

        SFPhysics::initClass("av::gua::SFPhysics", "av::Field");
        MFPhysics::initClass("av::gua::MFPhysics", "av::Field");
    }
}

void
av::gua::Physics::synchronize(bool autoStart) {
    m_guaPhysics->synchronize(autoStart);
}


void
av::gua::Physics::addRigidBody(Link<RigidBodyNode> const& rigidBody)
{
    m_guaPhysics->add_rigid_body(rigidBody->getGuaNode(),
                                 static_cast<
                                 ::gua::physics::RigidBodyNode::CollisionFilterGroups>
                                    (rigidBody->Group.getValue()),
                                 static_cast<
                                 ::gua::physics::RigidBodyNode::CollisionFilterGroups>
                                    (rigidBody->Mask.getValue()));
}

void
av::gua::Physics::removeRigidBody(Link<RigidBodyNode> const& rigidBody)
{
    m_guaPhysics->remove_rigid_body(rigidBody->getGuaNode());
}

void
av::gua::Physics::addConstraint(Link<Constraint> const& constraint)
{
    m_guaPhysics->add_constraint(constraint->getGuaConstraint(),
                                 constraint->DisableCollisionBetweenLinkedBodies.getValue());
}

void
av::gua::Physics::removeConstraint(Link<Constraint> const& constraint)
{
    m_guaPhysics->remove_constraint(constraint->getGuaConstraint());
}

float
av::gua::Physics::getPhysicsFPS() const
{
    return m_guaPhysics->get_physics_fps();
}

::gua::physics::Physics*
av::gua::Physics::getGuaPhysics() const
{
    return m_guaPhysics;
}

void
av::gua::Physics::getStateCB(const SFInt::GetValueEvent& event)
{
    *(event.getValuePtr()) = static_cast<int>(m_state);
}

void
av::gua::Physics::setStateCB(const SFInt::SetValueEvent& event)
{
    RunningState new_state(static_cast<RunningState>(event.getValue()));

    switch (m_state) {
        case RunningState::RUNNING:
            if (new_state == RunningState::STOPPED)
                m_guaPhysics->stop_simulation();
            else if (new_state == RunningState::PAUSED)
                m_guaPhysics->pause(true);
            break;
        case RunningState::PAUSED:
            if (new_state == RunningState::RUNNING)
                m_guaPhysics->pause(false);
            else if (new_state == RunningState::STOPPED)
                m_guaPhysics->stop_simulation();
            break;
        case RunningState::STOPPED:
            if (new_state == RunningState::RUNNING)
                m_guaPhysics->start_simulation();
            break;
        default: break;
    }

    m_state = new_state;
}

void
av::gua::Physics::getGravityCB(const SFVec3::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaPhysics->get_gravity();
}

void
av::gua::Physics::setGravityCB(const SFVec3::SetValueEvent& event)
{
   m_guaPhysics->set_gravity(event.getValue());
}

void
av::gua::Physics::getFixedTimeStepCB(const SFFloat::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_guaPhysics->get_fixed_timestep();
}

void
av::gua::Physics::setFixedTimeStepCB(const SFFloat::SetValueEvent& event)
{
   m_guaPhysics->set_fixed_timestep(event.getValue());
}

void
av::gua::Physics::getReduceSimulationRateCB(const SFBool::GetValueEvent& event)
{
    *(event.getValuePtr()) = m_simulationRateReduced;
}

void
av::gua::Physics::setReduceSimulationRateCB(const SFBool::SetValueEvent& event)
{
   m_guaPhysics->simulation_rate_reduction(event.getValue(), MaxSimTime.getValue());
   m_simulationRateReduced = event.getValue();
}
