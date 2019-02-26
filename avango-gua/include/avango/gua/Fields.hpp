#ifndef AVANGO_GUA_FIELDS_HPP
#define AVANGO_GUA_FIELDS_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/gua/Types.hpp>
#include <avango/gua/windows_specific_gua.hpp>
#include <avango/SingleField.h>
#include <avango/MultiField.h>
#include <gua/utils/Color3f.hpp>
#include <gua/math.hpp>

namespace av
{
namespace gua
{
void AV_GUA_DLL initFields();

using SFMatrix4 = SingleField<::gua::math::mat4>;
using MFMatrix4 = MultiField<::gua::math::mat4>;
using SFMatrix3 = SingleField<::gua::math::mat3>;
using MFMatrix3 = MultiField<::gua::math::mat3>;

using MFQuat = MultiField<::gua::math::quat>;
using SFQuat = SingleField<::gua::math::quat>;

using SFVec4 = SingleField<::gua::math::vec4>;
using MFVec4 = MultiField<::gua::math::vec4>;
using SFVec3 = SingleField<::gua::math::vec3>;
using MFVec3 = MultiField<::gua::math::vec3>;
using SFVec2 = SingleField<::gua::math::vec2>;
using MFVec2 = MultiField<::gua::math::vec2>;

using SFVec4i = SingleField<::gua::math::vec4i>;
using MFVec4i = MultiField<::gua::math::vec4i>;
using SFVec3i = SingleField<::gua::math::vec3i>;
using MFVec3i = MultiField<::gua::math::vec3i>;
using SFVec2i = SingleField<::gua::math::vec2i>;
using MFVec2i = MultiField<::gua::math::vec2i>;

using SFVec4ui = SingleField<::gua::math::vec4ui>;
using MFVec4ui = MultiField<::gua::math::vec4ui>;
using SFVec3ui = SingleField<::gua::math::vec3ui>;
using MFVec3ui = MultiField<::gua::math::vec3ui>;
using SFVec2ui = SingleField<::gua::math::vec2ui>;
using MFVec2ui = MultiField<::gua::math::vec2ui>;

using SFMatrix = SFMatrix4;
using MFMatrix = MFMatrix4;
using SFColor = SingleField<::gua::utils::Color3f>;
using MFColor = MultiField<::gua::utils::Color3f>;

} // namespace gua
#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_GUA_DLL SingleField<::gua::math::mat4>;
template class AV_GUA_DLL MultiField<::gua::math::mat4>;
template class AV_GUA_DLL SingleField<::gua::math::mat3>;
template class AV_GUA_DLL MultiField<::gua::math::mat3>;

template class AV_GUA_DLL MultiField<::gua::math::quat>;
template class AV_GUA_DLL SingleField<::gua::math::quat>;

template class AV_GUA_DLL SingleField<::gua::math::vec4>;
template class AV_GUA_DLL MultiField<::gua::math::vec4>;
template class AV_GUA_DLL SingleField<::gua::math::vec3>;
template class AV_GUA_DLL MultiField<::gua::math::vec3>;
template class AV_GUA_DLL SingleField<::gua::math::vec2>;
template class AV_GUA_DLL MultiField<::gua::math::vec2>;

template class AV_GUA_DLL SingleField<::gua::math::vec4i>;
template class AV_GUA_DLL MultiField<::gua::math::vec4i>;
template class AV_GUA_DLL SingleField<::gua::math::vec3i>;
template class AV_GUA_DLL MultiField<::gua::math::vec3i>;
template class AV_GUA_DLL SingleField<::gua::math::vec2i>;
template class AV_GUA_DLL MultiField<::gua::math::vec2i>;

template class AV_GUA_DLL SingleField<::gua::math::vec4ui>;
template class AV_GUA_DLL MultiField<::gua::math::vec4ui>;
template class AV_GUA_DLL SingleField<::gua::math::vec3ui>;
template class AV_GUA_DLL MultiField<::gua::math::vec3ui>;
template class AV_GUA_DLL SingleField<::gua::math::vec2ui>;
template class AV_GUA_DLL MultiField<::gua::math::vec2ui>;
template class AV_GUA_DLL SingleField<::gua::utils::Color3f>;
//  template class AV_GUA_DLL MultiField< ::gua::utils::Color3f>;
#endif
} // namespace av

#endif // AVANGO_GUA_FIELDS_HPP
