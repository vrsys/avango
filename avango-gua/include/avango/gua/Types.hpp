#ifndef AVANGO_GUA_TYPES_H
#define AVANGO_GUA_TYPES_H

#include <gua/math/math.hpp>
#include <gua/utils/Color3f.hpp>
#include <gua/physics.hpp>

#include <avango/gua/windows_specific_gua.hpp>
#include <avango/Config.h>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  #include <avango/Msg.h>
#endif //#if defined(AVANGO_DISTRIBUTION_SUPPORT)

namespace av
{
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat4&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat4&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat3&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat3&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::quat&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::quat&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::utils::Color3f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::utils::Color3f&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::physics::CollisionShape&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::physics::CollisionShape&);
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::physics::Constraint&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::physics::Constraint&);

  //AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::Camera&);
  //AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::Camera&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::RayNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::RayNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::ScreenNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::ScreenNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::TexturedQuadNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::TexturedQuadNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::GeometryNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::GeometryNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::SceneGraph&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::SceneGraph&);


#if defined(AVANGO_DISTRIBUTION_SUPPORT)

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat4& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat3& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat3& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::quat& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::quat& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::utils::Color3f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::utils::Color3f& buf);

#endif // AVANGO_DISTRIBUTION_SUPPORT
}

#endif //AVANGO_GUA_TYPES_H
