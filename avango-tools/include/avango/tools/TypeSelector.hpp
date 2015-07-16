#ifndef AVANGO_TOOLS_TYPESELECTOR_HPP
#define AVANGO_TOOLS_TYPESELECTOR_HPP

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
     * TypeSelector class selects targets from the types of given av::FieldContainers.
     *
     * \ingroup av_tools
     */
    class AV_TOOLS_DLL TypeSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      TypeSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~TypeSelector();

    public:

      /**
       * Defines the input targets from which the objects are selected.
       */
      MFTargetHolder Targets;

      /**
       * Defines types by objects which may be selected. The output is in SelectedTargets.
       */
      MFContainer SelectableTypes;

      /**
       * Defines additional types by objects which may come as output from another Selector.
       */
      MFTargetHolder SelectableTargetTypes;

      /**
       * Set to true if only equal types should be selectable.
       * Otherwise also derived types are selectable, which is the default.
       */
      SFBool EqualTypeOnly;

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    protected:

      bool isSelectable(const av::FieldContainer& object);

      std::set<Type> mTypes;
      bool mTypesDirty;
    };

    using SFTypeSelector = SingleField<Link<TypeSelector> >;
    using MFTypeSelector = MultiField<Link<TypeSelector> >;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_TOOLS_DLL SingleField<Link<tools::TypeSelector> >;
  template class AV_TOOLS_DLL MultiField<Link<tools::TypeSelector> >;
#endif
}

#endif //AVANGO_TOOLS_TYPESELECTOR_HPP
