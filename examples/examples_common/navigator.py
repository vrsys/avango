import avango.script
from avango.script import field_has_changed

import avango.gua

from . import device
import time


class Navigator(avango.script.Script):

    OutTransform = avango.gua.SFMatrix4()

    Mouse = device.MouseDevice()
    Keyboard = device.KeyboardDevice()

    RotationSpeed = avango.SFFloat()
    MotionSpeed = avango.SFFloat()

    StartLocation = avango.gua.SFVec3()
    StartRotation = avango.gua.SFVec2()

    __rel_rot_x = avango.SFFloat()
    __rel_rot_y = avango.SFFloat()

    def __init__(self):
        self.super(Navigator).__init__()
        
        self.__rot_x = 0.0
        self.__rot_y = 0.0

        self.__location = avango.gua.Vec3(0.0, 0.0, 0.0)

        self.RotationSpeed.value = 0.1
        self.MotionSpeed.value = 0.1

        self.__rel_rot_x.connect_from(self.Mouse.RelY)
        self.__rel_rot_y.connect_from(self.Mouse.RelX)

        self.__last_time = -1

        self.always_evaluate(True)

    @field_has_changed(StartLocation)
    def reset_location(self):
        self.__location = self.StartLocation.value

    @field_has_changed(StartRotation)
    def reset_rotation(self):
        self.__rot_x = self.StartRotation.value.x
        self.__rot_y = self.StartRotation.value.y

    def evaluate(self):
        if self.__last_time != -1:
            current_time = time.time()
            frame_time = current_time - self.__last_time
            self.__last_time = current_time

            self.__rot_x -= self.__rel_rot_x.value
            self.__rot_y -= self.__rel_rot_y.value

            rotation = avango.gua.make_rot_mat(
                self.__rot_y * self.RotationSpeed.value, 0.0, 1.0, 0.0) *\
                avango.gua.make_rot_mat(
                    self.__rot_x * self.RotationSpeed.value, 1.0, 0.0, 0.0)

            if self.Keyboard.KeyW.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    0.0, 0.0, -self.MotionSpeed.value)).get_translate()

            if self.Keyboard.KeyS.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    0.0, 0.0, self.MotionSpeed.value)).get_translate()

            if self.Keyboard.KeyA.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    -self.MotionSpeed.value, 0.0, 0.0)).get_translate()

            if self.Keyboard.KeyD.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    self.MotionSpeed.value, 0.0, 0.0)).get_translate()

            if self.Keyboard.KeyQ.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    0.0, -self.MotionSpeed.value, 0.0)).get_translate()

            if self.Keyboard.KeyE.value:
                self.__location += (rotation * avango.gua.make_trans_mat(
                    0.0, self.MotionSpeed.value, 0.0)).get_translate()

            target = avango.gua.make_trans_mat(self.__location) * rotation
            orig = avango.gua.make_trans_mat(
                self.OutTransform.value.get_translate()
                ) * avango.gua.make_rot_mat(
                self.OutTransform.value.get_rotate_scale_corrected())

            smoothness = frame_time * 10.0

            self.OutTransform.value = orig * (1.0 - smoothness
                                              ) + target * smoothness

        else:
            self.__last_time = time.time()
