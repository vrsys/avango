#ifndef AVANGO_GUA_FIXED_CONSTRAINT_HPP
#define AVANGO_GUA_FIXED_CONSTRAINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/FixedConstraint.hpp>

#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::FixedConstraint
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL FixedConstraint : public av::gua::Constraint
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::physics::FixedConstraint is created.
     * Otherwise, the given ::gua::physics::FixedConstraint is used.
     */
    FixedConstraint(::gua::physics::FixedConstraint* guaconstraint = new ::gua::physics::FixedConstraint(nullptr,
                                                                                                         nullptr,
                                                                                                         ::gua::math::mat4::identity(),
                                                                                                         ::gua::math::mat4::identity())); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~FixedConstraint();

  public:
    SFMatrix4 FrameA;
    SFMatrix4 FrameB;
    SFBool LockRotation;
    SFBool LockTranslation;

    /**
     * Get the wrapped ::gua::physics::FixedConstraint.
     */
    ::gua::physics::FixedConstraint* getGuaConstraint() const;

  public:
    virtual void getFrameACB(const SFMatrix4::GetValueEvent& event);
    virtual void setFrameACB(const SFMatrix4::SetValueEvent& event);

    virtual void getFrameBCB(const SFMatrix4::GetValueEvent& event);
    virtual void setFrameBCB(const SFMatrix4::SetValueEvent& event);

    virtual void getLockRotationCB(const SFBool::GetValueEvent& event);
    virtual void setLockRotationCB(const SFBool::SetValueEvent& event);

    virtual void getLockTranslationCB(const SFBool::GetValueEvent& event);
    virtual void setLockTranslationCB(const SFBool::SetValueEvent& event);

  private:
    ::gua::physics::FixedConstraint* m_guaConstraint;

    FixedConstraint(const FixedConstraint&);
    FixedConstraint& operator=(const FixedConstraint&);
};

using SFFixedConstraint = SingleField<Link<FixedConstraint>>;
using MFFixedConstraint = MultiField<Link<FixedConstraint>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::FixedConstraint>>;
template class AV_GUA_DLL MultiField<Link<gua::FixedConstraint>>;
#endif

} // namespace av

#endif // AVANGO_GUA_FIXED_CONSTRAINT_HPP
