
#ifndef AVANGO_GUA_VTBACKEND_HPP
#define AVANGO_GUA_VTBACKEND_HPP

#include <gua/guacamole.hpp>
#include <gua/platform.hpp>



#include <avango/FieldContainer.h>
#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>

#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/windows_specific_gua.hpp>
#include <gua/virtual_texturing/VTBackend.hpp>

#ifdef AVANGO_VIRTUAL_TEXTURING_SUPPORT

namespace av
{
namespace gua
{
class AV_GUA_DLL VTBackend : public av::FieldContainer
{
    AV_FC_DECLARE();

  public:
    VTBackend();
    ~VTBackend();

    const void start_backend() const;
    const void stop_backend() const;
    const void add_camera(const av::gua::CameraNode& camera) const;

  private:
    VTBackend(const VTBackend&);
    VTBackend& operator=(const VTBackend&);
};

using SFVTBackend = SingleField<Link<VTBackend>>;
using MFVTBackend = MultiField<Link<VTBackend>>;
} // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<Link<gua::VTBackend>>;
template class AV_GUA_DLL MultiField<Link<gua::VTBackend>>;
#endif
} // namespace av

#endif // AVANGO_GUA_VTBACKEND_HPP
#endif // AVANGO_VIRTUAL_TEXTURING_SUPPORT
