#ifndef AVANGO_GUA_NURBS_SURFACE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_NURBS_SURFACE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */
#include <avango/gua/nurbs/Init.hpp>
#include <gua/renderer/NURBSPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    namespace nurbs 
    {
      /**
       * Wrapper for ::gua::NURBSPassDescription
       *
       * \ingroup av_gua
       */
      class AV_NURBS_DLL NURBSPassDescription : public av::gua::PipelinePassDescription
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::NURBSPassDescription is created.
         * Otherwise, the given ::gua::NURBSPassDescription is used.
         */
        NURBSPassDescription(std::shared_ptr< ::gua::NURBSPassDescription> const& NURBSPassDescription =
                             std::shared_ptr< ::gua::NURBSPassDescription>(new ::gua::NURBSPassDescription()) );



      public:

        /**
         * Get the wrapped ::gua::NURBSPassDescription.
         */
        std::shared_ptr< ::gua::NURBSPassDescription> const& getGuaNURBSPassDescription() const;

      private:

        std::shared_ptr< ::gua::NURBSPassDescription> m_guaNURBSPassDescription;


        NURBSPassDescription(const NURBSPassDescription&);
        NURBSPassDescription& operator=(const NURBSPassDescription&);
      };

      using SFNURBSPassDescription = SingleField<Link<NURBSPassDescription> >;
      using MFNURBSPassDescription = MultiField<Link<NURBSPassDescription> >;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_NURBS_DLL SingleField<Link<gua::nurbs::NURBSPassDescription> >;
  template class AV_NURBS_DLL MultiField<Link<gua::nurbs::NURBSPassDescription> >;
#endif

}

#endif //AVANGO_GUA_NURBS_SURFACE_PASS_DESCRIPTION_HPP
