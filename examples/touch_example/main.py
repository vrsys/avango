#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from modules.voronoi_helpers import *
from modules.dynamic_splitscreen import *
from examples_common.GuaVE import GuaVE

width = 1920
size = avango.gua.Vec2ui(width, width * 9 /16)

CUBE_COUNT_X = 10
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 1

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

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
  # timer = avango.nodes.TimeSensor()

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

  # monkey_updater = TimedRotate()
  # monkey_updater.TimeIn.connect_from(timer.Time)
  # monkey.Transform.connect_from(monkey_updater.MatrixOut)

  eye = avango.gua.nodes.TransformNode(
    Name = "eye",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 4)
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 8,
    Height = 4.5,
    Transform = avango.gua.make_trans_mat(0, 0, 3),
    Children = [eye]
  )

  graph.Root.value.Children.value.append(screen)

  camera = avango.gua.nodes.Camera(
    LeftEye = "/screen/eye",
    LeftScreen = "/screen",
    SceneGraph = graph.Name.value
  )

  split_screens = DynamicSplitScreens(
    GraphIn = graph,
    WindowSize = size
  )

  split_screens.init(5, create_pipeline)

  split_screens.add_split_screen(camera, avango.gua.Vec2(0, 0))
  split_screens.add_split_screen(camera, avango.gua.Vec2(0.5, 0.5))
  split_screens.add_split_screen(camera, avango.gua.Vec2(-0.5, -0.5))

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [split_screens.PipelineOut.value]
  viewer.SceneGraphs.value = [split_screens.GraphOut.value, graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()
