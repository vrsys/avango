#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
from examples_common.device import TouchDevice
from examples_common.device import TouchCursor

class FingerUpdater(avango.script.Script):

  Cursor = TouchCursor()
  MatrixOut = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(FingerUpdater).__init__()
    self.always_evaluate(True)

  def evaluate(self):
    self.MatrixOut.value = avango.gua.make_trans_mat((self.Cursor.PosX.value-0.5)*16, 0, (self.Cursor.PosY.value-0.5)*9) * avango.gua.make_scale_mat(20)


def add_fingers(count, graph):
  loader = avango.gua.nodes.GeometryLoader()
  touch_device = TouchDevice()

  for i in range(0, 20):
    monkey = loader.create_geometry_from_file("monkey", "data/objects/sphere.obj", "Red", avango.gua.LoaderFlags.DEFAULTS)

    updater = FingerUpdater()
    updater.Cursor = touch_device.TouchCursors[i]
    monkey.Transform.connect_from(updater.MatrixOut)

    graph.Root.value.Children.value.append(monkey)


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

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 25)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 16,
    Height = 9,
    Transform = avango.gua.make_rot_mat(-90, 1, 0, 0)
  )
  screen.Children.value = [eye]

  graph.Root.value.Children.value = [light, screen]

  add_fingers(20, graph)

  # setup viewing
  width = 1920*2
  size = avango.gua.Vec2ui(width, width * 9 /16)
  pipe = avango.gua.nodes.Pipeline(
    Camera = avango.gua.nodes.Camera(
      LeftEye = "/screen/eye",
      LeftScreen = "/screen",
      SceneGraph = "scenegraph",
      Mode = avango.gua.ProjectionMode.ORTHOGRAPHIC
    ),
    Window = avango.gua.nodes.Window(
      Size = size,
      LeftResolution = size
    ),
    LeftResolution = size,
    EnableSsao = True
  )

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()
