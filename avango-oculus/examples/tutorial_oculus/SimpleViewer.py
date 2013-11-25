#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator
import avango.oculus

class OculusTransform(avango.script.Script):
  avango.oculus.initOVR()
  OculusIn = avango.oculus.nodes.OculusRift()
  MatrixOut = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(OculusTransform).__init__()
    self.always_evaluate(True)

  def evaluate(self):
    self.MatrixOut.value = self.OculusIn.Transform.value

class AccumulateMatrices(avango.script.Script):
  OculusTransform = avango.gua.SFMatrix4()
  NavigationTransform = avango.gua.SFMatrix4()
  OutputTransform = avango.gua.SFMatrix4()

  def __init__(self):
    self.super(AccumulateMatrices).__init__()

  def evaluate(self):
    self.OutputTransform.value = self.NavigationTransform.value * self.OculusTransform.value

class SimpleViewer(avango.script.Script):

  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline = avango.gua.SFPipeline()
  WindowSize = avango.gua.SFVec2ui()
  OneEyeWindowSize = avango.gua.SFVec2ui()

  Navigator = examples_common.navigator.Navigator()


  Viewer = avango.gua.nodes.Viewer()
  Shell = GuaVE()

  def __init__(self):
    self.super(SimpleViewer).__init__()
    self.WindowSize.value = avango.gua.Vec2ui(1280, 800)
    self.OneEyeWindowSize.value = avango.gua.Vec2ui(1280/2, 800)

    self.Pipeline.value = avango.gua.nodes.Pipeline(
                            Window = avango.oculus.nodes.OculusRift(
                              Size = self.WindowSize.value,
                              LeftResolution = self.OneEyeWindowSize.value,
                              RightResolution = self.OneEyeWindowSize.value
                            ),

                            LeftResolution = self.OneEyeWindowSize.value,
                            RightResolution = self.OneEyeWindowSize.value,
                            EnableStereo = True,
                            EnableBackfaceCulling = False,
                            AmbientColor = avango.gua.Color(1, 1, 1),
                            SsaoIntensity = 3,
                            SsaoRadius = 5,
                            EnableSsao = True,
                            EnableBloom = True,
                            BloomRadius = 20,
                            BloomIntensity = 2
                          )
    avango.gua.create_texture("data/textures/sky.png")
    self.Pipeline.value.BackgroundTexture.value = "data/textures/sky.png"

    self.Viewer.Pipelines.value = [self.Pipeline.value]


  def run(self, locals, globals):
    self.Shell.start(locals, globals)
    self.Viewer.run()

  def list_variabels(self):
    self.Shell.list_variables()

  @field_has_changed(SceneGraph)
  def update_scenegraph(self):
    navigation = avango.gua.nodes.TransformNode(Name = "navigation")

    eye_left = avango.gua.nodes.TransformNode(Name = "eye_left")
    eye_left.Transform.value = avango.gua.make_trans_mat(-0.032, 0.0, 0.0)

    eye_right = avango.gua.nodes.TransformNode(Name = "eye_right")
    eye_right.Transform.value = avango.gua.make_trans_mat(0.032, 0.0, 0.0)

    screen_left = avango.gua.nodes.ScreenNode(
      Name = "screen_left",
      Width = 0.08,
      Height = 0.1
    )
    screen_left.Transform.value = avango.gua.make_trans_mat(-0.04, 0.0, -0.05)

    screen_right = avango.gua.nodes.ScreenNode(
      Name = "screen_right",
      Width = 0.08,
      Height = 0.1
    )
    screen_right.Transform.value = avango.gua.make_trans_mat(0.04, 0.0, -0.05)

    navigation.Children.value = [eye_left, eye_right, screen_left, screen_right]

    self.SceneGraph.value.Root.value.Children.value.append(navigation)
    
    self.Navigator.OutTransform.connect_from(eye_left.Transform)

    self.Navigator.RotationSpeed.value = 0.2
    self.Navigator.MotionSpeed.value = 0.04

    oculus_updater = OculusTransform()
    matrix_accumulator = AccumulateMatrices()
    
    matrix_accumulator.OculusTransform.connect_from(oculus_updater.MatrixOut)
    matrix_accumulator.NavigationTransform.connect_from(self.Navigator.OutTransform)

    navigation.Transform.connect_from(matrix_accumulator.OutputTransform)

    self.Pipeline.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation/eye_left",
                                  RightEye = "/navigation/eye_right",
                                  LeftScreen = "/navigation/screen_left",
                                  RightScreen = "/navigation/screen_right",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]

