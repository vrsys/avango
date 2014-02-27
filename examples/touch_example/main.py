#!/usr/bin/python

import math
import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import modules.voronoi_helpers
from modules.dynamic_splitscreen import *
from modules.gesture_detectors import *
from examples_common.GuaVE import GuaVE

width = 1920*2
size = avango.gua.Vec2ui(width, width * 9 /16)
timer = avango.nodes.TimeSensor()

CUBE_COUNT_X = 10
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 1

class TouchHandler(avango.script.Script):

  SplitScreens = DynamicSplitScreens()

  def __init__(self):
    self.super(TouchHandler).__init__()
    self.always_evaluate(True)

    self.__touch_device = TouchDevice()
    self.__double_tap_detector = DoubleTapDetector()

    self.__last_cursor_states = []
    self.__dragged_split_screens = []

    for cursor in self.__touch_device.TouchCursors:
      self.__last_cursor_states.append(cursor.State.value)
      self.__dragged_split_screens.append((None, None))
      self.__double_tap_detector.add_cursor(cursor)

  def evaluate(self):

    if len(self.__double_tap_detector.DoubleTapPositions.value) > 0:
      for position in self.__double_tap_detector.DoubleTapPositions.value:

        new_pos = position - 0.5
        closest_screen, distance = self.__get_clostes_screen(new_pos)
        if distance > 0.1:
          camera = closest_screen.Pipe.value.Camera.value
          self.SplitScreens.add_split_screen(camera, new_pos)
        elif len(self.SplitScreens.SplitScreens) > 1:
          self.SplitScreens.remove_split_screen(closest_screen)

      self.__double_tap_detector.reset()

    else:
      for i in range(0, len(self.__touch_device.TouchCursors)):
        if self.__last_cursor_states[i] == 4 and \
           self.__touch_device.TouchCursors[i].State.value != 4:

          new_pos = avango.gua.Vec2(self.__touch_device.TouchCursors[i].PosX.value,
                                    self.__touch_device.TouchCursors[i].PosY.value) - 0.5
          closest_screen, distance = self.__get_clostes_screen(new_pos)

          if distance < 0.01:
            self.__dragged_split_screens[i] = (closest_screen, self.__touch_device.TouchCursors[i])

        elif self.__last_cursor_states[i] != 4 and \
             self.__touch_device.TouchCursors[i].State.value == 4:

          self.__dragged_split_screens[i] = (None, None)

        self.__last_cursor_states[i] = self.__touch_device.TouchCursors[i].State.value

      for pair in self.__dragged_split_screens:
        if pair[0]:
          pair[0].Location.value =  avango.gua.Vec2(pair[1].PosX.value, pair[1].PosY.value) - 0.5

  def __get_clostes_screen(self, position):
    current_split_locations = []

    for split in self.SplitScreens.SplitScreens:
      current_split_locations.append(split.Location.value)

    closest, distance = modules.voronoi_helpers.get_closest(position, current_split_locations)

    return self.SplitScreens.SplitScreens[closest], distance

class CameraMover(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  MatrixStart = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = self.MatrixStart.value * \
      avango.gua.make_rot_mat(math.sin(self.TimeIn.value*0.5)*2, 0, 1, 0) * \
      avango.gua.make_trans_mat(math.sin(self.TimeIn.value*0.5)*2.0, 0.0, math.cos(self.TimeIn.value*0.5)*2.0 - 2.0)


def add_camera(graph, parent):
  eye = avango.gua.nodes.TransformNode(
    Name = "eye",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 4)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 8,
    Height = 4.5,
    Children = [eye]
  )

  camera_mover = CameraMover(
    MatrixStart = avango.gua.make_trans_mat(0, 0, 3)
  )
  camera_mover.TimeIn.connect_from(timer.Time)
  screen.Transform.connect_from(camera_mover.MatrixOut)

  parent.Children.value.append(screen)

  camera = avango.gua.nodes.Camera(
    LeftEye = eye.Path.value,
    LeftScreen = screen.Path.value,
    SceneGraph = graph.Name.value
  )

  return camera

def create_pipeline():
  return avango.gua.nodes.Pipeline(
    LeftResolution = size,
    BackgroundTexture = "data/textures/checker.png",
    BackgroundMode = avango.gua.BackgroundMode.SKYMAP_TEXTURE,
    AmbientColor = avango.gua.Color(1, 1, 1)
  )

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  light = avango.gua.nodes.SunLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0)
  )
  graph.Root.value.Children.value.append(light)

  loader = avango.gua.nodes.GeometryLoader()
  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):
        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/monkey.obj",
                  "White",
                  avango.gua.LoaderFlags.DEFAULTS)

        new_cube.Transform.value = avango.gua.make_trans_mat((x - CUBE_COUNT_X/2)*2, (y - CUBE_COUNT_Y/2)*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)


  camera = add_camera(graph, graph.Root.value)

  split_screens = DynamicSplitScreens(
    GraphIn = graph,
    WindowSize = size
  )

  split_screens.init(5, create_pipeline)
  root_split_screen = split_screens.add_split_screen(camera, avango.gua.Vec2(0, 0))

  #add_split_screen(split_screens, avango.gua.Vec2(0.3, 0.3))

  touch_handler = TouchHandler()
  touch_handler.SplitScreens = split_screens


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
