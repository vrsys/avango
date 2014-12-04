#ifndef AVANGO_GUA_MATERIAL_HPP
#define AVANGO_GUA_MATERIAL_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Material.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
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
      Material(::gua::Material const& guaMaterial = ::gua::Material());



    public:

      /**
       * Get the wrapped ::gua::Material.
       */
      ::gua::Material const& getGuaMaterial() const;

    private:

      ::gua::Material m_guaMaterial;

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
