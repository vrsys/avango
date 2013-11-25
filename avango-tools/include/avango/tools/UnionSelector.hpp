#ifndef AVANGO_GUA_UNIONSELECTOR_HPP
#define AVANGO_GUA_UNIONSELECTOR_HPP

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
     * UnionSelector class passes the union of two sets of targets.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL UnionSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      UnionSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~UnionSelector();

    public:

      /**
       * Defines the input target sets whose union is passed to SelectedTargets.
       */
      MFTargetHolder TargetSet1;
      MFTargetHolder TargetSet2;

      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<UnionSelector> > SFUnionSelector;
    typedef MultiField<Link<UnionSelector> > MFUnionSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::UnionSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::UnionSelector> >;
#endif

}

#endif //AVANGO_GUA_UNIONSELECTOR_HPP
