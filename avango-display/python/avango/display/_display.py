import avango.display #FIXME remove cyclic dependency
import avango.osg.viewer
import getopt
import sys
from math import *

_display_type = ""
_composite_viewer = avango.osg.viewer.nodes.CompositeViewer()
_windows = []                           # How to use Windows?
_two_view_walls = [":0.0", ":0.0"]      # needed for Twopipe/Onepipe Mode
_screen_transforms = []                 # Cone has 4 Screens
_eye_vec = avango.osg.Vec3(0, 1.7, 0)
_touchscreen_camera = None
_touchscreen_window = None
_dominant_user_device = ""

def get_display_type():
    return _display_type

def init(argv):
    """evaluates command-line parameter"""

    global _display_type, _two_view_walls, _num_user

    try:
        opts, args = getopt.getopt(argv[1:], "hn:", ["help", "display=", "onepipe", "twopipe"])
    except getopt.GetoptError, err:
        print "error: wrong arguments"
        opts = [("-h", "")]

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print "Usage: python programm.py [--display=TwoWiew] [--twopipe]"
            print "options and arguments:"
            print "--display <screenid>     : selects Display setup"
        elif opt in ("--display"):
            _display_type = arg
        elif opt in ("--onepipe"):
            _two_view_walls = [":0.0", ":0.0"]
        elif opt in ("--twopipe"):
            _two_view_walls = [":0.0", ":0.1"]

    if _display_type == "FakeTwoView":
        _screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))

        # Viewer 1
        _windows.append(_make_window(0, 0, 1280, 1024, 6, 2.4, False, _two_view_walls[0]))
        _windows[0].Name.value = "1"

        # Viewer 2
        xpos = 0
        if _two_view_walls[1] == ":0.0":
            xpos = 1280
        _windows.append(_make_window(xpos, 0, 1280, 1024, 6, 2.4, False, _two_view_walls[1]))
        _windows[1].Name.value = "2"

    elif _display_type == "TwoView":
        _screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))

        # Viewer 1
        _windows.append(_make_window(0, 0, 1280, 1024, 6, 2.4, True, _two_view_walls[0]))
        _windows[0].Name.value = "1"

        # Viewer 2
        xpos = 0
        if _two_view_walls[1] == ":0.0":
            xpos = 1280
        _windows.append(_make_window(xpos, 0, 1280, 1024, 6, 2.4, True, _two_view_walls[1]))
        _windows[1].Name.value = "2"

    elif _display_type == "LShape":
        # generate 2 _screen_transforms
        # how many windows needed?
        print "not implemented."
        exit(0)

    elif _display_type == "iCone":
        # generate 4 _screen_transforms
        _screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(84.135), 0, 1, 0))
        _screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(28.045), 0, 1, 0))
        _screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(-28.045), 0, 1, 0))
        _screen_transforms.append(avango.osg.make_rot_mat(radians(-4.43), 1, 0, 0) * avango.osg.make_trans_mat(0, 1.390, -2.818) * avango.osg.make_rot_mat(radians(-84.135), 0, 1, 0))

        # Viewer 1
        _windows.append(_make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.0"))
        _windows.append(_make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.1"))
        _windows.append(_make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.2"))
        _windows.append(_make_window(0, 0, 1440, 1320, 3.540, 2.830, True, ":0.3"))

    elif _display_type == "TouchscreenEmulator":
        # Viewer 1
        _screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))
        _windows.append(_make_window(0, 0, 1280, 1024, 6, 2.4, False))
        _windows[0].Name.value = ""
        global _touchscreen_window
        _touchscreen_window = _make_window(0, 0, 1280, 1024, 6, 2.4, False)

    elif _display_type == "TwoviewTouchscreenEmulator":
        # Viewer 1
        _screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))
        _windows.append(_make_window(0, 0, 1280, 1024, 6, 2.4, True, ":0.0"))
        _windows[0].Name.value = ""
        global _touchscreen_window
        _touchscreen_window = _make_window(0, 0, 1280, 1024, 6, 2.4, False, "labws2:0.0")
        _touchscreen_window.Name.value = "Touchscreen"

    else:
        # Viewer 1
        _screen_transforms.append(avango.osg.make_trans_mat(0, 1.2, -2.4))
        _display_type = "Monitor"
        _windows.append(_make_window(0, 0, 1280, 1024, 6, 2.4, False))
        _windows[0].Name.value = ""

    return argv

