import math

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE

SPAWN_TIME = 0.5

class BallSpawner(avango.script.Script):

  TimeIn = avango.SFFloat()
  SceneGraph = avango.gua.SFSceneGraph()
  Physics = avango.gua.SFPhysics()
  MaxBallCount = avango.SFFloat()

  def __init__(self):

    self.super(BallSpawner).__init__()

    self.MaxBallCount.value = 50

    self.__last_spawn_time = -1
    self.__loader = avango.gua.nodes.TriMeshLoader()

    self.__spawned_balls = []

  def evaluate(self):
    global SPAWN_TIME
    current_time = self.TimeIn.value

    if self.__last_spawn_time == -1 or current_time - self.__last_spawn_time >= SPAWN_TIME:
      self.__last_spawn_time = current_time

      light_rb = avango.gua.nodes.RigidBodyNode(
        Name = "light_rb",
        Mass = 1.0,
        Friction = 0.6,
        RollingFriction = 0.03,
        Restitution = 1.0,
        Transform = avango.gua.make_trans_mat(math.sin(current_time) * 0.3, 1.5, 0.0)
      )

      light_geometry = self.__loader.create_geometry_from_file(
        "light_geometry", "data/objects/sphere.obj"
      )

      light_geometry.Transform.value = avango.gua.make_scale_mat(0.1, 0.1, 0.1)

      light_csn = avango.gua.nodes.CollisionShapeNode(
        Name = "light_csn",
        ShapeName = "sphere"
      )

      light_csn.Children.value.append(light_geometry)
      light_rb.Children.value.append(light_csn)
      self.SceneGraph.value.Root.value.Children.value.append(light_rb)
      self.Physics.value.add_rigid_body(light_rb)

      self.__spawned_balls.append(light_rb)

      if len(self.__spawned_balls) > self.MaxBallCount.value:
        to_remove = self.__spawned_balls.pop(0)
        self.Physics.value.remove_rigid_body(to_remove)
        self.SceneGraph.value.Root.value.Children.value.remove(to_remove)


def start():

  physics = avango.gua.nodes.Physics()

  avango.gua.create_sphere_shape("sphere", 0.1)
  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  teapot_rb = avango.gua.nodes.RigidBodyNode(
    Name = "teapot_rb",
    Mass = 0.0
  )

  physics.add_rigid_body(teapot_rb)

  teapot_cs = avango.gua.create_triangle_mesh_shape_from_geometry_file(
    "teapot_cs",
    "data/objects/teapot.obj",
    True,
    False,
    avango.gua.LoaderFlags.DEFAULTS
  )

  teapot_cs.Scaling.value = avango.gua.Vec3(0.5, 0.5, 0.5)

  teapot_csn = avango.gua.nodes.CollisionShapeNode(
    Name = "teapot_csn",
    ShapeName = "teapot_cs"
  )

  loader = avango.gua.nodes.TriMeshLoader()
  teapot_geometry = loader.create_geometry_from_file(
    "teapot_geometry",
    "data/objects/teapot.obj",
    avango.gua.LoaderFlags.OPTIMIZE_GEOMETRY
  )

  teapot_geometry.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.766, 0.336, 1.0))
  teapot_geometry.Material.value.set_uniform("Roughness", 0.3)
  teapot_geometry.Material.value.set_uniform("Metalness", 1.0)
  teapot_geometry.Transform.value = avango.gua.make_scale_mat(0.5, 0.5, 0.5)

  teapot_csn.Children.value.append(teapot_geometry)
  teapot_rb.Children.value.append(teapot_csn)

  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 50.0,
    Transform = avango.gua.make_trans_mat(2, 1, 3) * avango.gua.make_scale_mat(5, 5, 5)
  )

  size = avango.gua.Vec2ui(1920, 1080)

  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  avango.gua.register_window("window", window)

  cam = avango.gua.nodes.CameraNode(
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.5),
    LeftScreenPath = "/screen",
    SceneGraph = "scenegraph",
    Resolution = size,
    OutputWindowName = "window"
  )

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 1.6,
    Height = 0.9,
    Transform = avango.gua.make_trans_mat(0.0, 0.5, 2.0),
    Children = [cam]
  )

  graph.Root.value.Children.value = [teapot_rb, light, screen]

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Physics.value = physics
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()
  spawner = BallSpawner()
  spawner.TimeIn.connect_from(timer.Time)
  spawner.SceneGraph.value = graph
  spawner.Physics.value = physics

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

