#ifndef AVANGO_TOOLS_INJECTSELECTOR_HPP
#define AVANGO_TOOLS_INJECTSELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/tools/Selector.hpp>
#include <avango/tools/windows_specific_tools.hpp>

namespace av
{
  namespace tools
  {
    /**
     * InjectSelector class adds or replaces input target holders with given target holders,
     * if the targets match.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL InjectSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      InjectSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~InjectSelector();

    public:

      /**
       * Defines the input targets.
       */
      MFTargetHolder Targets;

      /**
       * Defines target holder which replace or which are added to the input targets.
       */
      MFTargetHolder InjectTargets;

      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<InjectSelector> > SFInjectSelector;
    typedef MultiField<Link<InjectSelector> > MFInjectSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::InjectSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::InjectSelector> >;
#endif

}

#endif //AVANGO_TOOLS_INJECTSELECTOR_HPP
