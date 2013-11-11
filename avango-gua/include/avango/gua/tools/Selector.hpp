#ifndef AVANGO_GUA_SELECTOR_HPP
#define AVANGO_GUA_SELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/FieldContainer.h>
#include <avango/gua/tools/TargetHolder.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Abstract selector base class
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL Selector : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor.
       */
      Selector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Selector();

    public:

      /**
       * This multifield outputs which targets are currently selected by the Selector.
       */
      MFTargetHolder SelectedTargets;

      Link<TargetHolder> combine(const Link<TargetHolder> holder1, const Link<TargetHolder> holder2);
    };

    typedef SingleField<Link<Selector> > SFSelector;
    typedef MultiField<Link<Selector> > MFSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Selector> >;
  template class AV_GUA_DLL MultiField<Link<gua::Selector> >;
#endif

}

#endif //AVANGO_GUA_SELECTOR_HPP
