import math
import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import modules.voronoi_helpers
import scene
from modules.dynamic_splitscreen import *
from modules.gesture_detectors import *
from modules.animation import *
from examples_common.GuaVE import GuaVE

width = 1920
size = avango.gua.Vec2ui(width, width * 9 /16)

class SplitScreenHandler(avango.script.Script):

  SplitScreens    = DynamicSplitScreens()

  OnFingerDown    = None
  OnFingerUp      = None
  OnFingerMoved   = None

  OnTap           = None
  OnDoubleTap     = None

  def __init__(self):
    self.super(SplitScreenHandler).__init__()
    self.always_evaluate(True)

    self.__dragged_split_screens = dict()
    self.__touch_handler = TouchHandler()
    self.__touch_handler.OnDoubleTap = self.on_double_tap
    self.__touch_handler.OnFingerDown = self.on_finger_down
    self.__touch_handler.OnFingerUp = self.on_finger_up
    self.__touch_handler.OnFingerMoved = self.on_finger_moved

  def on_double_tap(self, position):
    new_pos = position - 0.5
    closest_screen, distance = self.__get_closest_screen(new_pos)

    # A new screen POI is inserted
    if distance > 0.1:
      camera = closest_screen.Pipe.value.Camera.value
      self.SplitScreens.add_split_screen(camera, new_pos)

    # A screen POI has been hit and is therefore removed
    elif len(self.SplitScreens.SplitScreens) > 1:
      self.SplitScreens.remove_split_screen(closest_screen)

    elif self.OnDoubleTap:
      self.OnDoubleTap(position, closest_screen)

  def on_finger_down(self, cursor):
    new_pos = avango.gua.Vec2(cursor.PosX.value,
                              cursor.PosY.value) - 0.5
    closest_screen, distance = self.__get_closest_screen(new_pos)

    # A screen POI has been hit and will be dragged
    if distance < 0.01:
      self.__dragged_split_screens[cursor.CursorID.value] = closest_screen

    elif self.OnFingerDown:
      self.OnFingerDown(cursor, closest_screen)

  def on_finger_up(self, cursor):
    if cursor.CursorID.value in self.__dragged_split_screens:
      del self.__dragged_split_screens[cursor.CursorID.value]

    elif self.OnFingerUp:
      self.OnFingerUp(cursor)

  def on_finger_moved(self, cursor):
    # Apply finger transformations to all dragged screens
    if cursor.CursorID.value in self.__dragged_split_screens:
      self.__dragged_split_screens[cursor.CursorID.value].Location.value =  \
        avango.gua.Vec2(cursor.PosX.value, cursor.PosY.value) - 0.5

    elif self.OnFingerMoved:
      new_pos = avango.gua.Vec2(cursor.PosX.value,
                                cursor.PosY.value) - 0.5
      closest_screen, distance = self.__get_closest_screen(new_pos)
      self.OnFingerMoved(cursor, closest_screen)

  def __get_closest_screen(self, position):
    current_split_locations = []

    for split in self.SplitScreens.SplitScreens:
      current_split_locations.append(split.Location.value)

    closest, distance = modules.voronoi_helpers.get_closest(position, current_split_locations)

    return self.SplitScreens.SplitScreens[closest], distance

class TouchHandler(avango.script.Script):

  # Callback functions
  OnFingerDown    = None
  OnFingerUp      = None
  OnFingerMoved   = None

  OnTap           = None
  OnDoubleTap     = None

  SplitScreens    = DynamicSplitScreens()

  def __init__(self):
    self.super(TouchHandler).__init__()
    self.always_evaluate(True)

    self.__touch_device = TouchDevice()
    self.__tap_detectors = []
    self.__double_tap_detector = DoubleTapDetector()

    self.__last_cursor_states = []

    for cursor in self.__touch_device.TouchCursors:
      self.__last_cursor_states.append(cursor.State.value)
      tap_detector = TapDetector()
      tap_detector.Cursor = cursor
      self.__tap_detectors.append(tap_detector)
      self.__double_tap_detector.add_cursor(cursor)

  def evaluate(self):

    # Check for fingers moving on the surface
    for i in range(0, len(self.__touch_device.TouchCursors)):

      # A finger recently touched the surface
      if self.__last_cursor_states[i] == 4 and \
         self.__touch_device.TouchCursors[i].State.value != 4 and \
         self.OnFingerDown:

        self.OnFingerDown(self.__touch_device.TouchCursors[i])

      # A finger recently left the surface
      elif self.__last_cursor_states[i] != 4 and \
           self.__touch_device.TouchCursors[i].State.value == 4 and \
           self.OnFingerUp:

          self.OnFingerUp(self.__touch_device.TouchCursors[i])

      elif self.__touch_device.TouchCursors[i].State.value != 4 and \
           self.OnFingerMoved:

           self.OnFingerMoved(self.__touch_device.TouchCursors[i])

      self.__last_cursor_states[i] = self.__touch_device.TouchCursors[i].State.value

    # Check if any double tap occured
    if len(self.__double_tap_detector.DoubleTapPositions.value) > 0:
      for position in self.__double_tap_detector.DoubleTapPositions.value:
        if self.OnDoubleTap:
          self.OnDoubleTap(position)

      self.__double_tap_detector.reset()

    # Check if any tap occured
    else:
      for detector in self.__tap_detectors:
        if detector.TapDetected.value == True and self.OnTap:
          self.OnTap(detector.TapPosition.value)
          detector.reset()




