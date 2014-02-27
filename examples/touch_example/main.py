#!/usr/bin/python

import math
import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from modules.voronoi_helpers import *
from modules.dynamic_splitscreen import *
from examples_common.GuaVE import GuaVE

width = 1920
size = avango.gua.Vec2ui(width, width * 9 /16)
timer = avango.nodes.TimeSensor()

CUBE_COUNT_X = 10
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 1

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

def add_split_screen(dynamic_split_screens, location):

  current_split_locations = []

  for split in dynamic_split_screens.SplitScreens:
    current_split_locations.append(split.Location.value)

  closest = get_closest(location, current_split_locations)
  closest_camera = dynamic_split_screens.SplitScreens[closest].Pipe.value.Camera.value

  new_split = dynamic_split_screens.add_split_screen(closest_camera, location)

  camera_transform = avango.gua.nodes.TransformNode(
    Name = "camera_transform" + str(new_split.ID.value),
    Transform = dynamic_split_screens.GraphIn.value[closest_camera.LeftScreen.value].Transform.value
  )
  dynamic_split_screens.GraphIn.value.Root.value.Children.value.append(camera_transform)

  camera = add_camera(dynamic_split_screens.GraphIn.value, camera_transform)
  new_split.Pipe.value.Camera.value = camera

  camera_mover = CameraMover(
    MatrixStart = camera_transform.Transform.value
  )
  camera_mover.TimeIn.connect_from(timer.Time)
  dynamic_split_screens.GraphIn.value[camera.LeftScreen.value].Transform.connect_from(camera_mover.MatrixOut)

  return new_split

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

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [split_screens.PipelineOut.value]
  viewer.SceneGraphs.value = [split_screens.GraphOut.value, graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()
