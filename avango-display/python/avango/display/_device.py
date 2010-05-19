# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of AVANGO.                                           #
#                                                                        #
# Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# AVANGO is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# AVANGO is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
##########################################################################

import avango.osg.viewer
from math import *

class User(avango.script.Script):
    Active = avango.SFBool()
    Matrix = avango.osg.SFMatrix()

    def __init__(self):
        self.super(User).__init__()
        self.Active.value = True

class Device(avango.script.Script):
    Active = avango.SFBool()
    Matrix = avango.osg.SFMatrix()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()
    Button4 = avango.SFBool()
    Button5 = avango.SFBool()

    def __init__(self):
        self.super(Device).__init__()
        self.Active.value = True


class KeyboardDevice(avango.script.Script):

    KeysPressed = avango.MFInt()

    KeyRight = avango.SFBool()
    KeyLeft = avango.SFBool()
    KeyUp = avango.SFBool()
    KeyDown = avango.SFBool()
    PageDown = avango.SFBool()
    PageUp = avango.SFBool()
    Shift =  avango.SFBool()
    Ctrl =  avango.SFBool()
    KeyAlt =  avango.SFBool()
    Insert =  avango.SFBool()
    Delete = avango.SFBool()
    Home = avango.SFBool()
    End = avango.SFBool()
    KeyEsc = avango.SFBool()
    KeySpace = avango.SFBool()
    KeyEnter = avango.SFBool()
    AltReturn = avango.SFBool()

    KeyF1 = avango.SFBool()
    KeyF2 = avango.SFBool()
    KeyF3 = avango.SFBool()
    KeyF4 = avango.SFBool()
    KeyF5 = avango.SFBool()
    KeyF6 = avango.SFBool()
    KeyF7 = avango.SFBool()
    KeyF8 = avango.SFBool()
    KeyF9 = avango.SFBool()
    KeyF10 = avango.SFBool()
    KeyF11 = avango.SFBool()
    KeyF12 = avango.SFBool()

    Key0 = avango.SFBool()
    Key1 = avango.SFBool()
    Key2 = avango.SFBool()
    Key3 = avango.SFBool()
    Key4 = avango.SFBool()
    Key5 = avango.SFBool()
    Key6 = avango.SFBool()
    Key7 = avango.SFBool()
    Key8 = avango.SFBool()
    Key9 = avango.SFBool()

    KeyX = avango.SFBool()
    KeyZ = avango.SFBool()

    KeyBackslash = avango.SFBool()
    KeyCloseBracket = avango.SFBool()

    def __init__(self):
        self.super(KeyboardDevice).__init__()

        self.__prev_keys_pressed = []

        self.__keymap = {}

        self.__keymap[48] = self.Key1
        self.__keymap[49] = self.Key1
        self.__keymap[50] = self.Key2
        self.__keymap[51] = self.Key3
        self.__keymap[52] = self.Key4
        self.__keymap[53] = self.Key5
        self.__keymap[54] = self.Key6
        self.__keymap[55] = self.Key7
        self.__keymap[56] = self.Key8
        self.__keymap[57] = self.Key9

        self.__keymap[120] = self.KeyX
        self.__keymap[122] = self.KeyZ

        self.__keymap[92] = self.KeyBackslash
        self.__keymap[41] = self.KeyCloseBracket

    def add_key(self,name,id):
        """
        Add a SFBool, which will be connected to the key press with the given id
        The naming convention says that you should pass Key<Name> as name parameter.
        E.g.: If you want to add the key <1> call add_key("Key1",48)
        """

        #if a field with the given name is already known, no new field will be added
        field = self._get_field(name)
        if field:
            return False

        self.add_and_init_field(avango.SFBool(), name, id)
        self.__keymap[id] = getattr(self, name)
        self.__keymap[id].value = False

        return True

    def evaluate(self):

        pressed_keys = []
        for k in self.KeysPressed.value:
            pressed_keys.append(k)

        #pressed keys
        for key in pressed_keys:
            if key in self.__keymap:
                self.__keymap[key].value = True

        #release keys
        released_keys=filter(lambda x:x not in pressed_keys, self.__prev_keys_pressed)
        for key in released_keys:
            if key in self.__keymap:
                self.__keymap[key].value = False

        self.__prev_keys_pressed = pressed_keys


    def connect(self, eventfields):

        self.KeysPressed.connect_from(eventfields.KeysPressed)

        self.KeyRight.connect_from(eventfields.KeyRight)
        self.KeyLeft.connect_from(eventfields.KeyLeft)
        self.KeyUp.connect_from(eventfields.KeyUp)
        self.KeyDown.connect_from(eventfields.KeyDown)
        self.PageUp.connect_from(eventfields.KeyPageUp)
        self.PageDown.connect_from(eventfields.KeyPageDown)
        self.Shift.connect_from(eventfields.KeyShift)
        self.Ctrl.connect_from(eventfields.KeyCtrl)
        self.KeyAlt.connect_from(eventfields.KeyAlt)
        self.Insert.connect_from(eventfields.KeyInsert)
        self.Delete.connect_from(eventfields.KeyDelete)
        self.Home.connect_from(eventfields.KeyHome)
        self.End.connect_from(eventfields.KeyEnd)
        self.KeyEsc.connect_from(eventfields.KeyEsc)
        self.KeySpace.connect_from(eventfields.KeySpace)
        self.KeyEnter.connect_from(eventfields.KeyEnter)
        self.AltReturn.connect_from(eventfields.KeyAltReturn)
        self.KeyF1.connect_from(eventfields.KeyF1)
        self.KeyF2.connect_from(eventfields.KeyF2)
        self.KeyF3.connect_from(eventfields.KeyF3)
        self.KeyF4.connect_from(eventfields.KeyF4)
        self.KeyF5.connect_from(eventfields.KeyF5)
        self.KeyF6.connect_from(eventfields.KeyF6)
        self.KeyF7.connect_from(eventfields.KeyF7)
        self.KeyF8.connect_from(eventfields.KeyF8)
        self.KeyF9.connect_from(eventfields.KeyF9)
        self.KeyF10.connect_from(eventfields.KeyF10)
        self.KeyF11.connect_from(eventfields.KeyF11)
        self.KeyF12.connect_from(eventfields.KeyF12)


