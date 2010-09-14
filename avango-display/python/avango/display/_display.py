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

import avango.display #FIXME remove cyclic dependency

import avango.osg
import avango.osg.viewer
import avango.script
from avango.script import field_has_changed
import avango.utils

import sys
from math import *

#The avango.daemon module is currently not available under windows
if sys.platform != 'win32':
   import avango.daemon

class Display(object):

    def __init__(self, display_type, inspector):
        self._display_type = display_type
        self._inspector = inspector

        self._perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
        self._keep_alive = [] # Keeps references to objects alive

        self._composite_viewer = avango.osg.viewer.nodes.CompositeViewer()
        self._merge_viewer = ViewMerger()
        self._composite_viewer.Views.connect_from(self._merge_viewer.ViewsOut)

        #the device service will be created as soon as it is used.
        self._device_service = None

        self._bool_dict={'false':False, 'off':False, 'no':False, 
                         'true':True, 'on':True, 'yes':True }

        self._users = []
        self._windows = []

    def get_display_type(self):
        return self._display_type

    def get_num_users(self, subdisplay):
        return len(self._users)

    def make_user(self, user, interface, subdisplay):
        if user > len(self._users):
            return None
        return self._users[user]

    def make_view(self, subdisplay, display_view = None):
        if not display_view:
            display_view = avango.display.nodes.View()

        for window, transform, current_user in self._windows:
            eye_offset = 0.
            if window.StereoMode.value != avango.osg.viewer.stereo_mode.STEREO_MODE_NONE:
                eye_offset = 0.03

            camera, view = self.make_camera_with_viewport(
                display_view, eye_offset, transform, window)
            camera.EyeTransform.connect_from(self._users[current_user].Matrix)

            user_selector = ViewUserSelector(UserMatch=current_user+1)
            user_selector.ViewIn.value = view
            user_selector.UserSelector.connect_from(display_view.UserSelector)
            self.keep_alive(user_selector)

            self.connect_view_field(user_selector.ViewOut)

            self.view_created(camera,view,subdisplay)

        if self._inspector and len(self._inspector.Children.value) == 0:
            # FIXME this should use a proper aggregation node
            converter = avango.utils.nodes.SFNode2MFContainerConverter()
            converter.Input.connect_from(display_view.Root)
            self._inspector.Children.connect_from(converter.Output)

        return display_view

    def get_camera(self, subdisplay):
        return None

    def make_device(self, device, interface):
        return None

    def make_dominant_user_device(self, user, interface, subdisplay):
        return None

    def make_non_dominant_user_device(self, user, interface, subdisplay):
        return None

    def make_screen_descriptor(self, subdisplay):
        screen = avango.display.nodes.ScreenDescriptor()
        if len(self._windows) == 1:
            # Only handle simple case here: one window
            screen.Transform.value = self._windows[0][1]

            real_size = avango.utils.nodes.Float2Vec2Converter()
            self.keep_alive(real_size)
            real_size.Value0.connect_from(self._windows[0][0].RealActualWidth)
            real_size.Value1.connect_from(self._windows[0][0].RealActualHeight)
            screen.RealSize.connect_from(real_size.Output)

            pixel_size = avango.utils.nodes.Float2Vec2Converter()
            self.keep_alive(pixel_size)
            pixel_size.Value0.connect_from(self._windows[0][0].ActualWidth)
            pixel_size.Value1.connect_from(self._windows[0][0].ActualHeight)
            screen.PixelSize.connect_from(pixel_size.Output)

        return screen

    def run(self):
        self._composite_viewer.run()

    def keep_alive(self, value):
        self._keep_alive.append(value)

    def make_glasses(self, name, receiver_offset):
        sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self._device_service,
                                                  Station = name)
        sensor.ReceiverOffset.value = avango.osg.make_trans_mat(receiver_offset)
        sensor.TransmitterOffset.value = self._perf2osg

        return sensor

    def make_window(self, x_position, y_position, width, height, real_width, real_height, use_stereo, screen_identifier=":0.0", num_stencil_bits=2):
        """Generates Window with Position and Stereo enabled/disabled"""
        window = avango.osg.viewer.nodes.GraphicsWindow()
        window.ScreenIdentifier.value = screen_identifier
        window.RealScreenWidth.value = real_width
        window.RealScreenHeight.value = real_height
        window.WantedWidth.value = width
        window.WantedHeight.value = height
        window.Decoration.value = False
        window.ShowCursor.value = False
        window.WantedPositionX.value = x_position
        window.WantedPositionY.value = y_position
        window.AutoHeight.value = False
        window.NumStencilBits.value = num_stencil_bits
        if use_stereo:
            window.StereoMode.value = avango.osg.viewer.stereo_mode.STEREO_MODE_QUAD_BUFFER
        else:
            window.StereoMode.value = avango.osg.viewer.stereo_mode.STEREO_MODE_NONE
        return window

    def make_camera(self, display_view, eye_offset, window, connect_viewport = True):
        camera = avango.osg.viewer.nodes.Camera()
        camera.EyeOffset.value = eye_offset
        if connect_viewport:
            camera.Viewport.connect_from(display_view.Viewport)
        camera.Near.connect_from(display_view.Near)
        camera.Far.connect_from(display_view.Far)
        camera.BackgroundColor.connect_from(display_view.BackgroundColor)
        camera.ViewerTransform.connect_from(display_view.Camera)
        camera.Window.value = window

        view = avango.osg.viewer.nodes.View()
        #check for an alternative root node (Monitor setup)
        root = getattr(display_view,"BoundingSphereRoot",None)
        if root:
            view.Scene.connect_from(root)
        else:
            view.Scene.connect_from(display_view.Root)
            
        view.MasterCamera.value = camera
        return camera, view

    def make_camera_with_viewport(self, display_view, eye_offset, screen_transform, window):
        splitscreen_handling = ViewportConverter()
        splitscreen_handling.ViewportIn.connect_from(display_view.Viewport)
        splitscreen_handling.RealActualWidth.connect_from(window.RealActualWidth)
        splitscreen_handling.RealActualHeight.connect_from(window.RealActualHeight)
        splitscreen_handling.ScreenTransformIn.value = screen_transform
        self.keep_alive(splitscreen_handling)

        camera, view = self.make_camera(display_view, eye_offset, window, False)
        camera.ScreenTransform.connect_from(splitscreen_handling.ScreenTransformOut)
        camera.Viewport.connect_from(splitscreen_handling.ViewportOut)
        return camera, view

    def add_view(self, view):
        index = self._merge_viewer.add_input()
        self._merge_viewer.get_input(index).value = view

    def connect_view_field(self, view_field):
        index = self._merge_viewer.add_input()
        self._merge_viewer.get_input(index).connect_from(view_field)

    def add_window(self, window, transform, user):