def remove_split(graph, split_screen, split_screens):
  split_screens.remove_split_screen(split_screen)
  camera = split_screen.Pipe.value.Camera.value
  screen = graph[camera.LeftScreen.value]

  screen.Parent.value.Children.value.remove(screen)


def create_split_screen(graph, position, split_screens, screen_size):

  # find camera which is going to be splitted
  closest, distance = split_screens.get_closest(position)

  orig_camera = closest.Pipe.value.Camera.value
  orig_screen = graph[orig_camera.LeftScreen.value]
  orig_eye    = graph[orig_camera.LeftEye.value]

  # calculate target position of new camera and create animation
  split_camera_pos = avango.gua.Vec3(
     position.x*screen_size.x,
    -position.y*screen_size.y, 4
  )

  anim = Animation(
    Duration  = 1000.0,
    StartPos  = orig_eye.Transform.value.get_translate(),
    EndPos    = split_camera_pos,
    StartRot  = orig_eye.Transform.value.get_rotate(),
    EndRot    = orig_eye.Transform.value.get_rotate()
  )

  # create eye node
  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.connect_from(anim.CurrentMatrix)

  # create screen node
  screen = avango.gua.nodes.ScreenNode(
    Name      = "screen" + str(split_screens.get_next_id()),
    Width     = orig_screen.Width.value,
    Height    = orig_screen.Height.value,
    Children  = [eye],
    Transform = orig_screen.Transform.value
  )

  graph.Root.value.Children.value.append(screen)

  # return the newly created camera
  camera = avango.gua.nodes.Camera(
    LeftEye     = eye.Path.value,
    LeftScreen  = screen.Path.value,
    SceneGraph  = graph.Name.value
  )

  split_screen = split_screens.add_split_screen(camera, position)

  # class EyeTransformer(avango.script.Script):
  #   TouchPos   = avango.gua.SFVec2()
  #   EyePos     = avango.gua.SFVec3()
  #   ScreenSize = avango.gua.SFVec2()

  #   @field_has_changed(TouchPos)
  #   def update(self):
  #     self.EyePos.value = avango.gua.Vec3(
  #        self.TouchPos.value.x*self.ScreenSize.value.x,
  #       -self.TouchPos.value.y*self.ScreenSize.value.y, 4
  #     )

  # eye_transformer = EyeTransformer(
  #   ScreenSize = screen_size
  # )

  # eye_transformer.TouchPos.connect_from(split_screen.Location)
  # anim.EndPos.connect_from(eye_transformer.EyePos)

  return split_screen


def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph = scene.create()

  eye = avango.gua.nodes.TransformNode(
    Name = "eye",
    Transform = avango.gua.make_trans_mat(0, 0, 4)
  )

  screen_size = avango.gua.Vec2(8, 4.5)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = screen_size.x,
    Height = screen_size.y,
    Children = [eye]
  )

  graph.Root.value.Children.value.append(screen)

  camera = avango.gua.nodes.Camera(
    LeftEye = eye.Path.value,
    LeftScreen = screen.Path.value,
    SceneGraph = graph.Name.value
  )

  split_screens = DynamicSplitScreens(
    GraphIn = graph,
    WindowSize = size
  )

  split_screens.init(5, lambda:
    avango.gua.nodes.Pipeline(
      LeftResolution = size,
      BackgroundTexture = "data/textures/checker.png",
      BackgroundMode = avango.gua.BackgroundMode.SKYMAP_TEXTURE,
      AmbientColor = avango.gua.Color(1, 1, 1)
    )
  )

  split_screens.add_split_screen(camera, avango.gua.Vec2(0, 0))

  def split(split_pos):
    return create_split_screen(graph, split_pos, split_screens, screen_size)

  def unsplit(split_screen):
    remove_split(graph, split_screen, split_screens)


  split_screen_handler = SplitScreenHandler()
  split_screen_handler.SplitScreens = split_screens

  #setup viewer
  viewer = avango.gua.nodes.Viewer(
    DesiredFPS = 50
  )
  viewer.Pipelines.value = [split_screens.PipelineOut.value]
  viewer.SceneGraphs.value = [split_screens.GraphOut.value, graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()
