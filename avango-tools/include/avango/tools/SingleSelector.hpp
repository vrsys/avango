#ifndef AVANGO_TOOLS_SINGLE_SELECTOR_HPP
#define AVANGO_TOOLS_SINGLE_SELECTOR_HPP


#include <avango/tools/Selector.hpp>
#include <avango/tools/windows_specific_tools.hpp>

namespace av
{
  namespace tools
  {
    /**
     * SingleSelector class passes one of multiple targets.
     *
     * \ingroup av_gua
     */
    class AV_TOOLS_DLL SingleSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      SingleSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~SingleSelector();

    public:

      /**
       * Defines the input targets from which one is passed to SelectedTargets.
       */
      MFTargetHolder Targets;

      /**
       * Set to true to pass the last instead of the first target to SelectedTargets.
       */
      SFBool LastTarget;

      /* virtual */ void evaluate();

    };

    using SFSingleSelector = SingleField<Link<SingleSelector> >;
    using MFSingleSelector = MultiField<Link<SingleSelector> >;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::SingleSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::SingleSelector> >;
#endif

}

#endif //AVANGO_TOOLS_SINGLE_SELECTOR_HPP
