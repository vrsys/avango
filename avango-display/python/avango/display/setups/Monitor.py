# -*- Mode:Python -*-

##########################################################################
#                                                                        #
# This file is part of Avango.                                           #
#                                                                        #
# Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       #
# angewandten Forschung (FhG), Munich, Germany.                          #
#                                                                        #
# Avango is free software: you can redistribute it and/or modify         #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, version 3.                  #
#                                                                        #
# Avango is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Avango. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# Avango is a trademark owned by FhG.                                    #
#                                                                        #
##########################################################################

import avango.display

class Monitor(avango.display.Display):

    def __init__(self, inspector, options):
        super(Monitor, self).__init__("Monitor", inspector)

        self._subdisplay_window_events = {}
        self._subdisplay_camera = {}

        window = self.make_window(0, 0, 1024, 768, 0.4, 0.3, False)
        window.Name.value = "AVANGO"
        window.Decoration.value = True
        window.AutoHeight.value = True
        self.add_window(window, avango.osg.make_trans_mat(0, 1.7, -0.6), 0)

        user = avango.display.nodes.User()
        user.Matrix.value = avango.osg.make_trans_mat(avango.osg.Vec3(0., 1.7, 0.))
        self.add_user(user)

    def make_dominant_user_device(self, user, interface, subdisplay):
        device = avango.display.nodes.Device()
        if subdisplay == "":
            return device

        device.Matrix.connect_from(self._subdisplay_camera[subdisplay].MouseNearTransform)
        device.Button1.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_OnlyLeft)

        return device

    def make_view(self, subdisplay):
        if subdisplay == "":
            return super(Monitor, self).make_view(subdisplay)

        display_view = avango.display.nodes.View()

        # In the Monitor setting each subdisplay simply get a new window
        window = self.make_window(0, 0, 1024, 768, 4, 3, False)
        window.Decoration.value = True
        window.AutoHeight.value = True
        window.ShowCursor.value = True
        window.Title.value = subdisplay
        window.Name.value = subdisplay

        camera, view = self.make_camera(display_view, 0., window)
        camera.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)

        self.add_view(view)

        window_event = avango.osg.viewer.nodes.EventFields(View = view)
        self._subdisplay_window_events[subdisplay] = window_event
        window.DragEvent.connect_from(window_event.DragEvent)
        window.MoveEvent.connect_from(window_event.MoveEvent)
        self._subdisplay_camera[subdisplay] = camera

        return display_view

    def make_device(self, device, interface):
        if device != "SpaceMouse" or interface != "Relative6DOF":
            return None
        
        if not self._device_service: 
            self._device_service = avango.daemon.DeviceService()
        
        sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self._device_service,
                                                  Station = "spacemousestation")
        self.keep_alive(sensor)

        spacemouse = avango.display.nodes.SpaceMouse()
        spacemouse.SensorAbsX.connect_from(sensor.Value0)
        spacemouse.SensorAbsY.connect_from(sensor.Value1)
        spacemouse.SensorAbsZ.connect_from(sensor.Value2)
        spacemouse.SensorAbsRX.connect_from(sensor.Value3)
        spacemouse.SensorAbsRY.connect_from(sensor.Value4)
        spacemouse.SensorAbsRZ.connect_from(sensor.Value5)
        spacemouse.SensorRelX.connect_from(sensor.Value6)
        spacemouse.SensorRelY.connect_from(sensor.Value7)
        spacemouse.SensorRelZ.connect_from(sensor.Value8)
        spacemouse.SensorRelRX.connect_from(sensor.Value9)
        spacemouse.SensorRelRY.connect_from(sensor.Value10)
        spacemouse.SensorRelRZ.connect_from(sensor.Value11)
        spacemouse.SensorBtnA0.connect_from(sensor.Button0)
        spacemouse.SensorBtnA1.connect_from(sensor.Button1)
        spacemouse.SensorBtnB0.connect_from(sensor.Button9)
        spacemouse.SensorBtnB1.connect_from(sensor.Button10)
        spacemouse.SensorBtnB2.connect_from(sensor.Button3)
        spacemouse.SensorBtnB3.connect_from(sensor.Button4)

        time_sensor = avango.nodes.TimeSensor()
        self.keep_alive(time_sensor)
        spacemouse.TimeIn.connect_from(time_sensor.Time)

        return spacemouse
