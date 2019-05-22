#ifndef AVANGO_GUA_CONVEX_HULL_SHAPE_HPP
#define AVANGO_GUA_CONVEX_HULL_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/ConvexHullShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::ConvexHullShape
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL ConvexHullShape : public av::gua::CollisionShape
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::physics::ConvexHullShape is created.
     * Otherwise, the given ::gua::physics::ConvexHullShape is used.
     */
    ConvexHullShape(::gua::physics::ConvexHullShape* guashape = new ::gua::physics::ConvexHullShape()); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~ConvexHullShape();

  public:
    /**
     * Get the wrapped ::gua::physics::CollisionShape.
     */
    ::gua::physics::ConvexHullShape* getGuaShape() const;

  private:
    ::gua::physics::ConvexHullShape* m_guaShape;

    ConvexHullShape(const ConvexHullShape&);
    ConvexHullShape& operator=(const ConvexHullShape&);
};

using SFConvexHullShape = SingleField<Link<ConvexHullShape>>;
using MFConvexHullShape = MultiField<Link<ConvexHullShape>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::ConvexHullShape>>;
template class AV_GUA_DLL MultiField<Link<gua::ConvexHullShape>>;
#endif

} // namespace av

#endif // AVANGO_GUA_CONVEX_HULL_SHAPE_HPP
