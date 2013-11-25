#ifndef AVANGO_GUA_TYPESELECTOR_HPP
#define AVANGO_GUA_TYPESELECTOR_HPP

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
     * TypeSelector class selects targets from the types of given av::FieldContainers.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL TypeSelector : public Selector
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

    typedef SingleField<Link<TypeSelector> > SFTypeSelector;
    typedef MultiField<Link<TypeSelector> > MFTypeSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TypeSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::TypeSelector> >;
#endif
}

#endif //AVANGO_GUA_TYPESELECTOR_HPP
