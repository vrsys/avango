#ifndef AVANGO_GUA_DEPTH_MAP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_DEPTH_MAP_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/DepthCubeMapPass.hpp>

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
    class AV_GUA_DLL DepthMapPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TriMeshPassDescription is created.
       * Otherwise, the given ::gua::TriMeshPassDescription is used.
       */
      DepthMapPassDescription(std::shared_ptr< ::gua::DepthCubeMapPassDesciption> const& DepthCubeMapPassDesciption =
                             std::shared_ptr< ::gua::DepthCubeMapPassDesciption>(new ::gua::DepthCubeMapPassDesciption()) );



    public:

      /**
       * Get the wrapped ::gua::TriMeshPassDescription.
       */
      std::shared_ptr< ::gua::DepthCubeMapPassDesciption> const& getGuaDepthMapPassDescription() const;

    private:

      std::shared_ptr< ::gua::DepthCubeMapPassDesciption> m_guaDepthMapPassDescription;


      DepthMapPassDescription(const DepthMapPassDescription&);
      DepthMapPassDescription& operator=(const DepthMapPassDescription&);
    };

    using SFDepthMapPassDescription = SingleField<Link<DepthMapPassDescription> >;
    using MFDepthMapPassDescription = MultiField<Link<DepthMapPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DepthMapPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::DepthMapPassDescription> >;
#endif

}

#endif //AVANGO_GUA_DEPTH_MAP_PASS_DESCRIPTION_HPP
