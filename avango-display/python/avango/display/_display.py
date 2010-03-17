import avango.display #FIXME remove cyclic dependency
import avango.daemon
import avango.osg
import avango.osg.viewer
from math import *

class Display(object):

    def __init__(self, display_type, inspector):
        self._display_type = display_type
        self._inspector = inspector

        self._perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
        self._keep_alive = [] # Keeps references to objects alive

        self._composite_viewer = avango.osg.viewer.nodes.CompositeViewer()
        self._merge_viewer = ViewMerger()
        self._composite_viewer.Views.connect_from(self._merge_viewer.ViewsOut)

        self.device_service = avango.daemon.DeviceService()

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

    def make_view(self, subdisplay):
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

        if self._inspector and len(self._inspector.Children.value) == 0:
            # FIXME this should use a proper aggregation node
            converter = SFNode2MFContainerConverter()
            converter.Input.connect_from(display_view.Root)
            self._inspector.Children.connect_from(converter.Output)

        return display_view

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

            real_size = Float2Vec2Converter()
            self.keep_alive(real_size)
            real_size.Value0.connect_from(self._windows[0][0].RealActualWidth)
            real_size.Value1.connect_from(self._windows[0][0].RealActualHeight)
            screen.RealSize.connect_from(real_size.Output)

            pixel_size = Float2Vec2Converter()
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
        sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self.device_service,
                                                  Station = name)
        sensor.ReceiverOffset.value = avango.osg.make_trans_mat(receiver_offset)
        sensor.TransmitterOffset.value = self._perf2osg

        return sensor

    def make_window(self, x_position, y_position, width, height, real_width, real_height, use_stereo, screen_identifier=":0.0"):
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
        self._windows.append((window, transform, user))

    def add_user(self, user):
        self._users.append(user)


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


class SFNode2MFContainerConverter(avango.script.Script):
    "Converts a SFNode to a MFNode"

    Input = avango.osg.SFNode()
    Output = avango.MFContainer()

    def evaluate(self):
        self.Output.value = [ self.Input.value ]

class Float2Vec2Converter(avango.script.Script):
    "Converts two Floats into on Vec2"

    Value0 = avango.SFFloat()
    Value1 = avango.SFFloat()
    Output = avango.osg.SFVec2()

    def evaluate(self):
        self.Output.value = avango.osg.Vec2(self.Value0.value, self.Value1.value)

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
