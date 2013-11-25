#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator

class SimpleViewer(avango.script.Script):

  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline = avango.gua.SFPipeline()
  WindowSize = avango.gua.SFVec2ui()

  Navigator = examples_common.navigator.Navigator()

  Viewer = avango.gua.nodes.Viewer()
  Shell = GuaVE()

  def __init__(self):
    self.super(SimpleViewer).__init__()
    self.WindowSize.value = avango.gua.Vec2ui(1920, 1080)

    self.Pipeline.value = avango.gua.nodes.Pipeline(
                            Window = avango.gua.nodes.Window(
                              Size = self.WindowSize.value,
                              LeftResolution = self.WindowSize.value
                            ),

                            LeftResolution = self.WindowSize.value,
                            EnableBackfaceCulling = False,
                            AmbientColor = avango.gua.Color(1, 1, 1),
                            SsaoIntensity = 3,
                            SsaoRadius = 5,
                            EnableSsao = True,
                            EnableBloom = True,
                            BloomRadius = 20,
                            BloomIntensity = 2
                          )


    self.Viewer.Pipelines.value = [self.Pipeline.value]


  def run(self, locals, globals, show_guave_banner = True):
    self.Shell.start(locals, globals, show_guave_banner)
    self.Viewer.run()

  def list_variabels(self):
    self.Shell.list_variables()

  def start_navigation(self):
    self.__navigation.Transform.connect_from(self.Navigator.OutTransform)

  def set_background_image(self, path):
    avango.gua.create_texture(path)
    self.Pipeline.value.BackgroundTexture.value = path

  @field_has_changed(SceneGraph)
  def update_scenegraph(self):

    self.__navigation = avango.gua.nodes.TransformNode(Name = "navigation")

    eye = avango.gua.nodes.TransformNode(Name = "eye")
    eye.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 1.0)

    screen = avango.gua.nodes.ScreenNode(
      Name = "screen",
      Width = 1.6,
      Height = 1.0
    )
    screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, 0.0)

    self.__navigation.Children.value = [eye, screen]

    self.SceneGraph.value.Root.value.Children.value.append(self.__navigation)

    self.Navigator.OutTransform.connect_from(eye.Transform)

    self.Navigator.RotationSpeed.value = 0.2
    self.Navigator.MotionSpeed.value = 0.04



    self.Pipeline.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation/eye",
                                  RightEye = "/navigation/eye",
                                  LeftScreen = "/navigation/screen",
                                  RightScreen = "/navigation/screen",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]

