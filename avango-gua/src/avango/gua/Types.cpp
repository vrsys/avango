#include <avango/gua/Types.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Types"));
}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::mat4&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::mat4&) {}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::mat3&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::mat3&) {}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec4&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec4&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec4i&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec4i&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec4ui&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec4ui&) {}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec3&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec3&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec3i&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec3i&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec3ui&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec3ui&) {}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec2&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec2&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec2i&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec2i&) {}
av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::math::vec2ui&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::math::vec2ui&) {}

av::OutputStream& av::operator<<(av::OutputStream&, const ::gua::utils::Color3f&) {}
av::InputStream& av::operator>>(av::InputStream&, ::gua::utils::Color3f&) {}



#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <rpc/rpc.h>
#include <avango/Msg.h>

void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::mat4& buf) {}

void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::mat3& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::mat3& buf) {}

void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec4& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4i& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec4i& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4ui& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec4ui& buf) {}

void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec3& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3i& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec3i& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3ui& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec3ui& buf) {}

void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec2& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2i& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec2i& buf) {}
void av::av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2ui& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::math::vec2ui& buf) {}

void av::av_pushMsg(av::Msg& netMsg, const ::gua::utils::Color3f& buf) {}
void av::av_popMsg(av::Msg& netMsg, ::gua::utils::Color3f& buf) {}

#endif


