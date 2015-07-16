#ifndef AVANGO_GUA_CONSTRAINT_HPP
#define AVANGO_GUA_CONSTRAINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/Constraint.hpp>

#include <avango/gua/physics/RigidBodyNode.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::physics::Constraint
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Constraint : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::physics::Constraint is created.
       * Otherwise, the given ::gua::physics::Constraint is used.
       */
      Constraint(::gua::physics::Constraint* guaconstraint); // use defined type to circumvent compiler bug in VS8

      virtual ~Constraint();

    public:

      SFRigidBodyNode BodyA;
      SFRigidBodyNode BodyB;
      SFBool          Enabled;
      SFFloat         BreakingImpulseThreshold;
      SFBool          DisableCollisionBetweenLinkedBodies;


      /**
       * Get the wrapped ::gua::physics::Constraint.
       */
      ::gua::physics::Constraint* getGuaConstraint() const;

    public:

      virtual void getBodyACB(const SFRigidBodyNode::GetValueEvent& event);
      virtual void setBodyACB(const SFRigidBodyNode::SetValueEvent& event);

      virtual void getBodyBCB(const SFRigidBodyNode::GetValueEvent& event);
      virtual void setBodyBCB(const SFRigidBodyNode::SetValueEvent& event);

      virtual void getEnabledCB(const SFBool::GetValueEvent& event);
      virtual void setEnabledCB(const SFBool::SetValueEvent& event);

      virtual void getBreakingImpulseThresholdCB(const SFFloat::GetValueEvent& event);
      virtual void setBreakingImpulseThresholdCB(const SFFloat::SetValueEvent& event);

    private:

      ::gua::physics::Constraint *m_guaConstraint;

      Constraint(const Constraint&);
      Constraint& operator=(const Constraint&);
    };

    using SFConstraint = SingleField<Link<Constraint> >;
    using MFConstraint = MultiField<Link<Constraint> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Constraint> >;
  template class AV_GUA_DLL MultiField<Link<gua::Constraint> >;
#endif

}

#endif //AVANGO_GUA_CONSTRAINT_HPP
