#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator
import avango.oculus

# A field container that fetches the translation matrix from the tracking data
class TranslationTransform(avango.script.Script):
  MatrixOut = avango.gua.SFMatrix4()                # Output transformation matrix

  def __init__(self):
    self.super(TranslationTransform).__init__()
    self.always_evaluate(True)                      # has to be set to true in order to update transformation matrix regularly

    # init headtracking sensor at LCD wall
    self.headtracking_sensor = avango.daemon.nodes.DeviceSensor(DeviceService = avango.daemon.DeviceService())
    self.headtracking_sensor.Station.value = "tracking-oculus"
    self.headtracking_sensor.TransmitterOffset.value = avango.gua.make_identity_mat()
    self.headtracking_sensor.ReceiverOffset.value = avango.gua.make_identity_mat()

  def evaluate(self):
    _mat = self.headtracking_sensor.Matrix.value
    _factor = 5

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

class SimpleViewer(avango.script.Script):
  SceneGraph = avango.gua.SFSceneGraph()

  Pipeline = avango.gua.SFPipeline()
  WindowSize = avango.gua.SFVec2ui()
  OneEyeWindowSize = avango.gua.SFVec2ui()

  Navigator = examples_common.navigator.Navigator(StartLocation = avango.gua.Vec3(0, 0, 4))
   
  # Field container to save the current rotation matrix of the Oculus Rift
  OculusTransform = avango.gua.SFMatrix4()

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
    avango.gua.create_texture("data/textures/skymap.jpg")
    self.Pipeline.value.BackgroundTexture.value = "data/textures/skymap.jpg"

    self.Viewer.Pipelines.value = [self.Pipeline.value]
    self.always_evaluate(True)

  def evaluate(self):
    # Constantly write the Oculus rotation matrix to the field OculusTransform
    self.OculusTransform.value = self.Pipeline.value.Window.value.Transform.value

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

    # Both helper field containers are initialized
    translation_updater = TranslationTransform()
    accumulator = MatrixAccumulator()

    accumulator.OculusRotation.connect_from(self.OculusTransform)
    accumulator.TrackingTranslation.connect_from(translation_updater.MatrixOut)
    accumulator.UserMovement.connect_from(self.Navigator.OutTransform)

    navigation.Transform.connect_from(accumulator.MatrixOut)

    # Within the pipeline, we have to set the different eyes and screens
    self.Pipeline.value.Camera.value = avango.gua.nodes.Camera(
                                  LeftEye = "/navigation/eye_left",
                                  RightEye = "/navigation/eye_right",
                                  LeftScreen = "/navigation/screen_left",
                                  RightScreen = "/navigation/screen_right",
                                  SceneGraph = self.SceneGraph.value.Name.value
                                 )

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]