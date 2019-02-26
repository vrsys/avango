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

#include <avango/moving/DesktopFlyer.h>

#include <avango/Logger.h>

#include <boost/bind.hpp>

#include <cmath>
#include <iostream>

namespace
{
av::Logger& logger(av::getLogger("av::moving::DesktopFlyer"));
}

AV_FC_DEFINE(av::moving::DesktopFlyer);

AV_FIELD_DEFINE(av::moving::SFDesktopFlyer);
AV_FIELD_DEFINE(av::moving::MFDesktopFlyer);

av::moving::DesktopFlyer::DesktopFlyer() : mRotX(0.0), mRotY(0.0), mRotZ(0.0), mMoveX(0.0), mMoveY(0.0), mMoveZ(0.0), mModeChangeCounter(0)

{
    AV_FC_ADD_FIELD(RotateXYTrigger, false);
    AV_FC_ADD_FIELD(MoveYRotateZTrigger, false);
    AV_FC_ADD_FIELD(MoveZRotateZTrigger, false);
    AV_FC_ADD_FIELD(MoveXZTrigger, false);
    AV_FC_ADD_FIELD(MoveXYTrigger, false);
    AV_FC_ADD_FIELD(MoveForward, false);
    AV_FC_ADD_FIELD(MoveBackward, false);
    AV_FC_ADD_FIELD(MoveUp, false);
    AV_FC_ADD_FIELD(MoveDown, false);
    AV_FC_ADD_FIELD(MoveLeft, false);
    AV_FC_ADD_FIELD(MoveRight, false);
    AV_FC_ADD_FIELD(RotateUp, false);
    AV_FC_ADD_FIELD(RotateDown, false);
    AV_FC_ADD_FIELD(RotateLeft, false);
    AV_FC_ADD_FIELD(RotateRight, false);
    AV_FC_ADD_FIELD(RotateCW, false);
    AV_FC_ADD_FIELD(RotateCCW, false);
    AV_FC_ADD_FIELD(MouseIn, ::osg::Vec2(0.0, 0.0));
    AV_FC_ADD_FIELD(TimeIn, 0);
    AV_FC_ADD_FIELD(MoveSpeed, 27.78);
    AV_FC_ADD_FIELD(MoveAcceleration, 0.0);
    AV_FC_ADD_FIELD(MoveBreak, 5.0);
    AV_FC_ADD_FIELD(MaxSpeed, 55.56);
    AV_FC_ADD_FIELD(RotationSpeed, 3.14);
    AV_FC_ADD_FIELD(RelativeMouseTrigger, false);
    AV_FC_ADD_FIELD(HaltOnClutch, true);
    AV_FC_ADD_FIELD(InvertYMouseAxis, false);
    AV_FC_ADD_FIELD(MouseSpeed, 10.);

    alwaysEvaluate(true);
    mOldTime = TimeIn.getValue();
    mCurrentTime = mOldTime;
    mFrameTime = 1.0 / 60.0;
}

av::moving::DesktopFlyer::~DesktopFlyer() {}

void av::moving::DesktopFlyer::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::MatrixTransform::initClass();

        AV_FC_INIT(av::osg::MatrixTransform, av::moving::DesktopFlyer, true);

        SFDesktopFlyer::initClass("av::moving::SFDesktopFlyer", "av::Field");
        MFDesktopFlyer::initClass("av::moving::MFDesktopFlyer", "av::Field");
    }
}

/* virtual */ void av::moving::DesktopFlyer::fieldHasChanged(const av::Field& field) { av::osg::MatrixTransform::fieldHasChanged(field); }

