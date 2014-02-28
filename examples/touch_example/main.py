#!/usr/bin/python

import math
import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import modules.voronoi_helpers
import scene
from modules.dynamic_splitscreen import *
from modules.gesture_detectors import *
from examples_common.GuaVE import GuaVE

width = 1920*2
size = avango.gua.Vec2ui(width, width * 9 /16)

class TouchHandler(avango.script.Script):

  # Callback functions
  OnFingerDown    = None
  OnFingerUP      = None
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
    self.__dragged_split_screens = []

    for cursor in self.__touch_device.TouchCursors:
      self.__last_cursor_states.append(cursor.State.value)
      tap_detector = TapDetector()
      tap_detector.Cursor = cursor
      self.__tap_detectors.append(tap_detector)
      self.__dragged_split_screens.append((None, None))
      self.__double_tap_detector.add_cursor(cursor)

  def evaluate(self):

    # Check for fingers moving on the surface
    for i in range(0, len(self.__touch_device.TouchCursors)):

      # A finger recently touched the surface
      if self.__last_cursor_states[i] == 4 and \
         self.__touch_device.TouchCursors[i].State.value != 4 and \
         self.OnFingerDown:

        self.OnFingerDown(self.__touch_device.TouchCursors[i])


        # new_pos = avango.gua.Vec2(self.__touch_device.TouchCursors[i].PosX.value,
        #                           self.__touch_device.TouchCursors[i].PosY.value) - 0.5
        # closest_screen, distance = self.__get_closest_screen(new_pos)

        # A screen POI has been hit and will be dragged
        # if distance < 0.01:
        #   self.__dragged_split_screens[i] = (closest_screen, self.__touch_device.TouchCursors[i])

      # A finger recently left the surface
      elif self.__last_cursor_states[i] != 4 and \
           self.__touch_device.TouchCursors[i].State.value == 4 and \
           self.OnFingerUP:

          self.OnFingerUP(self.__touch_device.TouchCursors[i])
        # self.__dragged_split_screens[i] = (None, None)

      elif self.__touch_device.TouchCursors[i].State.value != 4 and \
           self.OnFingerMoved:

           self.OnFingerMoved(self.__touch_device.TouchCursors[i])

      self.__last_cursor_states[i] = self.__touch_device.TouchCursors[i].State.value

    # Apply finger transformations to all dragged screens
    # for pair in self.__dragged_split_screens:
    #   if pair[0]:
    #     pair[0].Location.value =  avango.gua.Vec2(pair[1].PosX.value, pair[1].PosY.value) - 0.5

    # Check if any double tap occured
    if len(self.__double_tap_detector.DoubleTapPositions.value) > 0:
      for position in self.__double_tap_detector.DoubleTapPositions.value:
        if self.OnDoubleTap:
          self.OnDoubleTap(position)
        # new_pos = position - 0.5
        # closest_screen, distance = self.__get_closest_screen(new_pos)

        # # A new screen POI is inserted
        # if distance > 0.1:
        #   camera = closest_screen.Pipe.value.Camera.value
        #   self.SplitScreens.add_split_screen(camera, new_pos)

        # # A screen POI has been hit and is therefore removed
        # elif len(self.SplitScreens.SplitScreens) > 1:
        #   self.SplitScreens.remove_split_screen(closest_screen)

      self.__double_tap_detector.reset()

    # Check if any tap occured
    else:
      for detector in self.__tap_detectors:
        if detector.TapDetected.value == True and self.OnTap:
          self.OnTap(detector.TapPosition.value)
          detector.reset()



  def __get_closest_screen(self, position):
    current_split_locations = []

    for split in self.SplitScreens.SplitScreens:
      current_split_locations.append(split.Location.value)

    closest, distance = modules.voronoi_helpers.get_closest(position, current_split_locations)

    return self.SplitScreens.SplitScreens[closest], distance

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

  parent.Children.value.append(screen)

  camera = avango.gua.nodes.Camera(
    LeftEye = eye.Path.value,
    LeftScreen = screen.Path.value,
    SceneGraph = graph.Name.value
  )

  return camera

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph = scene.create()

  camera = add_camera(graph, graph.Root.value)

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

  root_split_screen = split_screens.add_split_screen(camera, avango.gua.Vec2(0, 0))

  touch_handler = TouchHandler()
  touch_handler.SplitScreens = split_screens

  touch_handler.OnTap = lambda position:
    print "TAP", position

  touch_handler.OnDoubleTap = lambda position:
    print "DOUBLE TAP", position

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
