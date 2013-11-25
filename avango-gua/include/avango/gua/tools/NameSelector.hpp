#ifndef AVANGO_GUA_NAMESELECTOR_HPP
#define AVANGO_GUA_NAMESELECTOR_HPP

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
     * NameSelector class selects targets from given names.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL NameSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      NameSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~NameSelector();

    public:

      /**
       * Defines the input targets from which the named objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines names which are used to select targets. The output is in SelectedTargets.
       */
      MFString SelectableNames;

      /**
       * Defines additional names by objects which may come as output from another Selector.
       */
      MFTargetHolder SelectableTargetNames;

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

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    protected:

      bool isSelectable(av::FieldContainer& object);

      std::set<std::string> mNames;
      bool mNamesDirty;
    };

    typedef SingleField<Link<NameSelector> > SFNameSelector;
    typedef MultiField<Link<NameSelector> > MFNameSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::NameSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::NameSelector> >;
#endif

}

#endif //AVANGO_GUA_NAMESELECTOR_HPP
