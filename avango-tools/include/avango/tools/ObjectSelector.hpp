#ifndef AVANGO_TOOLS_OBJECTSELECTOR_HPP
#define AVANGO_TOOLS_OBJECTSELECTOR_HPP

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
     * ObjectSelector class selects targets from given av::FieldContainers.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL ObjectSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      ObjectSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ObjectSelector();

    public:

      /**
       * Defines the input targets from which the objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines the objects which may be selected. The output is in SelectedTargets.
       */
      MFContainer SelectableObjects;

      /**
       * Defines additional selectable objects wihch may come as output from another Selector.
       */
      MFTargetHolder SelectableTargets;


      /* virtual */ void evaluate();

    };

    typedef SingleField<Link<ObjectSelector> > SFObjectSelector;
    typedef MultiField<Link<ObjectSelector> > MFObjectSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::ObjectSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::ObjectSelector> >;
#endif

}

#endif //AVANGO_TOOLS_OBJECTSELECTOR_HPP
