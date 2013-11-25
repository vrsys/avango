#include <avango/tools/DragTool.hpp>

#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Logger.h>
#include <scm/core/math.h>

namespace
{
  av::Logger& logger(av::getLogger("av::tools::DragTool"));
}

AV_FC_DEFINE(av::tools::DragTool);

AV_FIELD_DEFINE(av::tools::SFDragTool);
AV_FIELD_DEFINE(av::tools::MFDragTool);

av::tools::DragTool::DragTool():
  mDragged(false)
{
  AV_FC_ADD_FIELD(DragTransform, ::gua::math::mat4::identity());
}

av::tools::DragTool::~DragTool()
{}

void
av::tools::DragTool::initClass()
{
  if (!isTypeInitialized())
  {
    av::tools::Tool::initClass();

    AV_FC_INIT(av::tools::Tool, av::tools::DragTool, true);

    SFDragTool::initClass("av::tools::SFDragTool", "av::Field");
    MFDragTool::initClass("av::tools::MFDragTool", "av::Field");
  }
}

/* virtual */ void
av::tools::DragTool::evaluate()
{
  // drag transformation matrix changed?
  mDragged = (DragTransform.getValue() != mLastDragMat);

  // calculate difference of the dragging transformation matrix to the last evaluate
  if (mDragged)
    mDragDiffMat = ::scm::math::inverse(mLastDragMat) * DragTransform.getValue();

  // call evaluateKeptTarget
  av::tools::Tool::evaluate();

  if (mDragged)
    mLastDragMat = DragTransform.getValue();
}

/* virtual */ void
av::tools::DragTool::evaluateKeptTarget(TargetHolder& holder)
{
  // apply dragging only if drag transformation matrix was changed
  if (mDragged)
  {
    const SFContainer::ValueType &target_obj = holder.Target.getValue();

    // apply dragging only to MatrixTransform nodes.
    av::gua::TransformNode *target = dynamic_cast<av::gua::TransformNode*>(target_obj.getPtr());
    if (target != nullptr)
    {
      auto gua_node (target->getGuaNode());
      if (gua_node) {
        //apply dragging
        const ::gua::math::mat4 target_mat = gua_node->get_world_transform();
        gua_node->set_transform(target_mat * mDragDiffMat * ::scm::math::inverse(target_mat) *
                              gua_node->get_transform());
      }
    }
  }
}
