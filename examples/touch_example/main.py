#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from modules.voronoi_helpers import *
from modules.dynamic_splitscreen import *
from examples_common.GuaVE import GuaVE

width = 1920*2
size = avango.gua.Vec2ui(width, width * 9 /16)

CUBE_COUNT_X = 10
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 10

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)


def create_pipeline(id, graph):
  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.8)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen" + str(id),
    Width = 1.6,
    Height = 0.9,
    Transform = avango.gua.make_trans_mat(id*0.2, 0, 10) * avango.gua.make_rot_mat(0, 1, 0, 0)
  )
  screen.Children.value = [eye]

  graph.Root.value.Children.value.append(screen)

  pipe = avango.gua.nodes.Pipeline(
    Camera = avango.gua.nodes.Camera(
      LeftEye = "/screen" + str(id) + "/eye",
      LeftScreen = "/screen" + str(id),
      SceneGraph = "scenegraph"
    ),
    LeftResolution = size,
    BackgroundTexture = "data/textures/checker.png",
    BackgroundMode = avango.gua.BackgroundMode.SKYMAP_TEXTURE
  )

  return pipe

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

  proxy_graph, proxy_pipe = add_dynamic_split_screens(5, create_pipeline, graph, size)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [proxy_pipe]
  viewer.SceneGraphs.value = [proxy_graph, graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()
