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

import avango.script
import avango.osg
import avango.display
import avango.utils
import os

class Monitor(avango.display.Display):

    def __init__(self, inspector, options):
        super(Monitor, self).__init__("Monitor", inspector)

        self._subdisplay_window = {}
        self._subdisplay_window_events = {}
        self._subdisplay_camera = {}

        self._screen_identifier = os.environ.get('DISPLAY', '')
        
        trackball_token = "trackball"
        self._enable_trackball=False
        if options.has_key(trackball_token) and self._bool_dict.has_key(options[trackball_token].lower()):
            self._enable_trackball=self._bool_dict[options[trackball_token].lower()]
            
        window = self.make_window(0, 0,  1024, 768, 0.4, 0.3, False, self._screen_identifier)
        window.Name.value = "AVANGO"
        window.Decoration.value = True
        window.AutoHeight.value = True
        window.ShowCursor.value = True
        self.add_window(window, avango.osg.make_trans_mat(0, 1.7, -0.6), 0)
        self._subdisplay_window[""]=window
        
        user = avango.display.nodes.User()
        user.Matrix.value = avango.osg.make_trans_mat(avango.osg.Vec3(0., 1.7, 0.))
        self.add_user(user)
    
    def make_view(self, subdisplay, trackball_support=True):
        
        display_view = avango.display.nodes.MonitorView()
        #display_view = avango.display.nodes.View()
        if subdisplay == "":
            super(Monitor, self).make_view(subdisplay, display_view)
                
        # In the Monitor setting each subdisplay simply get a new window
        else:
            window = self.make_window(0, 0, 1024, 768, 4, 3, False, self._screen_identifier)
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
            self._subdisplay_window[subdisplay] = window
        
        
        #configure trackball
        display_view.EnableTrackball.value = self._enable_trackball
        time_sensor = avango.nodes.TimeSensor()
        self.keep_alive(time_sensor)
        trackball = avango.utils.nodes.Trackball()
        trackball.Enable.connect_from(display_view.EnableTrackball)
        toggle_field = avango.utils.make_key_toggle_trigger_alternate(
                        avango.utils.make_bool2_and(self._subdisplay_window_events[subdisplay].KeyShift,
                                                    self._subdisplay_window_events[subdisplay].KeyEnter),
                       True)
        trackball.CenterToBoundingSphere.connect_from(toggle_field)
        trackball.BoundingSphere.connect_from(display_view.BoundingSphereRoot.value.BoundingSphere)
        trackball.TimeIn.connect_from(time_sensor.Time)
        trackball.SpinningTimeThreshold.value = 0.5
        trackball.Direction.connect_from(self._subdisplay_window[subdisplay].MousePositionNorm)
        trackball.RotateTrigger.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_OnlyMiddle)
        trackball.PanTrigger.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_LeftAndMiddle)
        trackball.ZoomTrigger.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_OnlyRight)
        trackball.ResetTrigger.connect_from(self._subdisplay_window_events[subdisplay].KeySpace)
        
        display_view.Camera.connect_from(trackball.Matrix)
        self.keep_alive(trackball)
        #TODO Enable the osg auto near/far plane computation
        display_view.Near.value = 0.1 
        display_view.Far.value = 100000
        
        
        
        #add some default actions
        #show window decoration (Ctrl+Enter)
        toggle_field = avango.utils.make_key_toggle_trigger_alternate(
                          avango.utils.make_bool2_and(self._subdisplay_window_events[subdisplay].KeyCtrl,
                                                      self._subdisplay_window_events[subdisplay].KeyEnter),
                          True)
        display_view.WindowDecoration.connect_from(toggle_field)
        self._subdisplay_window[subdisplay].Decoration.connect_from(display_view.WindowDecoration)
        #toggle fullscreen (Alt+Enter)
        toggle_field = avango.utils.make_key_toggle_trigger( 
                            avango.utils.make_bool2_and(self._subdisplay_window_events[subdisplay].KeyAlt,
                                                        self._subdisplay_window_events[subdisplay].KeyEnter) )
        display_view.ToggleFullScreen.connect_from(toggle_field)
        self._subdisplay_window[subdisplay].ToggleFullScreen.connect_from(display_view.ToggleFullScreen)
        
        return display_view
 
    
    def view_created(self, camera, view):
        'Primary window is automatically created by the constructor. All other windows are created by explicit calls of make_view(subdisplay)'
        # It seems that a view is only allowed to be associated with one EventField at a time.
        if self._subdisplay_window_events.has_key(""):
            return
        else:
            window_event = avango.osg.viewer.nodes.EventFields(View = view)
            self._subdisplay_window_events[""] = window_event
            self._subdisplay_window[""].DragEvent.connect_from(window_event.DragEvent)
            self._subdisplay_window[""].MoveEvent.connect_from(window_event.MoveEvent)
            self._subdisplay_camera[""] = camera


    def make_dominant_user_device(self, user, interface, subdisplay):
        if subdisplay not in self._subdisplay_camera:
            return avango.display.nodes.Device()

        if interface == "Keyboard":
            keyboard = avango.display.KeyboardDevice()
            keyboard.connect(self._subdisplay_window_events[subdisplay])
            return keyboard

        elif interface == "Mouse":
            mouse = avango.display.MouseDevice()
            mouse.connect(self._subdisplay_window_events[subdisplay],
                          self._subdisplay_camera[subdisplay],
                          self._subdisplay_window[subdisplay]
                          )
            return mouse
        
        else:
            device = avango.display.nodes.Device()
            device.Matrix.connect_from(self._subdisplay_camera[subdisplay].MouseNearTransform)
            device.Button1.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_OnlyLeft)
            return device


    def make_device(self, device, interface):
        
        if device == "SpaceMouse" or interface == "Relative6DOF":

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
            
            
