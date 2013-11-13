#ifndef AVANGO_GUA_SCALED_GRAB_TOOL_HPP
#define AVANGO_GUA_SCALED_GRAB_TOOL_HPP

/**
 * \file
 * \ingroup av_tools
 */

#include <avango/gua/tools/Tool.hpp>
#include <avango/gua/Fields.hpp>

namespace av
{
  namespace gua
  {
    /**
     * ScaledGrabTool class
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL ScaledGrabTool : public Tool
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      ScaledGrabTool();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~ScaledGrabTool();

    public:

      /**
       * Defines the device transformation relative to the root node coordinate system.
       */
      av::gua::SFMatrix4 DeviceTransform;

      /**
       * Defines the head transformation relative to the root node coordinate system.
       */
      av::gua::SFMatrix4 HeadTransform;

    protected:

      /* virtual */ void evaluateAddedTarget(TargetHolder& holder);
      /* virtual */ void evaluateKeptTarget(TargetHolder& holder);

      ::gua::math::mat4 mTargetMat, mTargetRotMat, mTargetTransMat;
      ::gua::math::mat4 mDeviceInvMat;
      ::gua::math::mat4 mPivotMat, mPivotInvMat;
      ::gua::math::vec3 mDevicePos;
      double mScale;
    };

    typedef SingleField<Link<ScaledGrabTool> > SFScaledGrabTool;
    typedef MultiField<Link<ScaledGrabTool> > MFScaledGrabTool;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::ScaledGrabTool> >;
  template class AV_GUA_DLL MultiField<Link<gua::ScaledGrabTool> >;
#endif

}

#endif //AVANGO_GUA_SCALED_GRAB_TOOL_HPP