class MouseDevice(avango.script.Script):
    Matrix = avango.osg.SFMatrix()
    MousePosNorm = avango.osg.SFVec2()
    MousePos = avango.osg.SFVec2()
    MouseMovementNorm = avango.osg.SFVec2()
    MouseButtonLeft = avango.SFBool()
    MouseButtonRight = avango.SFBool()
    MouseButtonMiddle = avango.SFBool()
    MouseScrollUp = avango.SFBool()
    MouseScrollDown = avango.SFBool()
    MouseLeftDoubleClick = avango.SFBool()
    MouseLeftAndRight = avango.SFBool()

    def __init__(self):
        self.super(MouseDevice).__init__()

    def connect(self, eventfields, camera, window):
        self.Matrix.connect_from(camera.MouseNearTransform)

        self.MousePosNorm.connect_from(window.MousePositionNorm)
        self.MousePos.connect_from(window.MousePosition)
        self.MouseMovementNorm.connect_from(window.MouseMovementNorm)

        self.MouseButtonLeft.connect_from(eventfields.MouseButtonLeft)
        self.MouseButtonRight.connect_from(eventfields.MouseButtonRight)
        self.MouseScrollUp.connect_from(eventfields.MouseScrollUp)
        self.MouseScrollDown.connect_from(eventfields.MouseScrollDown)
        self.MouseButtonMiddle.connect_from(eventfields.MouseButtonMiddle)
        self.MouseLeftDoubleClick.connect_from(eventfields.MouseButtonLeftDoubleClick)
        self.MouseLeftAndRight.connect_from(eventfields.MouseButtons_LeftAndRight)


