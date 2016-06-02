#ifndef AVANGO_GUA_PHYSICS_HPP
#define AVANGO_GUA_PHYSICS_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/Physics.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/physics/RigidBodyNode.hpp>
#include <avango/gua/physics/Constraint.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::Physics
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Physics : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum class RunningState {
        RUNNING,
        PAUSED,
        STOPPED
      };

      /**
       * Constructor. When called without arguments, a new ::gua::physics::Physics is created.
       * Otherwise, the given ::gua::physics::Physics is used.
       */
      Physics(::gua::physics::Physics* guaPhysics = new ::gua::physics::Physics()); // use defined type to circumvent compiler bug in VS8

      void synchronize(bool autoStart);

      void addRigidBody(RigidBodyNode const& rigidBody);
      void removeRigidBody(RigidBodyNode const& rigidBody);

      void addConstraint(Constraint const& constraint);
      void removeConstraint(Constraint const& constraint);

      float getPhysicsFPS() const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Physics();

    public:

      SFInt   State;
      SFVec3  Gravity;
      SFFloat FixedTimeStep;
      SFInt   MaxSimTime;
      SFBool  ReduceSimulationRate;

      /**
       * Get the wrapped ::gua::physics::Physics.
       */
      ::gua::physics::Physics* getGuaPhysics() const;

    public:

      virtual void getStateCB(const SFInt::GetValueEvent& event);
      virtual void setStateCB(const SFInt::SetValueEvent& event);

      virtual void getGravityCB(const SFVec3::GetValueEvent& event);
      virtual void setGravityCB(const SFVec3::SetValueEvent& event);

      virtual void getFixedTimeStepCB(const SFFloat::GetValueEvent& event);
      virtual void setFixedTimeStepCB(const SFFloat::SetValueEvent& event);

      virtual void getReduceSimulationRateCB(const SFBool::GetValueEvent& event);
      virtual void setReduceSimulationRateCB(const SFBool::SetValueEvent& event);


    private:

      ::gua::physics::Physics *m_guaPhysics;
      RunningState m_state;
      bool m_simulationRateReduced;

      Physics(const Physics&) = delete;
      Physics& operator=(const Physics&) = delete;
    };

    using SFPhysics = SingleField<Link<Physics> >;
    using MFPhysics = MultiField<Link<Physics> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Physics> >;
  template class AV_GUA_DLL MultiField<Link<gua::Physics> >;
#endif

}

#endif //AVANGO_GUA_PHYSICS_HPP
