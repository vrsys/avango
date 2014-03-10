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
    self.OutputTransform.value = avango.gua.make_trans_mat(self.NavigationTransform.value.get_translate()) * self.OculusTransform.value  # Combination using simple matrix multiplication

class OculusDualViewer(avango.script.Script):

  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline1 = avango.gua.SFPipeline()
  Pipeline2 = avango.gua.SFPipeline()

  WindowSize = avango.gua.SFVec2ui()
  OneEyeWindowSize = avango.gua.SFVec2ui()

  Navigator1 = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, 5))
  Navigator2 = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, -5))

  # Saves the current head rotation from the Oculus Rift
  OculusRotation1 = avango.gua.SFMatrix4()
  OculusRotation2 = avango.gua.SFMatrix4()

  # Player positions
  Player1Position = avango.gua.SFMatrix4()
  Player2Position = avango.gua.SFMatrix4()

  Viewer = avango.gua.nodes.Viewer()
  Shell = GuaVE()

  def __init__(self):
    self.super(OculusDualViewer).__init__()
    self.WindowSize.value = avango.gua.Vec2ui(1280, 800)                       
    self.OneEyeWindowSize.value = avango.gua.Vec2ui(1280/2, 800)               # For the OR, the width for one eye must be divided by 2

    self.Pipeline1.value = avango.gua.nodes.Pipeline(
                            Window = avango.oculus.nodes.OculusRift(           # The window is an OR-window, with ...
                              Size = self.WindowSize.value,                    #   ... the overall window size
                              LeftResolution = self.OneEyeWindowSize.value,    #   ... the left resolution (half the width)
                              RightResolution = self.OneEyeWindowSize.value    #   ... the right resolution (half the width)
                            ),

                            LeftResolution = self.OneEyeWindowSize.value,      # Left resolution has to be set for the pipeline as well
                            RightResolution = self.OneEyeWindowSize.value,     # Right resolution has to be set for the pipeline as well
                            EnableStereo = True,                               # Activate stereo mode
                            EnableBackfaceCulling = True,
                            AmbientColor = avango.gua.Color(1, 1, 1),
                            SsaoIntensity = 3,
                            SsaoRadius = 5,
                            EnableSsao = True,
                            EnableBloom = True,
                            BloomRadius = 20,
                            BloomIntensity = 2
                          )
    avango.gua.create_texture("data/textures/sky.png")
    self.Pipeline1.value.BackgroundTexture.value = "data/textures/sky.png"

    self.Pipeline2.value = avango.gua.nodes.Pipeline(
                            Window = avango.oculus.nodes.OculusRift(           # The window is an OR-window, with ...
                              Size = self.WindowSize.value,                    #   ... the overall window size
                              LeftResolution = self.OneEyeWindowSize.value,    #   ... the left resolution (half the width)
                              RightResolution = self.OneEyeWindowSize.value    #   ... the right resolution (half the width)
                            ),

                            LeftResolution = self.OneEyeWindowSize.value,      # Left resolution has to be set for the pipeline as well
                            RightResolution = self.OneEyeWindowSize.value,     # Right resolution has to be set for the pipeline as well
                            EnableStereo = True,                               # Activate stereo mode
                            EnableBackfaceCulling = True,
                            AmbientColor = avango.gua.Color(1, 1, 1),
                            SsaoIntensity = 3,
                            SsaoRadius = 5,
                            EnableSsao = True,
                            EnableBloom = True,
                            BloomRadius = 20,
                            BloomIntensity = 2
                          )
    self.Pipeline2.value.BackgroundTexture.value = "data/textures/sky.png"

    self.Viewer.Pipelines.value = [self.Pipeline1.value, self.Pipeline2.value]
    
    # The field container needs to be always evaluated
    self.always_evaluate(True)

  def evaluate(self):
     # The current head rotation of the Oculus Rift is written to the field OculusRotation
     self.OculusRotation1.value = self.Pipeline1.value.Window.value.Transform.value
     self.OculusRotation2.value = avango.gua.make_rot_mat(180, 0, 1, 0) * self.Pipeline2.value.Window.value.Transform.value

  def run(self, locals, globals):
    self.Shell.start(locals, globals)
    self.Viewer.run()

  def list_variabels(self):
    self.Shell.list_variables()

  @field_has_changed(SceneGraph)
  def update_scenegraph(self):
    navigation1 = avango.gua.nodes.TransformNode(Name = "navigation1")
    navigation2 = avango.gua.nodes.TransformNode(Name = "navigation2")

    # For the OR support, we need two eye nodes and two screen nodes that differ slightly in their x translation
    eye_left1 = avango.gua.nodes.TransformNode(Name = "eye_left1")
    eye_left1.Transform.value = avango.gua.make_trans_mat(-0.032, 0.0, 0.0)

    eye_left2 = avango.gua.nodes.TransformNode(Name = "eye_left2")
    eye_left2.Transform.value = avango.gua.make_trans_mat(-0.032, 0.0, 0.0)

    eye_right1 = avango.gua.nodes.TransformNode(Name = "eye_right1")
    eye_right1.Transform.value = avango.gua.make_trans_mat(0.032, 0.0, 0.0)

    eye_right2 = avango.gua.nodes.TransformNode(Name = "eye_right2")
    eye_right2.Transform.value = avango.gua.make_trans_mat(0.032, 0.0, 0.0)

    screen_left1 = avango.gua.nodes.ScreenNode(
      Name = "screen_left1",
      Width = 0.08,
      Height = 0.1
    )
    screen_left1.Transform.value = avango.gua.make_trans_mat(-0.04, 0.0, -0.05)

    screen_left2 = avango.gua.nodes.ScreenNode(
      Name = "screen_left2",
      Width = 0.08,
      Height = 0.1
    )
    screen_left2.Transform.value = avango.gua.make_trans_mat(-0.04, 0.0, -0.05)

    screen_right1 = avango.gua.nodes.ScreenNode(
      Name = "screen_right1",
      Width = 0.08,
      Height = 0.1
    )
    screen_right1.Transform.value = avango.gua.make_trans_mat(0.04, 0.0, -0.05)

    screen_right2 = avango.gua.nodes.ScreenNode(
      Name = "screen_right2",
      Width = 0.08,
      Height = 0.1
    )
    screen_right2.Transform.value = avango.gua.make_trans_mat(0.04, 0.0, -0.05)

    # All eyes and screens are added to the navigation node
    navigation1.Children.value = [eye_left1, eye_right1, screen_left1, screen_right1]
    navigation2.Children.value = [eye_left2, eye_right2, screen_left2, screen_right2]

    self.SceneGraph.value.Root.value.Children.value.append(navigation1)
    self.SceneGraph.value.Root.value.Children.value.append(navigation2)
    
    self.Navigator1.OutTransform.connect_from(eye_left1.Transform)
    self.Navigator2.OutTransform.connect_from(eye_left2.Transform)

    self.Navigator1.RotationSpeed.value = 0.2
    self.Navigator1.MotionSpeed.value = 0.04
    self.Navigator2.RotationSpeed.value = 0.2
    self.Navigator2.MotionSpeed.value = 0.04

    # The helper field container is initialized
    matrix_accumulator1 = AccumulateMatrices()
    matrix_accumulator2 = AccumulateMatrices()
    
    # The first input of the accumulator is the current head rotation of the Oculus Rift
    matrix_accumulator1.OculusTransform.connect_from(self.OculusRotation1)
    matrix_accumulator2.OculusTransform.connect_from(self.OculusRotation2)

    # The second input of the accumulator is the keyboard and mouse transform value
    matrix_accumulator1.NavigationTransform.connect_from(self.Navigator1.OutTransform)
    matrix_accumulator2.NavigationTransform.connect_from(self.Navigator2.OutTransform)

    # The navigation's transformation then is the output of the accumulator
    self.Player1Position.connect_from(matrix_accumulator1.OutputTransform)
    navigation1.Transform.connect_from(self.Player1Position)

    self.Player2Position.connect_from(matrix_accumulator2.OutputTransform)
    navigation2.Transform.connect_from(self.Player2Position)

    # Within the pipeline, we have to set the different eyes and screens
    self.Pipeline1.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation1/eye_left1",
                                  RightEye = "/navigation1/eye_right1",
                                  LeftScreen = "/navigation1/screen_left1",
                                  RightScreen = "/navigation1/screen_right1",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Pipeline2.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation2/eye_left2",
                                  RightEye = "/navigation2/eye_right2",
                                  LeftScreen = "/navigation2/screen_left2",
                                  RightScreen = "/navigation2/screen_right2",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]