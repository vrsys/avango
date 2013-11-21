#ifndef AVANGO_GUA_CHANGESELECTOR_HPP
#define AVANGO_GUA_CHANGESELECTOR_HPP

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
     * ChangeSelector class passes targets which were added since the last evaluate
     * and additional outputs removed targets.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL ChangeSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      ChangeSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ChangeSelector();

    public:

      /**
       * Defines the input targets whose added targets are passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Outputs the targets which were removed from Targets since the last evaluate.
       */
      MFTargetHolder KeptTargets;

      /**
       * Outputs the targets which were removed from Targets since the last evaluate.
       */
      MFTargetHolder RemovedTargets;

      /* virtual */ void evaluate();

    protected:

      MFTargetHolder::ContainerType mLastTargets;

    };

    typedef SingleField<Link<ChangeSelector> > SFChangeSelector;
    typedef MultiField<Link<ChangeSelector> > MFChangeSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::ChangeSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::ChangeSelector> >;
#endif

}

#endif //AVANGO_GUA_CHANGESELECTOR_HPP