/* virtual */ void av::moving::DesktopFlyer::evaluate()
{
    // calculate current frame time
    mCurrentTime = TimeIn.getValue();
    if(mCurrentTime > mOldTime)
    {
        mFrameTime = mCurrentTime - mOldTime;
        mOldTime = mCurrentTime;
    }

    av::osg::MatrixTransform::evaluate();

    float current_speed_x = 0.0;
    float current_speed_y = 0.0;
    float current_speed_z = 0.0;

    float mouse_x = MouseIn.getValue().x();
    float mouse_y = MouseIn.getValue().y();

    float ms = MouseSpeed.getValue();
    bool move_xz_trigger = MoveXZTrigger.getValue();
    bool move_xy_trigger = MoveXYTrigger.getValue();
    bool move_y_rot_z_trigger = MoveYRotateZTrigger.getValue();
    bool rot_xy_trigger = RotateXYTrigger.getValue();
    bool move_forward = MoveForward.getValue();
    bool move_backward = MoveBackward.getValue();
    bool move_left = MoveLeft.getValue();
    bool move_right = MoveRight.getValue();
    bool move_up = MoveUp.getValue();
    bool move_down = MoveDown.getValue();
    bool rot_cw = RotateCW.getValue();
    bool rot_ccw = RotateCCW.getValue();
    bool rot_left = RotateLeft.getValue();
    bool rot_right = RotateRight.getValue();
    bool rot_up = RotateUp.getValue();
    bool rot_down = RotateDown.getValue();

    bool move_z_active = (move_xz_trigger || (move_forward != move_backward));
    bool move_x_active = (move_xz_trigger || move_xy_trigger || (move_right != move_left));
    bool move_y_active = (move_y_rot_z_trigger || move_xy_trigger || (move_up != move_down));
    bool rot_z_active = (move_y_rot_z_trigger || (rot_ccw != rot_cw));
    bool rot_x_active = (rot_xy_trigger || (rot_right != rot_left));
    bool rot_y_active = (rot_xy_trigger || (rot_up != rot_down));

    // input values
    float move_z_input = 0;
    float move_x_input = 0;
    float move_y_input = 0;
    float rot_z_input = 0;
    float rot_x_input = 0;
    float rot_y_input = 0;

    if(move_z_active)
    {
        if(move_forward)
            move_z_input = 1;
        if(move_backward)
            move_z_input = -1;
        if(std::abs(move_z_input) < std::abs(mouse_y))
            move_z_input = mouse_y * ms;
    }
    if(move_x_active)
    {
        if(move_right)
            move_x_input = 1;
        if(move_left)
            move_x_input = -1;
        if(std::abs(move_x_input) < std::abs(mouse_x))
            move_x_input = mouse_x * ms;
    }
    if(move_y_active)
    {
        if(move_up)
            move_y_input = 1;
        if(move_down)
            move_y_input = -1;
        if(std::abs(move_y_input) < std::abs(mouse_y))
            move_y_input = mouse_y * ms;
    }
    if(rot_z_active)
    {
        if(rot_ccw)
            rot_z_input = 1;
        if(rot_cw)
            rot_z_input = -1;
        if(std::abs(rot_z_input) < std::abs(mouse_x))
            rot_z_input = mouse_x * ms;
    }
    if(rot_x_active)
    {
        if(rot_right)
            rot_x_input = 1;
        if(rot_left)
            rot_x_input = -1;
        if(std::abs(rot_x_input) < std::abs(mouse_x))
            rot_x_input = mouse_x * ms;
    }
    if(rot_y_active)
    {
        if(rot_up)
            rot_y_input = 1;
        if(rot_down)
            rot_y_input = -1;
        if(std::abs(rot_y_input) < std::abs(mouse_y))
            rot_y_input = mouse_y * ms;
    }

    bool mouse_triggered = (rot_xy_trigger || move_xz_trigger || move_xy_trigger || move_y_rot_z_trigger);
    bool halt = HaltOnClutch.getValue();
    if(!RelativeMouseTrigger.getValue() && mouse_triggered)
    {
        RelativeMouseTrigger.setValue(true);
        mouse_x = 0;
        mouse_y = 0;
        move_z_input = 0;
        move_x_input = 0;
        move_y_input = 0;
        rot_z_input = 0;
        rot_x_input = 0;
        rot_y_input = 0;
        if(halt)
        {
            mOldSpeedX = 0;
            mOldSpeedY = 0;
            mOldSpeedZ = 0;
        }
    }
    else if(!mouse_triggered)
    {
        RelativeMouseTrigger.setValue(false);
        mModeChangeCounter = 0;
    }

    float rot_speed = RotationSpeed.getValue();
    float invert_y = InvertYMouseAxis.getValue() ? -1.0 : 1.0;
    mRotX = -rot_x_input * rot_speed * mFrameTime;
    mRotY = invert_y * rot_y_input * rot_speed * mFrameTime;
    mRotZ = -rot_z_input * rot_speed * mFrameTime;

    float move_acc = MoveAcceleration.getValue();
    if(move_acc > 0)
    {
        current_speed_z = (!halt || move_z_active) * mOldSpeedZ + move_z_input * move_acc * mFrameTime;

        current_speed_x = (!halt || move_x_active) * mOldSpeedX + move_x_input * move_acc * mFrameTime;

        current_speed_y = (!halt || move_y_active) * mOldSpeedY + move_y_input * move_acc * mFrameTime;
    }
    else
    {
        current_speed_z = move_z_input * MoveSpeed.getValue();
        current_speed_x = move_x_input * MoveSpeed.getValue();
        current_speed_y = move_y_input * MoveSpeed.getValue();
    }

    // clamp to maxspeed
    float max_speed = MaxSpeed.getValue();
    if(std::abs(current_speed_z) > max_speed)
    {
        current_speed_z = max_speed * std::abs(current_speed_z) / current_speed_z;
    }
    if(std::abs(current_speed_x) > max_speed)
    {
        current_speed_x = max_speed * std::abs(current_speed_x) / current_speed_x;
    }
    if(std::abs(current_speed_y) > max_speed)
    {
        current_speed_y = max_speed * std::abs(current_speed_y) / current_speed_y;
    }

    // break
    float move_break = MoveBreak.getValue();
    if((move_acc > 0) && (move_break > 0))
    {
        if((std::abs(current_speed_x) > 0) || (std::abs(current_speed_y) > 0) || (std::abs(current_speed_z) > 0))
        {
            int dir_x = (current_speed_x > 0 ? 1 : -1);
            int dir_y = current_speed_y > 0 ? 1 : -1;
            int dir_z = current_speed_z > 0 ? 1 : -1;
            float brake_x = -move_break * dir_x * mFrameTime;
            float brake_y = -move_break * dir_y * mFrameTime;
            float brake_z = -move_break * dir_z * mFrameTime;
            int dir_x_new = current_speed_x + brake_x > 0 ? 1 : -1;
            int dir_y_new = current_speed_y + brake_y > 0 ? 1 : -1;
            int dir_z_new = current_speed_z + brake_z > 0 ? 1 : -1;
            current_speed_x = (move_x_input != 0 ? current_speed_x : ((dir_x != dir_x_new) ? 0 : current_speed_x + brake_x));
            current_speed_y = (move_y_input != 0) ? current_speed_y : (dir_y != dir_y_new) ? 0 : current_speed_y + brake_y;
            current_speed_z = (move_z_input != 0) ? current_speed_z : (dir_z != dir_z_new) ? 0 : current_speed_z + brake_z;
        }
    }

    mMoveX = current_speed_x * mFrameTime;
    mMoveY = current_speed_y * mFrameTime;
    mMoveZ = current_speed_z * mFrameTime;
    mOldSpeedX = current_speed_x;
    mOldSpeedY = current_speed_y;
    mOldSpeedZ = current_speed_z;

    Matrix.setValue(::osg::Matrix::translate(::osg::Vec3(mMoveX, mMoveY, -mMoveZ)) *
                    ::osg::Matrix::rotate(mRotX, ::osg::Vec3(0.0, 1.0, 0.0), mRotY, ::osg::Vec3(1.0, 0.0, 0.0), mRotZ, ::osg::Vec3(0.0, 0.0, 1.0)) * Matrix.getValue());
}
