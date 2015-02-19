#!/usr/bin/python3

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import sys

class TimedRotate(avango.script.Script):
  TimeIn = avango.SFFloat()
  MatrixOut = avango.gua.SFMatrix4()

  @field_has_changed(TimeIn)
  def update(self):
    self.MatrixOut.value = avango.gua.make_rot_mat(self.TimeIn.value*2.0, 0.0, 1.0, 0.0)

def start(filename):

  # setup scenegraph
  graph = avango.gua.nodes.SceneGraph(Name = "scenegraph")

  plodloader = avango.gua.nodes.PLODLoader()
  plodloader.UploadBudget.value = 32
  plodloader.RenderBudget.value = 2048
  plodloader.OutOfCoreBudget.value = 4096
  plod_geode = plodloader.create_geometry_from_file(
                  filename, avango.gua.PLODLoaderFlags.NORMALIZE_SCALE |
                            avango.gua.PLODLoaderFlags.NORMALIZE_POSITION )
                  #filename, avango.gua.PLODLoaderFlags.DEFAULTS)


  #light = avango.gua.nodes.PointLightNode(Name = "light", Color = avango.gua.Color(1.0, 1.0, 1.0))
  light = avango.gua.nodes.PointLightNode(
    Name = "light",
    Color = avango.gua.Color(1.0, 1.0, 1.0),
    Brightness = 50.0,
    Transform = avango.gua.make_trans_mat(1, 1, 5) * avango.gua.make_scale_mat(15, 15, 15)
  )

  light.Transform.value = avango.gua.make_trans_mat(1, 1, 2) * avango.gua.make_scale_mat(15, 15, 15)

  size = avango.gua.Vec2ui(1024, 768)

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 3.5)


  window = avango.gua.nodes.GlfwWindow(
    Size = size,
    LeftResolution = size
  )

  avango.gua.register_window("window", window)

  cam = avango.gua.nodes.CameraNode(
    LeftScreenPath = "/screen",
    SceneGraph = "scenegraph",
    Resolution = size,
    OutputWindowName = "window",
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 3.5)
  )

  pipeline_description = avango.gua.nodes.PipelineDescription()
  pipeline_description.Passes.value = [
        avango.gua.nodes.TriMeshPassDescription()
      , avango.gua.nodes.TexturedQuadPassDescription()
      , avango.gua.nodes.PLODPassDescription()
      , avango.gua.nodes.LightVisibilityPassDescription()
      , avango.gua.nodes.BBoxPassDescription()
      , avango.gua.nodes.ResolvePassDescription()
      , avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
      ]

  cam.PipelineDescription.value = pipeline_description

  screen = avango.gua.nodes.ScreenNode(Name = "screen", Width = 4, Height = 3)
  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4,
    Height = 3,
    Children = [cam]
  )

  screen.Children.value = [eye]

  graph.Root.value.Children.value = [plod_geode, light, screen]

  # setup viewing
  size = avango.gua.Vec2ui(1024, 768)

  #setup viewer
  viewer = avango.gua.nodes.Viewer()
  viewer.CameraNodes.value = [cam]
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start(sys.argv[1])

