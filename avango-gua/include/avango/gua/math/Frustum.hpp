#ifndef AVANGO_GUA_FRUSTUM_HPP
#define AVANGO_GUA_FRUSTUM_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Frustum.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
namespace gua
{
/**
 * Wrapper for ::gua::Frustum
 *
 * \ingroup av_gua
 */
class AV_GUA_DLL Frustum : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. When called without arguments, a new ::gua::Frustum is created.
     * Otherwise, the given ::gua::Frustum is used.
     */
    Frustum(::gua::Frustum* guaFrustum = new ::gua::Frustum());

    virtual ~Frustum();

  public:
    SFDouble ClipNear;
    SFDouble ClipFar;

    SFMatrix4 CameraTransform;
    SFMatrix4 ScreenTransform;
    SFMatrix4 ViewMatrix;
    SFMatrix4 ProjectionMatrix;

    MFVec3 Corners;

  public:
    virtual void getClipNearCB(const SFDouble::GetValueEvent& event);
    virtual void setClipNearCB(const SFDouble::SetValueEvent& event);

    virtual void getClipFarCB(const SFDouble::GetValueEvent& event);
    virtual void setClipFarCB(const SFDouble::SetValueEvent& event);

    virtual void getCameraTransformCB(const SFMatrix4::GetValueEvent& event);
    virtual void setCameraTransformCB(const SFMatrix4::SetValueEvent& event);

    virtual void getScreenTransformCB(const SFMatrix4::GetValueEvent& event);
    virtual void setScreenTransformCB(const SFMatrix4::SetValueEvent& event);

    virtual void getViewMatrixCB(const SFMatrix4::GetValueEvent& event);
    virtual void setViewMatrixCB(const SFMatrix4::SetValueEvent& event);

    virtual void getProjectionMatrixCB(const SFMatrix4::GetValueEvent& event);
    virtual void setProjectionMatrixCB(const SFMatrix4::SetValueEvent& event);

    virtual void getCornersCB(const MFVec3::GetValueEvent& event);
    virtual void setCornersCB(const MFVec3::SetValueEvent& event);

    ::gua::Frustum* getGuaFrustum() const;

  private:
    ::gua::Frustum* m_guaFrustum;

    // Frustum(const Frustum&);
    Frustum& operator=(const Frustum&);
};

using SFFrustum = SingleField<Link<Frustum>>;
using MFFrustum = MultiField<Link<Frustum>>;

} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::Frustum>>;
template class AV_GUA_DLL MultiField<Link<gua::Frustum>>;
#endif

} // namespace av

#endif // AVANGO_GUA_FRUSTUM_HPP
