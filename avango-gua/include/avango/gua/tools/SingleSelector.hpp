#ifndef AVANGO_GUA_SINGLE_SELECTOR_HPP
#define AVANGO_GUA_SINGLE_SELECTOR_HPP


#include <avango/gua/tools/Selector.hpp>
#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
  namespace gua
  {
    /**
     * SingleSelector class passes one of multiple targets.
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SingleSelector : public Selector
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

    typedef SingleField<Link<SingleSelector> > SFSingleSelector;
    typedef MultiField<Link<SingleSelector> > MFSingleSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SingleSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::SingleSelector> >;
#endif

}

#endif //AVANGO_GUA_SINGLE_SELECTOR_HPP
