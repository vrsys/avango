#ifndef AVANGO_GUA_DYNAMIC_TRIANGLE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_DYNAMIC_TRIANGLE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/DynamicTrianglePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::DynamicTrianglePassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL DynamicTrianglePassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::DynamicTrianglePassDescription is created.
       * Otherwise, the given ::gua::DynamicTrianglePassDescription is used.
       */
      DynamicTrianglePassDescription(std::shared_ptr< ::gua::DynamicTrianglePassDescription> const& DynamicTrianglePassDescription =
                             std::shared_ptr< ::gua::DynamicTrianglePassDescription>(new ::gua::DynamicTrianglePassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::DynamicTrianglePassDescription.
       */
      std::shared_ptr< ::gua::DynamicTrianglePassDescription> const& getGuaDynamicTrianglePassDescription() const;

    private:

      std::shared_ptr< ::gua::DynamicTrianglePassDescription> m_guaDynamicTrianglePassDescription;


      DynamicTrianglePassDescription(const DynamicTrianglePassDescription&);
      DynamicTrianglePassDescription& operator=(const DynamicTrianglePassDescription&);
    };

    using SFDynamicTrianglePassDescription = SingleField<Link<DynamicTrianglePassDescription> >;
    using MFDynamicTrianglePassDescription = MultiField<Link<DynamicTrianglePassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DynamicTrianglePassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::DynamicTrianglePassDescription> >;
#endif

}

#endif //AVANGO_GUA_DYNAMIC_TRIANGLE_PASS_DESCRIPTION_HPP
