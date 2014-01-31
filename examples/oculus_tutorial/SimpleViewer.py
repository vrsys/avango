#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator
import avango.oculus

# Field container to combine the Oculus transformation matrix with the Keyboard and Mouse transformation matrices
class AccumulateMatrices(avango.script.Script):
  OculusTransform = avango.gua.SFMatrix4()                # Oculus Rift matrix
  NavigationTransform = avango.gua.SFMatrix4()            # Keyboard and mouse matrix
  OutputTransform = avango.gua.SFMatrix4()                # Combined output transformation matrix

  def __init__(self):
    self.super(AccumulateMatrices).__init__()

  def evaluate(self):
    self.OutputTransform.value = self.NavigationTransform.value * self.OculusTransform.value  # Combination using simple matrix multiplication

class SimpleViewer(avango.script.Script):

  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline = avango.gua.SFPipeline()
  WindowSize = avango.gua.SFVec2ui()
  OneEyeWindowSize = avango.gua.SFVec2ui()

  Navigator = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, 5))

  # Saves the current head rotation from the Oculus Rift
  OculusRotation = avango.gua.SFMatrix4()

  Viewer = avango.gua.nodes.Viewer()
  Shell = GuaVE()

  def __init__(self):
    self.super(SimpleViewer).__init__()
    self.WindowSize.value = avango.gua.Vec2ui(1280, 800)                       
    self.OneEyeWindowSize.value = avango.gua.Vec2ui(1280/2, 800)               # For the OR, the width for one eye must be divided by 2

    self.Pipeline.value = avango.gua.nodes.Pipeline(
                            Window = avango.oculus.nodes.OculusRift(           # The window is an OR-window, with ...
                              Size = self.WindowSize.value,                    #   ... the overall window size
                              LeftResolution = self.OneEyeWindowSize.value,    #   ... the left resolution (half the width)
                              RightResolution = self.OneEyeWindowSize.value    #   ... the right resolution (half the width)
                            ),

                            LeftResolution = self.OneEyeWindowSize.value,      # Left resolution has to be set for the pipeline as well
                            RightResolution = self.OneEyeWindowSize.value,     # Right resolution has to be set for the pipeline as well
                            EnableStereo = True,                               # Activate stereo mode
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
    
    # The field container needs to be always evaluated
    self.always_evaluate(True)

  def evaluate(self):
     # The current head rotation of the Oculus Rift is written to the field OculusRotation
     self.OculusRotation.value = self.Pipeline.value.Window.value.Transform.value

  def run(self, locals, globals):
    self.Shell.start(locals, globals)
    self.Viewer.run()

  def list_variabels(self):
    self.Shell.list_variables()

  @field_has_changed(SceneGraph)
  def update_scenegraph(self):
    navigation = avango.gua.nodes.TransformNode(Name = "navigation")

    # For the OR support, we need two eye nodes and two screen nodes that differ slightly in their x translation
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

    # All eyes and screens are added to the navigation node
    navigation.Children.value = [eye_left, eye_right, screen_left, screen_right]

    self.SceneGraph.value.Root.value.Children.value.append(navigation)
    
    self.Navigator.OutTransform.connect_from(eye_left.Transform)

    self.Navigator.RotationSpeed.value = 0.2
    self.Navigator.MotionSpeed.value = 0.04

    # The helper field container is initialized
    matrix_accumulator = AccumulateMatrices()
    
    # The first input of the accumulator is the current head rotation of the Oculus Rift
    matrix_accumulator.OculusTransform.connect_from(self.OculusRotation)

    # The second input of the accumulator is the keyboard and mouse transform value
    matrix_accumulator.NavigationTransform.connect_from(self.Navigator.OutTransform)

    # The navigation's transformation then is the output of the accumulator
    navigation.Transform.connect_from(matrix_accumulator.OutputTransform)

    # Within the pipeline, we have to set the different eyes and screens
    self.Pipeline.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation/eye_left",
                                  RightEye = "/navigation/eye_right",
                                  LeftScreen = "/navigation/screen_left",
                                  RightScreen = "/navigation/screen_right",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]

