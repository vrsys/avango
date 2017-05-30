#ifndef AVANGO_GUA_TYPES_H
#define AVANGO_GUA_TYPES_H

#include <avango/Config.h>
#include <avango/gua/windows_specific_gua.hpp>
#include <avango/InputStream.h>
#include <avango/OutputStream.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  #include <avango/Msg.h>
#endif //#if defined(AVANGO_DISTRIBUTION_SUPPORT)

#include <gua/math/math.hpp>
#include <gua/utils/Color3f.hpp>
#if defined(AVANGO_PHYSICS_SUPPORT)
#include <gua/physics.hpp>
#endif

namespace gua
{
  class TriMeshNode;
  class CubemapNode;
  class SkeletalAnimationNode;
#if defined(AVANGO_VIDEO3D_SUPPORT)
  class Video3DNode;
#endif
  class VolumeNode;
  class NURBSNode;
  class SceneGraph;
  class VolumeNode;
  class TexturedQuadNode;
  class RayNode;
  class ScreenNode;
#if defined(AVANGO_PBR_SUPPORT)
  class PBRNode;
  class PLODNode;
#endif //if defined(AVANGO_PBR_SUPPORT)
#if defined(AVANGO_TV_3_SUPPORT)
  class TV_3Node;
#endif //if defined(AVANGO_TV_3_SUPPORT)
}

namespace av
{
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat4d&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat4d&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat4f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat4f&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat3d&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat3d&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::mat3f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::mat3f&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::quatd&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::quatd&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::quatf&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::quatf&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4d&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4d&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4f&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec4ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec4ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3d&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3d&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3f&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec3ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec3ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2d&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2d&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2f&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2i&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2i&);
  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::math::vec2ui&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::math::vec2ui&);

  AV_GUA_DLL OutputStream& operator<<(OutputStream&, const ::gua::utils::Color3f&);
  AV_GUA_DLL InputStream& operator>>(InputStream&, ::gua::utils::Color3f&);

#if defined(AVANGO_PHYSICS_SUPPORT)
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::physics::CollisionShape&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::physics::CollisionShape&);
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::physics::Constraint&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::physics::Constraint&);
#endif

  //AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::Camera&);
  //AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::Camera&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::RayNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::RayNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::ScreenNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::ScreenNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::TexturedQuadNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::TexturedQuadNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::TriMeshNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::TriMeshNode&);
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::CubemapNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::CubemapNode&);
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::SkeletalAnimationNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::SkeletalAnimationNode&);
#if defined(AVANGO_VIDEO3D_SUPPORT)
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::Video3DNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::Video3DNode&);
#endif
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::VolumeNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::VolumeNode&);

#if defined(AVANGO_PBR_SUPPORT)
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::PBRNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::PBRNode&);
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::PLODNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::PLODNode&);
#endif //#if defined(AVANGO_PBR_SUPPORT)

#if defined(AVANGO_TV_3_SUPPORT)
  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::TV_3Node&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::TV_3Node&);
#endif //#if defined(AVANGO_TV_3_SUPPORT)

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::NURBSNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::NURBSNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::VolumeNode&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::VolumeNode&);

  AV_GUA_DLL av::OutputStream& operator<<(OutputStream& os, const ::gua::SceneGraph&);
  AV_GUA_DLL av::InputStream& operator>>(InputStream& is, ::gua::SceneGraph&);

} // namespace av

namespace av {

  // these functions need to be put into the ::gua namespace,
  // as they are really a part of the interface for the corresponding GUA types.
  // ADL (Argument Dependant Loopup) takes care that these functions are found in the right namespace
  // This makes these functions visible to SIngleField<T> and MultiField<T> at the point of template instantiation,
  // not just definition ( see C++ ISO Standard 14882 from '98; Chapter 14.6.4 )
#if defined(AVANGO_DISTRIBUTION_SUPPORT)

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4d& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat4d& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat4f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat4f& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat3d& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat3d& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::mat3f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::mat3f& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::quatd& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::quatd& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::quatf& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::quatf& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4d& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4d& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4f& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec4ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec4ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3d& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3d& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3f& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec3ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec3ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2d& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2d& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2f& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2i& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2i& buf);
  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::math::vec2ui& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::math::vec2ui& buf);

  AV_GUA_DLL void av_pushMsg(av::Msg& netMsg, const ::gua::utils::Color3f& buf);
  AV_GUA_DLL void av_popMsg(av::Msg& netMsg, ::gua::utils::Color3f& buf);

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)
}

#endif // AVANGO_GUA_TYPES_H