class SpaceMouse(avango.script.Script):
    TimeIn = avango.SFFloat()

    SensorAbsX = avango.SFFloat()
    SensorAbsY = avango.SFFloat()
    SensorAbsZ = avango.SFFloat()
    SensorAbsRX = avango.SFFloat()
    SensorAbsRY = avango.SFFloat()
    SensorAbsRZ = avango.SFFloat()
    SensorRelX = avango.SFFloat()
    SensorRelY = avango.SFFloat()
    SensorRelZ = avango.SFFloat()
    SensorRelRX = avango.SFFloat()
    SensorRelRY = avango.SFFloat()
    SensorRelRZ = avango.SFFloat()

    SensorBtnA0 = avango.SFBool()
    SensorBtnA1 = avango.SFBool()
    SensorBtnB0 = avango.SFBool()
    SensorBtnB1 = avango.SFBool()
    SensorBtnB2 = avango.SFBool()
    SensorBtnB3 = avango.SFBool()

    TranslationScale = avango.SFFloat()
    RotationScale = avango.SFFloat()

    MatrixOut = avango.osg.SFMatrix()
    Button0 = avango.SFBool()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()

    # Class attribute will be overridden once last time was set
    _last_time = -1.

    def get_time_diff(self, cur_time):
        result = cur_time
        if self._last_time != -1.:
            result = cur_time - self._last_time
        self._last_time = cur_time
        return result

    def evaluate(self):
        values = self.get_values()

        cur_time = values.TimeIn
        time_delta = self.get_time_diff(cur_time)

        # Mix values from different SpaceMouse types
        trans_x = values.SensorAbsX + values.SensorRelX/500.
        trans_y = values.SensorAbsY - values.SensorRelZ/500.
        trans_z = values.SensorAbsZ + values.SensorRelY/500.
        translation = avango.osg.Vec3(trans_x, trans_y, trans_z)
        rot_x = values.SensorAbsRX + values.SensorRelRX/500.
        rot_y = values.SensorAbsRY - values.SensorRelRZ/500.
        rot_z = values.SensorAbsRZ + values.SensorRelRY/500.
        rotation = avango.osg.Vec3(rot_x, rot_y, rot_z)

        translation *= time_delta * values.TranslationScale
        rotation *= time_delta * values.RotationScale

        rot_mat_x = avango.osg.make_rot_mat(rotation.x, 1., 0., 0.)
        rot_mat_y = avango.osg.make_rot_mat(rotation.y, 0., 1., 0.)
        rot_mat_z = avango.osg.make_rot_mat(rotation.z, 0., 0., 1.)
        values.MatrixOut = rot_mat_x * rot_mat_y * rot_mat_z * avango.osg.make_trans_mat(translation)

        values.Button0 = values.SensorBtnA0 | values.SensorBtnB0
        values.Button1 = values.SensorBtnA1 | values.SensorBtnB1
        values.Button2 = values.SensorBtnB2
        values.Button3 = values.SensorBtnB3


def make_wiimote_device(wiimote_station, dtrack_station, device_service, receiver_offset, transmitter_offset):
    device = WiimoteDevice()
    device._setup(wiimote_station, dtrack_station, device_service, receiver_offset, transmitter_offset)
    return device


class WiimoteDevice(avango.script.Script):

    Button0 = avango.SFBool()
    Button1 = avango.SFBool()
    Button2 = avango.SFBool()
    Button3 = avango.SFBool()
    Button4 = avango.SFBool()
    Button5 = avango.SFBool()
    Button6 = avango.SFBool()
    Button7 = avango.SFBool()
    Button8 = avango.SFBool()
    Button9 = avango.SFBool()
    Button10 = avango.SFBool()

    LED0 = avango.SFBool()
    LED1 = avango.SFBool()
    LED2 = avango.SFBool()
    LED3 = avango.SFBool()

    Matrix = avango.osg.SFMatrix()

    def __init__(self):
        self.super(WiimoteDevice).__init__()

        self.Button0.value = False
        self.Button1.value = False
        self.Button2.value = False
        self.Button3.value = False
        self.Button4.value = False
        self.Button5.value = False
        self.Button6.value = False
        self.Button7.value = False
        self.Button8.value = False
        self.Button9.value = False
        self.Button10.value = False

        self.LED0.value = False
        self.LED1.value = False
        self.LED2.value = False
        self.LED3.value = False

        #will be initialized by the init function
        self.dtrack_sensor = None
        self.wiimote_sensor = None
        self.wiimote_actuator = None
        self.device_service = None


    def _setup(self, wiimote_station, dtrack_station, device_service, receiver_offset, transmitter_offset):
        self.device_service = device_service
        self.dtrack_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                                              Station = dtrack_station,
                                                              ReceiverOffset = receiver_offset,
                                                              TransmitterOffset = transmitter_offset)
        self.wiimote_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = device_service,
                                                               Station = wiimote_station)
        self.wiimote_actuator = avango.daemon.nodes.WiimoteActuator(DeviceService = device_service,
                                                                    Station = wiimote_station)

        self.Button0.connect_from(self.wiimote_sensor.Button0)
        self.Button1.connect_from(self.wiimote_sensor.Button1)
        self.Button2.connect_from(self.wiimote_sensor.Button2)
        self.Button3.connect_from(self.wiimote_sensor.Button3)
        self.Button4.connect_from(self.wiimote_sensor.Button4)
        self.Button5.connect_from(self.wiimote_sensor.Button5)
        self.Button6.connect_from(self.wiimote_sensor.Button6)
        self.Button7.connect_from(self.wiimote_sensor.Button7)
        self.Button8.connect_from(self.wiimote_sensor.Button8)
        self.Button9.connect_from(self.wiimote_sensor.Button9)
        self.Button10.connect_from(self.wiimote_sensor.Button10)

        self.wiimote_actuator.LED0.connect_from(self.LED0)
        self.wiimote_actuator.LED1.connect_from(self.LED1)
        self.wiimote_actuator.LED2.connect_from(self.LED2)
        self.wiimote_actuator.LED3.connect_from(self.LED3)

        self.Matrix.connect_from(self.dtrack_sensor.Matrix)
