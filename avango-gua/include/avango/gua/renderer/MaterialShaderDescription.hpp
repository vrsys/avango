#ifndef AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP
#define AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/MaterialShaderDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/MaterialShaderMethod.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    class NetTransform;
    /**
     * Wrapper for ::gua::MaterialShaderDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL MaterialShaderDescription : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::MaterialShaderDescription is created.
       * Otherwise, the given ::gua::MaterialShaderDescription is used.
       */
      MaterialShaderDescription(std::shared_ptr< ::gua::MaterialShaderDescription> const& guaMaterialShaderDescription =
                                std::shared_ptr< ::gua::MaterialShaderDescription>(new ::gua::MaterialShaderDescription()));

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

    public:

      MFMaterialShaderMethod VertexMethods;
      MFMaterialShaderMethod FragmentMethods;

      virtual void getVertexMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event);
      virtual void setVertexMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event);

      virtual void getFragmentMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event);
      virtual void setFragmentMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event);

      void load_from_file(std::string const& file);
      void load_from_json(std::string const& json);
      /**
       * Get the wrapped ::gua::MaterialShaderDescription.
       */
      std::shared_ptr< ::gua::MaterialShaderDescription> const& getGuaMaterialShaderDescription() const;

    private:

      std::shared_ptr< ::gua::MaterialShaderDescription> m_guaMaterialShaderDescription;

      MFMaterialShaderMethod::ContainerType m_vertexMethods;
      MFMaterialShaderMethod::ContainerType m_fragmentMethods;


      // MaterialShaderDescription(const MaterialShaderDescription&);
      MaterialShaderDescription& operator=(const MaterialShaderDescription&);
    };

    using SFMaterialShaderDescription = SingleField<Link<MaterialShaderDescription> >;
    using MFMaterialShaderDescription = MultiField<Link<MaterialShaderDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::MaterialShaderDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::MaterialShaderDescription> >;
#endif

}

#endif //AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP
