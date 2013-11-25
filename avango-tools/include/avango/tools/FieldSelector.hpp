#ifndef AVANGO_TOOLS_FIELDSELECTOR_HPP
#define AVANGO_TOOLS_FIELDSELECTOR_HPP

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
     * FieldSelector class selects targets containing given field.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL FieldSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      FieldSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~FieldSelector();

    public:

      /**
       * Defines the input targets from which the named objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines the name which is used to select targets. The output is in SelectedTargets.
       */
      SFString SelectableFieldName;

      /**
       * Set to false if also objects should be selectable, which contain the given names
       * as substring. Otherwise only equal names are selectable, which is the default.
       */
      SFBool EqualNameOnly;

      /**
       * Specify, if the given strings in SelectableNames and SelectableTargetNames should be treated
       * as regular expressions. Default value: False.
       * If EqualNameOnly and TreatAsRegularExpression are both set to True, each regular expression is
       * surrounded by .*<Expression>.*
       */
      SFBool TreatAsRegularExpression;



      /* virtual */ void evaluate();

    protected:

      bool isSelectable(av::FieldContainer& object);
    };

    typedef SingleField<Link<FieldSelector> > SFFieldSelector;
    typedef MultiField<Link<FieldSelector> > MFFieldSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::FieldSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::FieldSelector> >;
#endif

}

#endif //AVANGO_TOOLS_FIELDSELECTOR_HPP