def make_user(user=0, interface="", subdisplay=""):
    """Create a field container that represents the user position.
    
      - *user* selects the n-th *active* user of the display.

      - *interface* is used to select a different field container than the
        default one, i.e. different display may provide more specific or
        incompatible information about the active user. The resulting interface
        will still always contain the *Active* boolean field (see below).

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the user of a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Position* is a matrix representing the position of the user in display
        local coordinates.

      - *Active* is a boolean that is true if the user is recognized by the
        system (e.g. because the tracking cameras see the corresponding target)

      - *Identifier* is a unique name that indicate which user is the active
        user. Some tracking systems can recognize different target and this
        identifiers contains the informations which target was identified and
        chosen as the active user.
    """
    pass

def make_dominant_user_device(user=0, interface="", subdisplay=""):
    """Create a field container that represents the device held in the dominant
    hand of the active user
    
      - *user* selects the n-th *active* user of the display.

      - *interface* is used to select a different field container than the
        default one, i.e. different display may provide more specific or
        incompatible information about the active device. The resulting interface
        will still always contain the *Active* boolean field (see below).

      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the user (and device) of a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Position* is a matrix representing the position of the device in display
        local coordinates.

      - *Button[0-4]* are bool field representing buttons on a device

      - *Active* is a boolean that is true if the device is recognized by the
        system (e.g. because the tracking cameras see the corresponding target)

      - *Identifier* is a unique name that indicate which device is the active
        device. Some tracking systems can recognize different target and this
        identifiers contains the informations which target was identified and
        chosen as the active device.
    """
    device = avango.display.nodes.Device()
    #TODO: add hardwired PDA-Device for Bernini
    if (_display_type == "TouchscreenEmulator" or _display_type == "TwoviewTouchscreenEmulator") and subdisplay == "Touchscreen":
        device.Matrix.connect_from(_touchscreen_camera.MouseNearTransform)
        device.Button1.connect_from(_touchscreen_camera.Window.value.MouseButtons_OnlyLeft)
    elif _display_type == "TwoView":
        device.Matrix.connect_from(avango.display.pda_sensor.Matrix)
    elif _dominant_user_device == "Wiimote1":
        device.Matrix.connect_from(avango.display.wiimote_atek_sensor.Matrix)
    return device

def make_non_dominant_user_device(user=0, interface="", subdisplay=""):
    """Create a field container that represents the device held in the
    non-dominant hand of the active user. See make_dominant_user_device() for
    more information.
    """
    pass

def run():
    _composite_viewer.run()


def make_view(subdisplay=""):
    """Create a field container that represents a viewport into a scene.
    
      - *subdisplay* is used, if the display has multiple disjoint subdisplays
        and the viewport on a specific display has to be selected.

    The resulting (default) field container will have the following fields:

      - *Root* is a matrix representing the position of the device in display
        local coordinates.

      - *Camera* is a matrix describing the translation of the display local
        coordinates into the scene.

      - *Viewport* is a Vec4 describing the relative position on the screen
        (link to diagram).


      - *Depth* is a float value indicating the stacking order of multiple
        views.

      - *UserSelector* contains a name identifier which user should see this
      viewport.

      - *Near* is the near clipping plane.
      
      - *Far* is the near clipping plane.
    """

    display_view = avango.display.nodes.View()

    singlescreen_displays = ["TwoView", "FakeTwoView", "Monitor", "TwoviewTouchscreenEmulator", "TouchscreenEmulator"]

    if subdisplay == "":
        if _display_type in singlescreen_displays:
            for user_counter in range(0, 2):
                osg_view = avango.osg.viewer.nodes.View()
                osg_view.Scene.connect_from(display_view.Root)

                screen_transform = _screen_transforms[0]

                camera = avango.osg.viewer.nodes.Camera()
                camera.EyeOffset.value = 0.03
                camera.ViewerTransform.connect_from(display_view.Camera)
                camera.Near.connect_from(display_view.Near)
                camera.Far.connect_from(display_view.Far)

                splitscreen_handling = SplitscreenHandling()
                splitscreen_handling.ViewportIn.connect_from(display_view.Viewport)
                splitscreen_handling.ScreenTransformIn.value = screen_transform
                camera.ScreenTransform.connect_from(splitscreen_handling.ScreenTransformOut)
                camera.Viewport.connect_from(splitscreen_handling.ViewportOut)

                osg_view_handler = OsgViewHandler()
                osg_view_handler.related_user = str(user_counter+1)
                osg_view_handler.UserSelector.connect_from(display_view.UserSelector)
                camera.Window.connect_from(osg_view_handler.Window)
                camera.EyeTransform.connect_from(osg_view_handler.EyeTransform)

                osg_view.MasterCamera.value = camera
                _composite_viewer.Views.value.append(osg_view)

        elif _display_type == "iCone":
            for i in range(0, len(_windows)):
                osg_view = avango.osg.viewer.nodes.View()
                osg_view.Scene.connect_from(display_view.Root)
                camera = avango.osg.viewer.nodes.Camera()
                camera.EyeOffset.value = 0.03
                camera.Viewport.connect_from(display_view.Viewport)
                camera.ViewerTransform.connect_from(display_view.Camera)
                camera.ScreenTransform.value = _screen_transforms[i]
                camera.Window.value = _windows[i]

                global _eye_vec
                _eye_vec = avango.osg.Vec3(0, 1.7, 0)
                camera.EyeTransform.value = avango.osg.make_trans_mat(_eye_vec)

                osg_view.MasterCamera.value = camera
                _composite_viewer.Views.value.append(osg_view)

    elif subdisplay == "Touchscreen" and (_display_type == "TouchscreenEmulator" or _display_type == "TwoviewTouchscreenEmulator"):
        osg_view = avango.osg.viewer.nodes.View()
        osg_view.Scene.connect_from(display_view.Root)
        camera = avango.osg.viewer.nodes.Camera(Far=3000.)
        camera.ViewerTransform.connect_from(display_view.Camera)
        camera.ScreenTransform.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
        camera.Window.value = _touchscreen_window
        camera.Window.value.Decoration.value = True
        camera.Window.value.ShowCursor.value = True
        camera.Window.value.Title.value = "Touchscreen"
        osg_view.MasterCamera.value = camera
        _composite_viewer.Views.value.append(osg_view)

        global _touchscreen_camera
        _touchscreen_camera = camera

    return display_view


