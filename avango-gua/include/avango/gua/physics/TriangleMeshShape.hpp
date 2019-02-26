#ifndef AVANGO_GUA_TRIANGLE_MESH_SHAPE_HPP
#define AVANGO_GUA_TRIANGLE_MESH_SHAPE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/physics/TriangleMeshShape.hpp>

#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::physics::TriangleMeshShape
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL TriangleMeshShape : public av::gua::CollisionShape
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::physics::TriangleMeshShape is created.
     * Otherwise, the given ::gua::physics::TriangleMeshShape is used.
     */
    TriangleMeshShape(::gua::physics::TriangleMeshShape* guashape = new ::gua::physics::TriangleMeshShape()); // use defined type to circumvent compiler bug in VS8

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~TriangleMeshShape();

  public:
    SFVec3 Scaling;

    /**
     * Get the wrapped ::gua::physics::CollisionShape.
     */
    ::gua::physics::TriangleMeshShape* getGuaShape() const;

  public:
    virtual void getScalingCB(const SFVec3::GetValueEvent& event);
    virtual void setScalingCB(const SFVec3::SetValueEvent& event);

  private:
    ::gua::physics::TriangleMeshShape* m_guaShape;

    TriangleMeshShape(const TriangleMeshShape&);
    TriangleMeshShape& operator=(const TriangleMeshShape&);
};

using SFTriangleMeshShape = SingleField<Link<TriangleMeshShape>>;
using MFTriangleMeshShape = MultiField<Link<TriangleMeshShape>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::TriangleMeshShape>>;
template class AV_GUA_DLL MultiField<Link<gua::TriangleMeshShape>>;
#endif

} // namespace av

#endif // AVANGO_GUA_TRIANGLE_MESH_SHAPE_HPP
