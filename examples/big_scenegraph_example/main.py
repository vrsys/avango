import avango
import avango.gua
import time
import math
import random
from avango.script import field_has_changed

from examples_common.GuaVE import GuaVE

# create a simple scene
avango.gua.load_shading_models_from("data/materials")
avango.gua.load_materials_from("data/materials")

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def add_lights(graph, count):

  loader = avango.gua.nodes.TriMeshLoader()

  for x in range(0, count):

    randdir = avango.gua.Vec3(
      random.random() * 2.0 - 1.0,
      random.random() * 2.0 - 1.0,
      random.random() * 2.0 - 1.0
    )

    randdir.normalize()

    sphere_geometry = loader.create_geometry_from_file(
      "sphere" + str(x),
      "data/objects/light_sphere.obj",
      "data/materials/White.gmd",
      avango.gua.LoaderFlags.DEFAULTS
    )

    sphere_geometry.Transform.value = avango.gua.make_scale_mat(0.04, 0.04, 0.04)

    point_light = avango.gua.nodes.PointLightNode(
      Name = "light",
      Color = avango.gua.Color(1, 1, 1),
      Transform = avango.gua.make_identity_mat()
    )

    light_proxy = graph.Root.value.Children.value.append(avango.gua.nodes.TransformNode(
      Name = "light" + str(x),
      Transform = avango.gua.make_trans_mat(randdir[0], randdir[1], randdir[2]),
      Children = [sphere_geometry, point_light]
    ))


def setup_scene(graph, root_monkey, depth_count):

  if (depth_count > 0):
    loader = avango.gua.nodes.TriMeshLoader()

    offset = 2.0

    directions = {
      avango.gua.Vec3(0, offset, 0),
      avango.gua.Vec3(0, -offset, 0),
      avango.gua.Vec3(offset, 0, 0),
      avango.gua.Vec3(-offset, 0, 0),
      avango.gua.Vec3(0, 0, offset),
      avango.gua.Vec3(0, 0, -offset)
    };

    for direction in directions:
      monkey_geometry = loader.create_geometry_from_file(
        "monkey",
        "data/objects/monkey.obj",
        "data/materials/Stones.gmd",
        avango.gua.LoaderFlags.DEFAULTS
      )

      root_monkey.Children.value.append(monkey_geometry)
      monkey_geometry.Transform.value = avango.gua.make_trans_mat(direction[0], direction[1], direction[2]) * avango.gua.make_scale_mat(0.5, 0.5, 0.5)

      setup_scene(graph, monkey_geometry, depth_count - 1)

def start():

  graph = avango.gua.nodes.SceneGraph(
    Name = "big_scenegraph"
  )

  loader = avango.gua.nodes.TriMeshLoader()

  root_node = avango.gua.nodes.TransformNode(
    Name = "Root",
    Transform = avango.gua.make_scale_mat(0.5, 0.5, 0.5)
  )

  root_monkey = loader.create_geometry_from_file(
    "root_ape",
    "data/objects/monkey.obj",
    "data/materials/Stones.gmd",
    avango.gua.LoaderFlags.DEFAULTS
  )

  root_node.Children.value = [root_monkey]
  graph.Root.value.Children.value.append(root_node)

  monkey_updater = TimedRotate()
  timer = avango.nodes.TimeSensor()
  monkey_updater.TimeIn.connect_from(timer.Time)
  root_monkey.Transform.connect_from(monkey_updater.MatrixOut)

  add_lights(graph, 40)

  setup_scene(graph, root_monkey, 4)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 1.6,
    Height = 0.9,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 0.5)
  )

  eye = avango.gua.nodes.TransformNode(
    Name = "eye",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 2.5)
  )


  screen.Children.value = [eye]

  graph.Root.value.Children.value.append(screen)

  camera = avango.gua.nodes.Camera(
    LeftEye = "/screen/eye",
    RightEye = "/screen/eye",
    LeftScreen = "/screen",
    RightScreen = "/screen",
    SceneGraph = "big_scenegraph"
  )

  size = avango.gua.Vec2ui(1500, int(1500*9/16))

  window = avango.gua.nodes.Window(
    Size = size,
    LeftResolution = size
  )

  pipe = avango.gua.nodes.Pipeline(
    Camera = camera,
    Window = window,
    EnableSsao = True,
    SsaoIntensity = 2.0,
    EnableBloom = True,
    BloomThreshold = 0.8,
    BloomRadius = 10.0,
    BloomIntensity = 0.8,
    EnableHDR = True,
    HDRKey = 5,
    LeftResolution = size
  )


  renderer = avango.gua.create_renderer(pipe);

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  viewer.run()



if __name__ == '__main__':
  start()

