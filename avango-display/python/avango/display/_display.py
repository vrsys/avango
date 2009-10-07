import avango.display #FIXME remove cyclic dependency
import avango.daemon
import avango.osg.viewer
import avango.inspector
import getopt
import sys
from math import *

class _Display(object):

    def __init__(self):
        self._display_type = ""
        self._two_view_walls = [":0.0", ":0.0"]      # needed for Twopipe/Onepipe Mode

        self._perf2osg = avango.osg.make_rot_mat(radians(-90), 1, 0, 0)
        self._device_service = avango.daemon.DeviceService()

        self._composite_viewer = avango.osg.viewer.nodes.CompositeViewer()
        self._merge_viewer = ViewMerger()
        self._composite_viewer.Views.connect_from(self._merge_viewer.ViewsOut)

        self._windows = []
        self._users = []
        self._screen_transforms = []
        self._screen_sizes = []
        self._eye_vec = avango.osg.Vec3(0., 1.7, 0.)

        self._touchscreen_camera = None
        self._touchscreen_window = None

        self._inspector = None

        # Keeps references to objects alive
        self._keep_alive = []

        self._subdisplay_window_events = {}
        self._subdisplay_camera = {}

    def parse(self, argv):
        try:
            opts, args = getopt.getopt(argv[1:], "hn:l:d:i",
                                       ["help", "notify=", "log-file=", "display=", "inspector", "onepipe", "twopipe"])
        except getopt.GetoptError, err:
            pass

        notify_level = -1
        notify_logfile = ''
        for opt, arg in opts:
            if opt in ("-h", "--help"):
                print "Usage: python programm.py [--display=TwoWiew] [--twopipe] [--inspector]"
                print "options and arguments:"
                print "--display <screenid>     : selects Display setup"
                print "--twopipe                : use TwoPipe mode, i.e. two graphics cards"
                print "--inspector              : inspect scene graph with AVANGO inspector"
            elif opt in ("-n", "--notify"):
                notify_level = int(arg)
            elif opt in ("-l", "--log-file"):
                notify_logfile = arg
            elif opt in ("-d", "--display"):
                self._display_type = arg
            elif opt in ("-i", "--inspector"):
                self._inspector = avango.inspector.nodes.Inspector()
            elif opt in ("--onepipe"):
                self._two_view_walls = [":0.0", ":0.0"]
            elif opt in ("--twopipe"):
                self._two_view_walls = [":0.0", ":0.1"]

        if notify_level > -1:
            if notify_logfile == '':
                avango.enable_logging(notify_level)
            else:
                avango.enable_logging(notify_level, notify_logfile)

        # We always have one user
        self._users.append(avango.display.nodes.User())
        self._users[0].Matrix.value = avango.osg.make_trans_mat(self._eye_vec)

        if self._display_type == "FakeTwoView":
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))

            # Viewer 1
            self._windows.append(self._make_window(0, 0, 1280, 1024, 6, 2.4, False, self._two_view_walls[0]))
            self._windows[0].Name.value = "1"

            # Viewer 2
            xpos = 0
            if self._two_view_walls[1] == ":0.0":
                xpos = 1280
            self._windows.append(self._make_window(xpos, 0, 1280, 1024, 6, 2.4, False, self._two_view_walls[1]))
            self._windows[1].Name.value = "2"

        elif self._display_type == "TwoView":
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))

            # Viewer 1
            self._windows.append(self._make_window(0, 0, 1280, 1024, 6, 2.4, True, self._two_view_walls[0]))
            self._windows[0].Name.value = "1"

            # Viewer 2
            xpos = 0
            if self._two_view_walls[1] == ":0.0":
                xpos = 1280
            self._windows.append(self._make_window(xpos, 0, 1280, 1024, 6, 2.4, True, self._two_view_walls[1]))
            self._windows[1].Name.value = "2"

            # Properly connect the two users (and add second user before)
            self._users.append(avango.display.nodes.User())
            view1_yellow_glasses = self._create_glasses("ve-dtrack-head4", avango.osg.Vec3(-0.074, -0.018, 0.025))
            self._users[0].Matrix.connect_from(view1_yellow_glasses.Matrix)
            view2_blue_glasses = self._create_glasses("ve-dtrack-head3", avango.osg.Vec3(-0.073, -0.016, 0.025))
            self._users[1].Matrix.connect_from(view2_blue_glasses.Matrix)

        elif self._display_type == "LShape":
            # generate 2 _screen_transforms
            # how many windows needed?
            print "not implemented."
            exit(0)

        elif self._display_type == "iCone":
            # generate 4 _screen_transforms
            self._screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(84.135), 0, 1, 0))
            self._screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(28.045), 0, 1, 0))
            self._screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(-28.045), 0, 1, 0))
            self._screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(-84.135), 0, 1, 0))

            # Viewer 1
            self._windows.append(self._make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.0"))
            self._windows.append(self._make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.1"))
            self._windows.append(self._make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.2"))
            self._windows.append(self._make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.3"))

        elif self._display_type == "TouchscreenEmulator":
            # Viewer 1
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))
            self._windows.append(self._make_window(0, 0, 1280, 1024, 6, 2.4, False))
            self._windows[0].Name.value = ""
            self._touchscreen_window = self._make_window(0, 0, 1280, 1024, 6, 2.4, False)

        elif self._display_type == "TwoviewTouchscreenEmulator":
            # Viewer 1
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))
            self._windows.append(self._make_window(0, 0, 1280, 1024, 6, 2.4, True, ":0.0"))
            self._windows[0].Name.value = ""
            self._touchscreen_window = self._make_window(1280, 0, 1280, 1024, 6., 2.4, False, ":0.0")
            self._touchscreen_window.Name.value = "Touchscreen"

        elif self._display_type == "Wall":
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.6+0.15, -1.2))

            # Viewer 1
            self._windows.append(self._make_window(0, 0, 800, 600, 1.2, 0.9, True))
            self._windows[0].Name.value = ""

        elif self._display_type == "AutoStereoDisplay":
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.7, -0.7))

            # Viewer 1
            self._windows.append(self._make_window(0, 0, 1200, 1600, 0.33, 0.43, True))
            self._windows[0].Name.value = ""

        else:
            # Viewer 1
            self._screen_transforms.append(avango.osg.make_trans_mat(0, 1.7, -0.6))
            self._display_type = "Monitor"
            window = self._make_window(0, 0, 1024, 768, 0.4, 0.3, False)
            window.Decoration.value = True
            window.AutoHeight.value = True
            self._windows.append(window)

        return args

    def get_display_type(self):
        return self._display_type

    def get_num_users(self, subdisplay):
        return len(self._users)

    def make_user(self, user, interface, subdisplay):
        if user > len(self._users):
            return None
        return self._users[user]

    def make_dominant_user_device(self, user, interface, subdisplay):
        device = avango.display.nodes.Device()
        if (self._display_type == "TouchscreenEmulator" or self._display_type == "TwoviewTouchscreenEmulator") and subdisplay == "Touchscreen":
            device.Matrix.connect_from(self._touchscreen_camera.MouseNearTransform)
            device.Button1.connect_from(self._touchscreen_event.MouseButtons_OnlyLeft)
        elif self._display_type == "TwoView":
            pda_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self._device_service,
                                                          Station = "ve-dtrack-pda2")
            pda_sensor.TransmitterOffset.value = self._perf2osg
            pda_sensor.ReceiverOffset.value = avango.osg.make_trans_mat(0.076, -0.016, 0.025)
            device.Matrix.connect_from(pda_sensor.Matrix)
        elif self._display_type == "Monitor":
            device.Matrix.connect_from(self._subdisplay_camera[subdisplay].MouseNearTransform)
            device.Button1.connect_from(self._subdisplay_window_events[subdisplay].MouseButtons_OnlyLeft)
        return device

    def make_non_dominant_user_device(self, user, interface, subdisplay):
        pass

    def run(self):
        self._composite_viewer.run()

    def make_view(self, subdisplay):
        display_view = avango.display.nodes.View()

        if subdisplay == "":
            if self._display_type == "iCone":
                for i in range(0, len(self._windows)):
                    camera = avango.osg.viewer.nodes.Camera()
                    camera.EyeOffset.value = 0.03
                    camera.Viewport.connect_from(display_view.Viewport)
                    camera.ViewerTransform.connect_from(display_view.Camera)
                    camera.ScreenTransform.value = self._screen_transforms[i]
                    camera.Window.value = self._windows[i]
                    camera.EyeTransform.value = avango.osg.make_trans_mat(self._eye_vec)

                    osg_view = avango.osg.viewer.nodes.View()
                    osg_view.Scene.connect_from(display_view.Root)
                    osg_view.MasterCamera.value = camera

                    index = self._merge_viewer.add_input()
                    self._merge_viewer.get_input(index).value = osg_view
            else:
                for current_user in xrange(0, len(self._users)):
                    screen_transform = self._screen_transforms[0]
                    screen_size = self._screen_sizes[0]

                    camera = avango.osg.viewer.nodes.Camera()
                    if self._windows[current_user].QuadBufferStereo.value:
                        camera.EyeOffset.value = 0.03
                    camera.ViewerTransform.connect_from(display_view.Camera)
                    camera.Near.connect_from(display_view.Near)
                    camera.Far.connect_from(display_view.Far)
                    camera.BackgroundColor.connect_from(display_view.BackgroundColor)
                    camera.Window.value = self._windows[current_user]
                    camera.EyeTransform.connect_from(self._users[current_user].Matrix)

                    splitscreen_handling = ViewportConverter()
                    splitscreen_handling.ViewportIn.connect_from(display_view.Viewport)
                    splitscreen_handling.RealActualWidth.connect_from(self._windows[current_user].RealActualWidth)
                    splitscreen_handling.RealActualHeight.connect_from(self._windows[current_user].RealActualHeight)
                    splitscreen_handling.ScreenTransformIn.value = screen_transform
                    camera.ScreenTransform.connect_from(splitscreen_handling.ScreenTransformOut)
                    camera.Viewport.connect_from(splitscreen_handling.ViewportOut)

                    osg_view = avango.osg.viewer.nodes.View()
                    osg_view.Scene.connect_from(display_view.Root)
                    osg_view.MasterCamera.value = camera

                    user_selector = ViewUserSelector(UserMatch=current_user+1)
                    user_selector.ViewIn.value = osg_view
                    user_selector.UserSelector.connect_from(display_view.UserSelector)

                    index = self._merge_viewer.add_input()
                    self._merge_viewer.get_input(index).connect_from(user_selector.ViewOut)

        elif subdisplay == "Touchscreen" and (self._display_type in ["TouchscreenEmulator", "TwoviewTouchscreenEmulator"]):
            camera = avango.osg.viewer.nodes.Camera()
            camera.ViewerTransform.connect_from(display_view.Camera)
            camera.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
            camera.Near.connect_from(display_view.Near)
            camera.Far.connect_from(display_view.Far)
            camera.BackgroundColor.connect_from(display_view.BackgroundColor)
            camera.Window.value = self._touchscreen_window
            camera.Window.value.Decoration.value = True
            camera.Window.value.ShowCursor.value = True
            camera.Window.value.Title.value = "Touchscreen"

            osg_view = avango.osg.viewer.nodes.View()
            osg_view.Scene.connect_from(display_view.Root)
            osg_view.MasterCamera.value = camera

            index = self._merge_viewer.add_input()
            self._merge_viewer.get_input(index).value = osg_view

            self._touchscreen_camera = camera
            self._touchscreen_event = avango.osg.viewer.nodes.EventFields(View = osg_view)
            self._touchscreen_window.DragEvent.connect_from(self._touchscreen_event.DragEvent)
            self._touchscreen_window.MoveEvent.connect_from(self._touchscreen_event.MoveEvent)

        elif subdisplay != "" and self._display_type == "Monitor":
            # In the Monitor setting each subdisplay simply get a new window
            camera = avango.osg.viewer.nodes.Camera()
            camera.ViewerTransform.connect_from(display_view.Camera)
            camera.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
            camera.Near.connect_from(display_view.Near)
            camera.Far.connect_from(display_view.Far)
            camera.BackgroundColor.connect_from(display_view.BackgroundColor)
            window = self._make_window(0, 0, 1024, 768, 4, 3, False)
            window.Decoration.value = True
            window.AutoHeight.value = True
            window.Name.value = subdisplay
            camera.Window.value = window
            camera.Window.value.Decoration.value = True
            camera.Window.value.ShowCursor.value = True
            camera.Window.value.Title.value = subdisplay

            osg_view = avango.osg.viewer.nodes.View()
            osg_view.Scene.connect_from(display_view.Root)
            osg_view.MasterCamera.value = camera

            index = self._merge_viewer.add_input()
            self._merge_viewer.get_input(index).value = osg_view

            window_event = avango.osg.viewer.nodes.EventFields(View = osg_view)
            self._subdisplay_window_events[subdisplay] = window_event
            window.DragEvent.connect_from(window_event.DragEvent)
            window.MoveEvent.connect_from(window_event.MoveEvent)
            self._subdisplay_camera[subdisplay] = camera

        if self._inspector and len(self._inspector.Children.value) == 0:
            # FIXME this should use a proper aggregation node
            converter = SFNode2MFContainerConverter()
            converter.Input.connect_from(display_view.Root)
            self._inspector.Children.connect_from(converter.Output)

        return display_view

    def _make_window(self, x_position, y_position, width, height, real_width, real_height, use_stereo, screen_identifier=":0.0"):
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
        window.QuadBufferStereo.value = use_stereo
        self._screen_sizes.append(avango.osg.Vec2(real_width, real_height))
        return window
    
    def _create_glasses(self, name, receiver_offset):
        sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self._device_service,
                                                  Station = name)
        sensor.ReceiverOffset.value = avango.osg.make_trans_mat(receiver_offset)
        sensor.TransmitterOffset.value = self._perf2osg

        return sensor

    def make_device(self, device, interface):
        if self._display_type == "Monitor":
            if device == "SpaceMouse" and interface == "Relative6DOF":
                sensor = avango.daemon.nodes.DeviceSensor(DeviceService = self._device_service,
                                                          Station = "spacemousestation")
                self._keep_alive.append(sensor)

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
                self._keep_alive.append(time_sensor)
                spacemouse.TimeIn.connect_from(time_sensor.Time)

                return spacemouse


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
