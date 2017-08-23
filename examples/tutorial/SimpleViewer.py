import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

from GuaVE import GuaVE
from Navigator import Navigator

class SimpleViewer(avango.script.Script):

  SceneGraph = avango.gua.SFSceneGraph()
  Navigator = Navigator()
  Viewer = avango.gua.nodes.Viewer()
  Shell = GuaVE()

  def __init__(self):
    self.super(SimpleViewer).__init__()
    self.window_size = avango.gua.Vec2ui(1920, 1080)

    # create resolve pass
    self.res_pass = avango.gua.nodes.ResolvePassDescription()
    self.res_pass.EnableSSAO.value = True
    self.res_pass.SSAOIntensity.value = 4.0
    self.res_pass.SSAOFalloff.value = 10.0
    self.res_pass.SSAORadius.value = 7.0
    self.res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1, 0.1, 0.1)
    self.res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
    self.res_pass.Exposure.value = 1.0
    self.res_pass.BackgroundColor.value = avango.gua.Color(0.0, 0.0, 0.0)
    self.res_pass.BackgroundMode.value = avango.gua.BackgroundMode.COLOR

    # create anti-aliasing pass
    self.anti_aliasing = avango.gua.nodes.SSAAPassDescription()

    # create pipeline description
    self.pipeline_description = avango.gua.nodes.PipelineDescription(Passes=[
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      self.res_pass,
      self.anti_aliasing])

    # create window
    self.window = avango.gua.nodes.GlfwWindow(Size = self.window_size,
                                              LeftResolution = self.window_size)
    avango.gua.register_window("window", self.window)


  def run(self, locals, globals, show_guave_banner = True):
    self.Shell.start(locals, globals, show_guave_banner)
    self.Viewer.run()

  def list_variabels(self):
    self.Shell.list_variables()

  def start_navigation(self):
    self.navigation.Transform.connect_from(self.Navigator.OutTransform)

  def set_background_image(self, path):
    self.res_pass.BackgroundMode.value = avango.gua.BackgroundMode.SKYMAP_TEXTURE
    self.res_pass.BackgroundTexture.value = path

  @field_has_changed(SceneGraph)
  def update_scenegraph(self):

    self.navigation = avango.gua.nodes.TransformNode(Name = "navigation")

    cam = avango.gua.nodes.CameraNode(Name = "cam",
                                      LeftScreenPath = "/navigation/screen",
                                      SceneGraph = self.SceneGraph.value.Name.value,
                                      Resolution = self.window_size,
                                      OutputWindowName = "window",
                                      Transform = avango.gua.make_trans_mat(0.0, 0.0, 1.0))

    cam.PipelineDescription.value = self.pipeline_description

    screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                         Width = 1.6,
                                         Height = 1.0)

    self.navigation.Children.value = [cam, screen]

    self.SceneGraph.value.Root.value.Children.value.append(self.navigation)

    self.Navigator.OutTransform.connect_from(cam.Transform)
    self.Navigator.RotationSpeed.value = 0.2
    self.Navigator.MotionSpeed.value = 0.04

    self.Viewer.SceneGraphs.value = [self.SceneGraph.value]
    self.Viewer.Windows.value = [self.window]

