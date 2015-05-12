#ifndef AVANGO_GUA_TEXTURE2D_HPP
#define AVANGO_GUA_TEXTURE2D_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Texture2D.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Texture2D
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Texture2D : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Texture2D is
       * created.  Otherwise, the given ::gua::Texture2D is used.
       */
      Texture2D(std::shared_ptr< ::gua::Texture2D> const& Texture2D =
                    std::shared_ptr< ::gua::Texture2D>(new ::gua::Texture2D()));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      //virtual ~Texture2D() {}

    public:

      SFString Bytes;

      SFInt Width;
      SFInt Height;

      /**
       * Get the wrapped ::gua::Texture2D object.
       * \return a shared pointer to the texture object
       */
      std::shared_ptr< ::gua::Texture2D> const& getGuaTexture2D() const;

    private:

      std::shared_ptr< ::gua::Texture2D> m_guaTexture2D;

      Texture2D(const Texture2D&);
      Texture2D& operator=(const Texture2D&);

    protected:

      virtual void getBytesCB(const av::SFString::GetValueEvent& event);
      virtual void setBytesCB(const av::SFString::SetValueEvent& event);
      virtual void getWidthCB(const av::SFInt::GetValueEvent& event);
      virtual void setWidthCB(const av::SFInt::SetValueEvent& event);
      virtual void getHeightCB(const av::SFInt::GetValueEvent& event);
      virtual void setHeightCB(const av::SFInt::SetValueEvent& event);

    };

    typedef SingleField<Link<Texture2D> > SFTexture2D;
    typedef MultiField<Link<Texture2D> > MFTexture2D;
  } // namespace gua

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Texture2D> >;
  template class AV_GUA_DLL MultiField<Link<gua::Texture2D> >;
#endif

} // namespace av

#endif //AVANGO_GUA_TEXTURE2D_HPP
