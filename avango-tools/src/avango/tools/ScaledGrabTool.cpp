#include <avango/tools/ScaledGrabTool.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/Types.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::ScaledGrabTool"));
}

AV_FC_DEFINE(av::tools::ScaledGrabTool);

AV_FIELD_DEFINE(av::tools::SFScaledGrabTool);
AV_FIELD_DEFINE(av::tools::MFScaledGrabTool);

av::tools::ScaledGrabTool::ScaledGrabTool() :
  mScale(1.0)
{
  AV_FC_ADD_FIELD(DeviceTransform, ::gua::math::mat4::identity());
  AV_FC_ADD_FIELD(HeadTransform, ::gua::math::mat4::identity());
}

av::tools::ScaledGrabTool::~ScaledGrabTool()
{}

void
av::tools::ScaledGrabTool::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Tool::initClass();

    AV_FC_INIT(av::tools::Tool, av::tools::ScaledGrabTool, true);

    SFScaledGrabTool::initClass("av::tools::SFScaledGrabTool", "av::Field");
    MFScaledGrabTool::initClass("av::tools::MFScaledGrabTool", "av::Field");
  }
}

/* virtual */ void
av::tools::ScaledGrabTool::evaluateAddedTarget(TargetHolder& holder)
{
  // const SFContainer::ValueType &target_obj = holder.Target.getValue();

  // // grab only MatrixTransform nodes
  // av::gua::TransformNode *target = dynamic_cast<av::gua::TransformNode*>(target_obj.getPtr());
  // if (target != nullptr)
  // {
  //   auto gua_node(dynamic_cast< ::gua::TransformNode*>(target->getGuaNode()));
  //   mTargetMat = gua_node->get_world_transform();
  //   mTargetRotMat = mTargetMat;
  //   mTargetRotMat[12] = 0.f;
  //   mTargetRotMat[13] = 0.f;
  //   mTargetRotMat[14] = 0.f;
  //   ::gua::math::vec3 translation(mTargetMat[12], mTargetMat[13], mTargetMat[14])
  //   mTargetTransMat = ::scm::math::make_translation(translation);
  //   const ::gua::math::vec3 target_pos = translation;

  //   const ::gua::math::mat4 device_mat = DeviceTransform.getValue();
  //   mDevicePos = device_mat.getTrans();
  //   mDeviceInvMat = ::scm::math::inverse(device_mat);

  //   ::gua::math::vec3 pivot_pos;
  //   av::gua::SFVec3 *target_pivot_pos =
  //     dynamic_cast<av::gua::SFVec3*>(target->getField("ScaledGrabPivotPoint"));
  //   if (target_pivot_pos != 0)
  //     pivot_pos = target_pivot_pos->getValue() * mTargetMat;
  //   else
  //   {
  //     Link<IntersectionTargetHolder> isect_holder;
  //     TargetHolder::FindList isect_holder_list =
  //       holder.find(IntersectionTargetHolder::getClassTypeId());
  //     if (!isect_holder_list.empty())
  //       isect_holder = dynamic_cast<IntersectionTargetHolder*>(isect_holder_list.front().getPtr());

  //     if (isect_holder.isValid())
  //       pivot_pos = isect_holder->Intersection.getValue()->Point.getValue();
  //     else
  //       pivot_pos = target_pos;
  //   }

  //   const ::osg::Vec3 pivot_local_pos = pivot_pos - target_pos;
  //   mPivotMat.makeTranslate(pivot_local_pos);
  //   mPivotInvMat.makeTranslate(-pivot_local_pos);

  //   SFDouble *target_scale = dynamic_cast<SFDouble*>(gua_node->getField("ScaledGrabScale"));
  //   if (target_scale != 0)
  //     mScale = target_scale->getValue();
  //   else
  //   {
  //     const ::osg::Vec3 head_pos = HeadTransform.getValue().getTrans();

  //     ::osg::Vec3 pivot_head = head_pos - pivot_pos;
  //     pivot_head.y() = 0.0;
  //     const double pivot_head_dist = pivot_head.length();

  //     ::osg::Vec3 device_head = head_pos - mDevicePos;
  //     device_head.y() = 0.0;
  //     const double device_head_dist = device_head.length();

  //     mScale = (pivot_head_dist > device_head_dist ? pivot_head_dist / device_head_dist : 1.0);
  //   }
  // }
}

/* virtual */ void
av::tools::ScaledGrabTool::evaluateKeptTarget(TargetHolder& holder)
{
  // const SFContainer::ValueType &target_obj = holder.Target.getValue();

  // // apply dragging only to MatrixTransform nodes
  // av::osg::MatrixTransform *target = dynamic_cast<av::osg::MatrixTransform*>(target_obj.getPtr());
  // if (target != 0)
  // {
  //   const ::osg::Matrix device_mat = DeviceTransform.getValue();
  //   const ::osg::Vec3 device_pos = device_mat.getTrans();
  //   ::osg::Matrix device_rot_mat = mDeviceInvMat * device_mat;
  //   device_rot_mat.setTrans(0.0, 0.0, 0.0);

  //   const ::osg::Matrix rot_mat = mPivotInvMat * device_rot_mat * mPivotMat;
  //   const ::osg::Matrix trans_mat = ::osg::Matrix::translate((device_pos - mDevicePos) * mScale);
  //   const ::osg::Matrix new_target_mat = mTargetRotMat * rot_mat * mTargetTransMat * trans_mat;

  //   const ::osg::Matrix target_mat_inv = ::osg::Matrix::inverse(target->getAbsoluteTransform(this));
  //   target->Matrix.setValue(new_target_mat * target_mat_inv * target->Matrix.getValue());
  // }
}
