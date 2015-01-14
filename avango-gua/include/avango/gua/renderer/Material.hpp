#ifndef AVANGO_GUA_MATERIAL_HPP
#define AVANGO_GUA_MATERIAL_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Material.hpp>

#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    class NetTransform;
    /**
     * Wrapper for ::gua::Material
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Material : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Material is created.
       * Otherwise, the given ::gua::Material is used.
       */
      Material(std::shared_ptr< ::gua::Material> const& guaMaterial =
               std::shared_ptr< ::gua::Material> (new ::gua::Material()));

      SFString ShaderName;

      virtual void getShaderNameCB(const SFString::GetValueEvent& event);
      virtual void setShaderNameCB(const SFString::SetValueEvent& event);

    public:

      template <typename T>
      void set_uniform(std::string const& name, T const& value) {
        m_guaMaterial->set_uniform(name, value);
      }

      template <typename T>
      void set_view_uniform(std::string const& name, T const& value, int view_id) {
        m_guaMaterial->set_uniform(name, value, view_id);
      }

      virtual void on_distribute(av::gua::NetTransform& netNode);
      virtual void on_undistribute(av::gua::NetTransform& netNode);

      /**
       * Get the wrapped ::gua::Material.
       */
      std::shared_ptr< ::gua::Material> const& getGuaMaterial() const;

    private:
      /*virtual*/ void fieldHasChangedLocalSideEffect(Field const& field);

      std::shared_ptr< ::gua::Material> m_guaMaterial;

      // for remote material construction
      SFMaterialShaderDescription m_materialShaderDescription;

      Material(const Material&);
      Material& operator=(const Material&);
    };

    typedef SingleField<Link<Material> > SFMaterial;
    typedef MultiField<Link<Material> > MFMaterial;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Material> >;
  template class AV_GUA_DLL MultiField<Link<gua::Material> >;
#endif

}

#endif //AVANGO_GUA_MATERIAL_HPP
