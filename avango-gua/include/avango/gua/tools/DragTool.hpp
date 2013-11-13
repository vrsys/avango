#ifndef AVANGO_GUA_DRAGTOOL_HPP
#define AVANGO_GUA_DRAGTOOL_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/gua/tools/Tool.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/gua/windows_specific_gua.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Drag tool class
     *
     * \ingroup av_tools
     */
    class AV_GUA_DLL DragTool : public Tool
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      DragTool();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~DragTool();

    public:

      /**
       * Defines the dragging transformation relative to the root node coordinate system.
       */
      av::gua::SFMatrix4 DragTransform;

      /* virtual */ void evaluate();

    protected:

      /* virtual */ void evaluateKeptTarget(TargetHolder& holder);

      ::gua::math::mat4 mLastDragMat, mDragDiffMat;
      bool mDragged;
    };

    typedef SingleField<Link<DragTool> > SFDragTool;
    typedef MultiField<Link<DragTool> > MFDragTool;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<::av::gua::DragTool> >;
  template class AV_GUA_DLL MultiField<Link<::av::gua::DragTool> >;
#endif

}

#endif //AVANGO_GUA_DRAGTOOL_HPP