# connects osg_view to a window and eye_connection
class OsgViewHandler(avango.script.Script):
    UserSelector = avango.SFString()
    Window = avango.osg.viewer.SFGraphicsWindow()
    EyeTransform = avango.osg.SFMatrix()

    def __init__(self):
        self.related_user = ""

    def evaluate(self):
        global _display_type
        if self.related_user == "1" and (self.UserSelector.value == "1" or self.UserSelector.value == ""):
            self.Window.value = _windows[0]
            if _display_type == "TwoView":
                self.EyeTransform.connect_from(avango.display.head_sensor_1.Matrix)
            else:
                self.EyeTransform.value = avango.osg.make_trans_mat(_eye_vec)

        elif self.related_user == "2" and (self.UserSelector.value == "2" or self.UserSelector.value == ""):
            if len(_windows) >= 2:
                self.Window.value = _windows[1]
                if _display_type == "TwoView":
                    self.EyeTransform.connect_from(avango.display.head_sensor_2.Matrix)
                else:
                    self.EyeTransform.value = avango.osg.make_trans_mat(_eye_vec)
        else:
            if self.related_user == "" and self.UserSelector.value == "1":
                self.Window.value = _windows[0]
                self.EyeTransform.value = avango.osg.make_trans_mat(_eye_vec)
            elif self.related_user == "" and self.UserSelector.value == "2" and len(_windows) == 2:
                self.Window.value = _windows[1]
                self.EyeTransform.value = avango.osg.make_trans_mat(_eye_vec)


class SplitscreenHandling(avango.script.Script):

    ViewportIn = avango.osg.SFVec4()
    ViewportOut = avango.osg.SFVec4()
    ScreenTransformIn = avango.osg.SFMatrix()
    ScreenTransformOut = avango.osg.SFMatrix()

    def __init__(self):
        self.ScreenTransformIn.value = avango.osg.make_trans_mat(0, 1.2, -2.4)
        self.screenwidth = 3.
        self.screenheight = 2.4

    def evaluate(self):
        x = 1-self.ViewportIn.value.z

        x_trans = 0
        y_trans = 0
        z_trans = 0

        x = self.ViewportIn.value.x
        y = self.ViewportIn.value.y
        z = self.ViewportIn.value.z
        w = self.ViewportIn.value.w
        self.ViewportOut.value = avango.osg.Vec4(x, y,z-x, w-y)

        old_center_x = 0.5*self.screenwidth
        new_center_x = (x + (z-x) / 2) * self.screenwidth
        x_trans = new_center_x - old_center_x

        old_center_y = 0.5*self.screenheight
        new_center_y = (y + (w-y) / 2) * self.screenheight
        y_trans = new_center_y - old_center_y

        self.ScreenTransformOut.value = avango.osg.make_trans_mat(x_trans, y_trans, z_trans) * self.ScreenTransformIn.value


def _make_window(x_position, y_position, width, height, real_width, real_height, use_stereo, screen_identifier=":0.0"):
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
    return window
