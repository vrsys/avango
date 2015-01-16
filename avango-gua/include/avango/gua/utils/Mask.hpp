#ifndef AVANGO_GUA_MASK_HPP
#define AVANGO_GUA_MASK_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/utils/Mask.hpp>

#include <avango/gua/utils/TagList.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Mask
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Mask : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Mask is created.
       * Otherwise, the given ::gua::Mask is used.
       */
      Mask( ::gua::Mask* guaMask = new ::gua::Mask());

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~Mask();

    public:

      SFTagList WhiteList;
      SFTagList BlackList;

      virtual void getWhiteListCB(const SFTagList::GetValueEvent& event);
      virtual void setWhiteListCB(const SFTagList::SetValueEvent& event);

      virtual void getBlackListCB(const SFTagList::GetValueEvent& event);
      virtual void setBlackListCB(const SFTagList::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::Mask.
       */
      ::gua::Mask* getGuaMask() const;

    private:

      ::gua::Mask* m_guaMask;
      av::Link< av::gua::TagList> m_whiteList;
      av::Link< av::gua::TagList> m_blackList;

      Mask(const Mask&);
      Mask& operator=(const Mask&);
    };

    typedef SingleField<Link<Mask> > SFMask;
    typedef MultiField<Link<Mask> > MFMask;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Mask> >;
  template class AV_GUA_DLL MultiField<Link<gua::Mask> >;
#endif

}

#endif //AVANGO_GUA_MASK_HPP
