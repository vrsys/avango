#ifndef AVANGO_GUA_TAG_LIST_HPP
#define AVANGO_GUA_TAG_LIST_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/utils/TagList.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TagList
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TagList : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TagList is created.
       * Otherwise, the given ::gua::TagList is used.
       */
      TagList( ::gua::utils::TagList* guaTagList = new ::gua::utils::TagList());

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
     // virtual ~TagList();

    public:

      MFString Tags;

      virtual void getTagsCB(const MFString::GetValueEvent& event);
      virtual void setTagsCB(const MFString::SetValueEvent& event);

      void print() const;

      /**
       * Get the wrapped ::gua::TagList.
       */
      ::gua::utils::TagList* getGuaTagList() const;

    private:

      ::gua::utils::TagList* m_guaTagList;

      TagList(const TagList&);
      TagList& operator=(const TagList&);
    };

    typedef SingleField<Link<TagList> > SFTagList;
    typedef MultiField<Link<TagList> > MFTagList;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TagList> >;
  template class AV_GUA_DLL MultiField<Link<gua::TagList> >;
#endif

}

#endif //AVANGO_GUA_TAG_LIST_HPP
