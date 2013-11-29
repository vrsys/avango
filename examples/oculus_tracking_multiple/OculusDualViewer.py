#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator
import avango.oculus

# A field container that fetches the translation matrix from the tracking data
class TranslationTransform1(avango.script.Script):
  MatrixOut = avango.gua.SFMatrix4()                # Output transformation matrix

  def __init__(self):
    self.super(TranslationTransform1).__init__()
    self.always_evaluate(True)                      # has to be set to true in order to update transformation matrix regularly

    # init headtracking sensor at LCD wall
    self.headtracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    self.headtracking_sensor.Station.value = "tracking-oculus-1"
    self.headtracking_sensor.TransmitterOffset.value = avango.gua.make_identity_mat()
    self.headtracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

  def evaluate(self):
    _mat = self.headtracking_sensor.Matrix.value
    _factor = 6

    # The output matrix is the captured translation matrix (amplified with a scaling factor)
    self.MatrixOut.value = avango.gua.make_trans_mat(_mat.get_translate() * _factor)

# A field container that fetches the translation matrix from the tracking data
class TranslationTransform2(avango.script.Script):
  MatrixOut = avango.gua.SFMatrix4()                # Output transformation matrix

  def __init__(self):
    self.super(TranslationTransform2).__init__()
    self.always_evaluate(True)                      # has to be set to true in order to update transformation matrix regularly

    # init headtracking sensor at LCD wall
    self.headtracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    self.headtracking_sensor.Station.value = "tracking-oculus-2"
    self.headtracking_sensor.TransmitterOffset.value = avango.gua.make_identity_mat()
    self.headtracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

  def evaluate(self):
    _mat = self.headtracking_sensor.Matrix.value
    _factor = 6

    # The output matrix is the captured translation matrix (amplified with a scaling factor)
    self.MatrixOut.value = avango.gua.make_trans_mat(_mat.get_translate() * _factor)

# A field container that takes Oculus rotation, tracking translation and user controlled transformation and
# accumulates them to the final transformation matrix
class MatrixAccumulator(avango.script.Script):
  OculusRotation      = avango.gua.SFMatrix4()          # Rotation matrix from Oculus Rift
  TrackingTranslation = avango.gua.SFMatrix4()          # Translation matrix from the AR-Track software
  UserMovement        = avango.gua.SFMatrix4()          # Translation matrix from user input (keyboard, etc.)
  MatrixOut           = avango.gua.SFMatrix4()          # Output matrix as the combination of the matrices above                      

  def __init__(self):
    self.super(MatrixAccumulator).__init__()
    self.FallingMatrix = avango.gua.make_trans_mat(0, -0.005, 0)      # Matrix used when the user steps of the platform
    self.Falling = False                                              # Boolean variable to indicate if the user is falling
    self.FallingCoefficient = 0.005                                   # Falling speed, increased the longer you fall
    self.always_evaluate(True)

  def evaluate(self):
    # Extract the both translations from the matrices for further processing
    _user_translation = self.UserMovement.value.get_translate()
    _tracking_translation = self.TrackingTranslation.value.get_translate()

    _total_matrix = avango.gua.make_trans_mat(_user_translation.x + _tracking_translation.x, _tracking_translation.y, _user_translation.z + _tracking_translation.z) * self.OculusRotation.value
    _translation_x = _total_matrix.get_translate().x
    _translation_y = _total_matrix.get_translate().y
    _translation_z = _total_matrix.get_translate().z
  
    # Detect if falling mode has to be activated
    if (_translation_x < -15 or _translation_x > 15 or _translation_z < -15 or _translation_z > 15):
      self.Falling = True    

    # If the player moves over the edges of the plane, he is falling down, else the transformation matrix is set as it is 
    if (self.Falling):
      self.MatrixOut.value = self.FallingMatrix * _total_matrix
      self.FallingMatrix = avango.gua.make_trans_mat(0, self.FallingMatrix.get_translate().y - self.FallingCoefficient, 0)
      self.FallingCoefficient += 0.005
    else:
      self.MatrixOut.value = _total_matrix

    # Prevent from jumping back to the platform when you move back
    if (self.MatrixOut.value.get_translate().y > 0):
      self.Falling = False

class OculusDualViewer(avango.script.Script):
  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline1 = avango.gua.SFPipeline()
  Pipeline2 = avango.gua.SFPipeline()

  WindowSize = avango.gua.SFVec2ui()
  OneEyeWindowSize = avango.gua.SFVec2ui()

  Navigator1 = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, 0))
  Navigator2 = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, 0))
   
  # Field container to save the current rotation matrix of the Oculus Rift
  OculusTransform1 = avango.gua.SFMatrix4()
  OculusTransform2 = avango.gua.SFMatrix4()

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
                            EnableBackfaceCulling = False,
                            AmbientColor = avango.gua.Color(1, 1, 1),
                            SsaoIntensity = 3,
                            SsaoRadius = 5,
                            EnableSsao = True,
                            EnableBloom = True,
                            BloomRadius = 20,
                            BloomIntensity = 2
                          )
    avango.gua.create_texture("data/textures/skymap.jpg")
    self.Pipeline1.value.BackgroundTexture.value = "data/textures/skymap.jpg"

    self.Pipeline2.value = avango.gua.nodes.Pipeline(
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
    self.Pipeline2.value.BackgroundTexture.value = "data/textures/skymap.jpg"

    self.Viewer.Pipelines.value = [self.Pipeline1.value, self.Pipeline2.value]
    self.always_evaluate(True)

  def evaluate(self):
    # Constantly write the Oculus rotation matrix to the field OculusTransform
    self.OculusTransform1.value = self.Pipeline1.value.Window.value.Transform.value
    self.OculusTransform2.value = self.Pipeline2.value.Window.value.Transform.value

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

    # Both helper field containers are initialized
    translation_updater1 = TranslationTransform1()
    translation_updater2 = TranslationTransform2()
    accumulator1 = MatrixAccumulator()
    accumulator2 = MatrixAccumulator()

    accumulator1.OculusRotation.connect_from(self.OculusTransform1)
    accumulator1.TrackingTranslation.connect_from(translation_updater1.MatrixOut)
    accumulator1.UserMovement.connect_from(self.Navigator1.OutTransform)

    accumulator2.OculusRotation.connect_from(self.OculusTransform2)
    accumulator2.TrackingTranslation.connect_from(translation_updater2.MatrixOut)
    accumulator2.UserMovement.connect_from(self.Navigator2.OutTransform)

    # The navigation's transformation then is the output of the accumulator
    self.Player1Position.connect_from(accumulator1.MatrixOut)
    navigation1.Transform.connect_from(self.Player1Position)

    self.Player2Position.connect_from(accumulator2.MatrixOut)
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