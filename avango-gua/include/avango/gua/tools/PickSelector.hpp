#ifndef AVANGO_GUA_PICKSELECTOR_HPP
#define AVANGO_GUA_PICKSELECTOR_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/gua/tools/Selector.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>

#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Pick selector class.
     *
     * If triggered, a line segment intersection is performed and for each valid intersection
     * a TargetHolder is created and placed in SelectedTargets. The type of the TargetHolder can
     * be defined with CreateNodePaths and CreateIntersections.
     * The Target in the TargetHolder is the first not ignored avango object found in the
     * intersection node path.
     *
     * The following fields may be added to nodes to influence the picking result:
     * - SFUInt PickMask:      See description of NodePickMask and SubtreePickMask below.
     * - SFBool PickFrontFace: Set to false to ignore front face hits for this node.
     * - SFBool PickBackFace:  Set to false to ignore back face hits for this node.
     * - SFBool PickIgnore:    Set to true to ignore this node as target and in the node path.
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL PickSelector : public Selector
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      PickSelector();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PickSelector();

    public:

      /**
       * Triggers the picking.
       */
      SFBool PickTrigger;

      /**
       * If true, the picking is only updated on PickTrigger transitions, which is the default.
       * If false, picking is updated in every evaluate while PickTrigger is true.
       */
      SFBool TransitionOnly;

      /**
       * If TransitionOnly is False and EveryFrame is true, picking is updated in every frame
       * while PickTrigger is true. By default, EveryFrame is false, which updates the picking
       * only in the frames when evaluate was triggered, e.g. by a change of PickRayTransform.
       */
      SFBool EveryFrame;

      /**
       * Defines the pick ray.
       */
      av::gua::SFRayNode PickRay;

      /**
       * Entry scenegraph node from which the intersection traversal should start.
       */
      av::gua::SFSceneGraph SceneGraph;

      SFInt Options;

      /**
  	   * If true, the creator of the TargetHolder will be set. This is the default.
  	   *
  	   * To deactivate this is considered a workaround.
  	   *
  	   */
  	  SFBool SetCreator;

      /**
       * To disable picking for specific nodes, add a SFUInt field called "PickMask"
       * to the node. The picking is disabled, if the bitwise AND of it with the
       * NodePickMask of the PickSelector is zero. By default the mask is all 1's.
       */
      SFString PickMask;

       /**
       * Manually pick with the current field values and return the selected targets.
       */
      MFTargetHolder::ContainerType const& pick();

      /* virtual */ void evaluate();

    protected:

      MFTargetHolder::ContainerType mSelectedTargets;
      bool mLastPickTrigger;
    };

    typedef SingleField<Link<PickSelector> > SFPickSelector;
    typedef MultiField<Link<PickSelector> > MFPickSelector;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PickSelector> >;
  template class AV_GUA_DLL MultiField<Link<gua::PickSelector> >;
#endif

}

#endif //AVANGO_GUA_PICKSELECTOR_HPP
