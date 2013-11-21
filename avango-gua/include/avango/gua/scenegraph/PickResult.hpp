#ifndef AVANGO_GUA_PICK_RESULT_HPP
#define AVANGO_GUA_PICK_RESULT_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/scenegraph/PickResult.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/scenegraph/Node.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PickResult
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PickResult : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Options {
        PICK_ALL = ::gua::PickResult::PICK_ALL,
        PICK_ONLY_FIRST_OBJECT = ::gua::PickResult::PICK_ONLY_FIRST_OBJECT,
        PICK_ONLY_FIRST_FACE = ::gua::PickResult::PICK_ONLY_FIRST_FACE,
        GET_POSITIONS = ::gua::PickResult::GET_POSITIONS,
        GET_WORLD_POSITIONS = ::gua::PickResult::GET_WORLD_POSITIONS,
        GET_NORMALS = ::gua::PickResult::GET_NORMALS,
        GET_WORLD_NORMALS = ::gua::PickResult::GET_WORLD_NORMALS,
        INTERPOLATE_NORMALS = ::gua::PickResult::INTERPOLATE_NORMALS,
        GET_TEXTURE_COORDS = ::gua::PickResult::GET_TEXTURE_COORDS
      };

      /**
       * Constructor. When called without arguments, a new ::gua::PickResult is created.
       * Otherwise, the given ::gua::PickResult is used.
       */
      PickResult(::gua::PickResult const& result =
                  ::gua::PickResult(0.0, nullptr,
                                    ::gua::math::vec3(), ::gua::math::vec3(),
                                    ::gua::math::vec3(), ::gua::math::vec3(),
                                    ::gua::math::vec2()));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PickResult();

    public:

      SFFloat Distance;
      SFNode  Object;
      SFVec3  Position;
      SFVec3  WorldPosition;
      SFVec3  Normal;
      SFVec3  WorldNormal;
      SFVec2  TextureCoords;

    private:

      PickResult(const PickResult&);
      PickResult& operator=(const PickResult&);
    };

    typedef SingleField<Link<PickResult> > SFPickResult;
    typedef MultiField<Link<PickResult> > MFPickResult;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PickResult> >;
  template class AV_GUA_DLL MultiField<Link<gua::PickResult> >;
#endif

}

#endif //AVANGO_GUA_PICK_RESULT_HPP
