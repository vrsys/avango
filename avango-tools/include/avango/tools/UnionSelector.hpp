#ifndef AVANGO_TOOLS_UNIONSELECTOR_HPP
#define AVANGO_TOOLS_UNIONSELECTOR_HPP

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
     * UnionSelector class passes the union of two sets of targets.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL UnionSelector : public Selector
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
  template class AV_TOOLS_DLL SingleField<Link<tools::UnionSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::UnionSelector> >;
#endif

}

#endif //AVANGO_TOOLS_UNIONSELECTOR_HPP
