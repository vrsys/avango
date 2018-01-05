#ifndef AVANGO_GUA_LINE_STRIP_VERTEX_HPP
#define AVANGO_GUA_LINE_STRIP_VERTEX_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/utils/LineStrip.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

#include <vector>
namespace av
{
  namespace gua
  {
    class NetTransform;
    /**
     * Wrapper for ::gua::LineStrip::Vertex
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LineStripVertex : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::LineStrip::Vertex is created.
       * Otherwise, the given ::gua::LineStrip::Vertex is used.
       */
      LineStripVertex();

      virtual ~LineStripVertex();

    public:
/*
      SFVec3 Position;
      SFVec3 Color;
      SFFloat Thickness;
      SFVec3 Normal;*/
      std::vector<::gua::LineStrip::Vertex> queued_vertices;
    public:

    void pushLineStripVertex(::gua::math::vec3f const& v_position,
                             ::gua::math::vec3f const& v_color,
                             float const& thickness);
/*
      virtual void getPositionCB(const SFVec3::GetValueEvent& event);
      virtual void setPositionCB(const SFVec3::SetValueEvent& event);

      virtual void getColorCB(const SFVec3::GetValueEvent& event);
      virtual void setColorCB(const SFVec3::SetValueEvent& event);

      virtual void getThicknessCB(const SFFloat::GetValueEvent& event);
      virtual void setThicknessCB(const SFFloat::SetValueEvent& event);

      ::gua::LineStrip::Vertex* getGuaLineStripVertex() const;

      bool operator==(av::gua::LineStripVertex const& rhs) const {
        return *m_guaLineStripVertex == *rhs.getGuaLineStripVertex();
      }

      bool operator!=(av::gua::LineStripVertex const& rhs) const {
        return *m_guaLineStripVertex != *rhs.getGuaLineStripVertex();
      }
*/
    private:


      //::gua::LineStrip::Vertex* m_guaLineStripVertex;

      LineStripVertex& operator=(const LineStripVertex&);
    };

    using SFLineStripVertex = SingleField<Link<LineStripVertex> >;
    using MFLineStripVertex = MultiField<Link<LineStripVertex> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LineStripVertex> >;
  template class AV_GUA_DLL MultiField<Link<gua::LineStripVertex> >;
#endif

}

#endif //AVANGO_GUA_LINE_STRIP_VERTEX_HPP
