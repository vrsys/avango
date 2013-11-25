#ifndef AVANGO_TOOLS_SELECTOR_HPP
#define AVANGO_TOOLS_SELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/FieldContainer.h>
#include <avango/tools/TargetHolder.hpp>

namespace av
{
  namespace tools
  {
    /**
     * Abstract selector base class
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL Selector : public av::FieldContainer
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
  template class AV_TOOLS_DLL SingleField<Link<tools::Selector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::Selector> >;
#endif

}

#endif //AVANGO_TOOLS_SELECTOR_HPP
