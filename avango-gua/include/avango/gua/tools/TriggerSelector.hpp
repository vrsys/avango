#ifndef AVANGO_GUA_TRIGGERSELECTOR_HPP
#define AVANGO_GUA_TRIGGERSELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/gua/tools/Selector.hpp>
#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * TriggerSelector class passes targets if triggered.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL TriggerSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TriggerSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TriggerSelector();

    public:

      /**
       * Defines the input targets which are passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Set to true to pass Targets to SelectedTargets.
       */
      SFBool Trigger;

      /**
       * Set to true, if SelectedTargets should only be updated on Trigger transitions.
       */
      SFBool TransitionOnly;

      /* virtual */ void evaluate();

    protected:

      bool mLastTrigger;

    };

    typedef SingleField<Link<TriggerSelector> > SFTriggerSelector;
    typedef MultiField<Link<TriggerSelector> > MFTriggerSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TriggerSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::TriggerSelector> >;
#endif

}

#endif
