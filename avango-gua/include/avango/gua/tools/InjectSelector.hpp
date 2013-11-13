#ifndef AVANGO_GUA_INJECTSELECTOR_HPP
#define AVANGO_GUA_INJECTSELECTOR_HPP

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
     * InjectSelector class adds or replaces input target holders with given target holders,
     * if the targets match.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL InjectSelector : public Selector
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
  template class AV_GUA_DLL SingleField<Link<gua::InjectSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::InjectSelector> >;
#endif

}

#endif //AVANGO_GUA_INJECTSELECTOR_HPP
