#ifndef AVANGO_GUA_FIELDS_HPP
#define AVANGO_GUA_FIELDS_HPP

/**
 * \file
 * \ingroup av_gua
 */

//#include <avango/gua/Types.hpp>
#include <avango/gua/windows_specific_gua.hpp>
#include <avango/SingleField.h>
#include <avango/MultiField.h>
#include <gua/utils/Color3f.hpp>
#include <gua/scenegraph/TransformNode.hpp>

namespace av
{
  namespace gua
  {
    void AV_GUA_DLL initFields();

    typedef SingleField< ::gua::math::mat4> SFMatrix4;
    typedef MultiField< ::gua::math::mat4> MFMatrix4;
    typedef SingleField< ::gua::math::mat3> SFMatrix3;
    typedef MultiField< ::gua::math::mat3> MFMatrix3;

    typedef SingleField< ::gua::math::vec4> SFVec4;
    typedef MultiField< ::gua::math::vec4> MFVec4;
    typedef SingleField< ::gua::math::vec3> SFVec3;
    typedef MultiField< ::gua::math::vec3> MFVec3;
    typedef SingleField< ::gua::math::vec2> SFVec2;
    typedef MultiField< ::gua::math::vec2> MFVec2;

    typedef SingleField< ::gua::math::vec4i> SFVec4i;
    typedef MultiField< ::gua::math::vec4i> MFVec4i;
    typedef SingleField< ::gua::math::vec3i> SFVec3i;
    typedef MultiField< ::gua::math::vec3i> MFVec3i;
    typedef SingleField< ::gua::math::vec2i> SFVec2i;
    typedef MultiField< ::gua::math::vec2i> MFVec2i;

    typedef SingleField< ::gua::math::vec4ui> SFVec4ui;
    typedef MultiField< ::gua::math::vec4ui> MFVec4ui;
    typedef SingleField< ::gua::math::vec3ui> SFVec3ui;
    typedef MultiField< ::gua::math::vec3ui> MFVec3ui;
    typedef SingleField< ::gua::math::vec2ui> SFVec2ui;
    typedef MultiField< ::gua::math::vec2ui> MFVec2ui;

    typedef SFMatrix4 SFMatrix;
    typedef MFMatrix4 MFMatrix;
    typedef SingleField< ::gua::utils::Color3f> SFColor;
    typedef MultiField< ::gua::utils::Color3f> MFColor;

  }
#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField< ::gua::math::mat4>;
  template class AV_GUA_DLL MultiField< ::gua::math::mat4>;
  template class AV_GUA_DLL SingleField< ::gua::math::mat3>;
  template class AV_GUA_DLL MultiField< ::gua::math::mat3>;

  template class AV_GUA_DLL SingleField< ::gua::math::vec4>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec4>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec3>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec3>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec2>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec2>;

  template class AV_GUA_DLL SingleField< ::gua::math::vec4i>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec4i>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec3i>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec3i>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec2i>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec2i>;

  template class AV_GUA_DLL SingleField< ::gua::math::vec4ui>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec4ui>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec3ui>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec3ui>;
  template class AV_GUA_DLL SingleField< ::gua::math::vec2ui>;
  template class AV_GUA_DLL MultiField< ::gua::math::vec2ui>;
  template class AV_GUA_DLL SingleField< ::gua::utils::Color3f>;
//  template class AV_GUA_DLL MultiField< ::gua::utils::Color3f>;
#endif
}

#endif // AVANGO_GUA_FIELDS_HPP
