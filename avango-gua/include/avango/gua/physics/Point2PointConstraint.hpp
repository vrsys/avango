#ifndef AVANGO_GUA_POINT_2_POINT_CONSTRAINT_HPP
#define AVANGO_GUA_POINT_2_POINT_CONSTRAINT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/Point2PointConstraint.hpp>

#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::Point2PointConstraint
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Point2PointConstraint : public av::gua::Constraint
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::physics::Point2PointConstraint is created.
     * Otherwise, the given ::gua::physics::Point2PointConstraint is used.
     */
    Point2PointConstraint(::gua::physics::Point2PointConstraint* guaconstraint = new ::gua::physics::Point2PointConstraint(nullptr,
                                                                                                                           nullptr,
                                                                                                                           ::gua::math::vec3(),
                                                                                                                           ::gua::math::vec3())); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~Point2PointConstraint();

  public:
    SFVec3 PivotA;
    SFVec3 PivotB;

    /**
     * Get the wrapped ::gua::physics::Point2PointConstraint.
     */
    ::gua::physics::Point2PointConstraint* getGuaConstraint() const;

  public:
    virtual void getPivotACB(const SFVec3::GetValueEvent& event);
    virtual void setPivotACB(const SFVec3::SetValueEvent& event);

    virtual void getPivotBCB(const SFVec3::GetValueEvent& event);
    virtual void setPivotBCB(const SFVec3::SetValueEvent& event);

  private:
    ::gua::physics::Point2PointConstraint* m_guaConstraint;

    Point2PointConstraint(const Point2PointConstraint&);
    Point2PointConstraint& operator=(const Point2PointConstraint&);
};

using SFPoint2PointConstraint = SingleField<Link<Point2PointConstraint>>;
using MFPoint2PointConstraint = MultiField<Link<Point2PointConstraint>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::Point2PointConstraint>>;
template class AV_GUA_DLL MultiField<Link<gua::Point2PointConstraint>>;
#endif

} // namespace av

#endif // AVANGO_GUA_POINT_2_POINT_CONSTRAINT_HPP
