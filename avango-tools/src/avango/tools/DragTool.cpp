// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/tools/DragTool.h>

#include <avango/osg/MatrixTransform.h>

#include <avango/Logger.h>

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
  AV_FC_ADD_FIELD(DragTransform, ::osg::Matrix());
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
    mDragDiffMat = ::osg::Matrix::inverse(mLastDragMat) * DragTransform.getValue();

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
    av::osg::MatrixTransform *target = dynamic_cast<av::osg::MatrixTransform*>(target_obj.getPtr());
    if (target != 0)
    {
      //apply dragging
      const ::osg::Matrix target_mat = target->getAbsoluteTransform(this);
      target->Matrix.setValue(target_mat * mDragDiffMat * ::osg::Matrix::inverse(target_mat) *
                              target->Matrix.getValue());
    }
  }
}