#        import traceback
#        traceback.print_stack()
        self._windows.append((window, transform, user))

    def add_user(self, user):
        self._users.append(user)

    def view_created(self,camera,view,subdisplay=""):
        '''
        Template function, which can be overridden by subclasses.
        This function is called every time a view has been created.
        '''
        pass

class ViewUserSelector(avango.script.Script):
    'Activates or deactivates a viewport depending on the selected user'

    UserSelector = avango.SFInt()
    UserMatch = avango.SFInt()

    ViewIn = avango.osg.viewer.SFView()
    ViewOut = avango.osg.viewer.SFView()

    def evaluate(self):
        if self.UserSelector.value != 0 and self.UserSelector.value != self.UserMatch.value:
            self.ViewOut.value = None
            return

        self.ViewOut.value = self.ViewIn.value


class ViewMerger(avango.script.Script):
    'Merge a list of input fields into an output list of views'

    ViewsOut = avango.osg.viewer.MFView()

    def __init__(self):
        self.super(ViewMerger).__init__()
        self._num_inputs = 0

    def evaluate(self):
        result = []
        for i in xrange(0, self._num_inputs):
            value = self.get_input(i).value
            if value:
                result.append(value)
        self.ViewsOut.value = result

    def get_input(self, index):
        if index >= self._num_inputs:
            return None
        return getattr(self, self.get_input_name(index))

    def get_input_name(self, index):
        return 'ViewIn%i' % index

    def add_input(self):
        result = self._num_inputs
        self._num_inputs += 1

        self.add_field(avango.osg.viewer.SFView(), self.get_input_name(result))

        return result


class ViewportConverter(avango.script.Script):
    """Given a (relative) Viewport (i.e. as rectangular section of a window)
    as new Transformation and (camera) Viewport is calculated."""

    ViewportIn = avango.osg.SFVec4()
    ViewportOut = avango.osg.SFVec4()

    RealActualWidth = avango.SFFloat()
    RealActualHeight = avango.SFFloat()

    ScreenTransformIn = avango.osg.SFMatrix()
    ScreenTransformOut = avango.osg.SFMatrix()

    def evaluate(self):
        viewport_in = self.ViewportIn.value
        self.ViewportOut.value = avango.osg.Vec4(viewport_in.x,
                                                 viewport_in.y,
                                                 viewport_in.z - viewport_in.x,
                                                 viewport_in.w - viewport_in.y)

        x_trans = 0.5 * (viewport_in.x + viewport_in.z - 1.) * self.RealActualWidth.value
        y_trans = 0.5 * (viewport_in.y + viewport_in.w - 1.) * self.RealActualHeight.value
        self.ScreenTransformOut.value = avango.osg.make_trans_mat(x_trans, y_trans, 0.) * self.ScreenTransformIn.value
