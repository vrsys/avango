#ifndef AVANGO_GUA_TRI_MESH_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_TRI_MESH_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TriMeshPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TriMeshPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TriMeshPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TriMeshPassDescription is created.
       * Otherwise, the given ::gua::TriMeshPassDescription is used.
       */
      TriMeshPassDescription(std::shared_ptr< ::gua::TriMeshPassDescription> const& TriMeshPassDescription =
                             std::shared_ptr< ::gua::TriMeshPassDescription>(new ::gua::TriMeshPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::TriMeshPassDescription.
       */
      std::shared_ptr< ::gua::TriMeshPassDescription> const& getGuaTriMeshPassDescription() const;

    private:

      std::shared_ptr< ::gua::TriMeshPassDescription> m_guaTriMeshPassDescription;


      TriMeshPassDescription(const TriMeshPassDescription&);
      TriMeshPassDescription& operator=(const TriMeshPassDescription&);
    };

    using SFTriMeshPassDescription = SingleField<Link<TriMeshPassDescription> >;
    using MFTriMeshPassDescription = MultiField<Link<TriMeshPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TriMeshPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::TriMeshPassDescription> >;
#endif

}

#endif //AVANGO_GUA_TRI_MESH_PASS_DESCRIPTION_HPP
