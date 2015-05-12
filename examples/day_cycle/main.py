import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
import avango.gua.gui
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

from astral.astral import Astral
from datetime import datetime
from datetime import timedelta

CUBE_COUNT_X = 20
CUBE_COUNT_Y = 2
CUBE_COUNT_Z = 20

a = Astral()

def get_azimuth(city, time):
  return a.solar_azimuth(time, city.latitude, city.longitude)

def get_elevation(city, time):
  return a.solar_elevation(time, city.latitude, city.longitude)

class FPSUpdater(avango.script.Script):
  TimeIn = avango.SFFloat()
  FPSResource = avango.gua.gui.SFGuiResource()
  Window = avango.gua.SFWindowBase()
  Viewer = avango.gua.SFViewer()

  @field_has_changed(TimeIn)
  def update_fps(self):
    application_string = "{:5.2f}".format(self.Viewer.value.ApplicationFPS.value)
    rendering_string = "{:5.2f}".format(self.Window.value.RenderingFPS.value)
    fps_string = "FPS: " + application_string + " " + rendering_string
    self.FPSResource.value.call_javascript("set_fps_text", [fps_string])

class SunUpdater(avango.script.Script):

  TimeIn = avango.SFFloat()
  TimeScale = avango.SFFloat()

  MatrixOut = avango.gua.SFMatrix4()
  DirectionOut = avango.gua.SFVec3()
  SunColorOut = avango.gua.SFColor()
  GroundColorOut = avango.gua.SFColor()
  BlendFactorOut = avango.SFFloat()

  def __init__(self):
    self.super(SunUpdater).__init__()
    self.city = a["Berlin"]

  @field_has_changed(TimeIn)
  def update(self):
    date = datetime.utcnow() + timedelta(0,self.TimeIn.value*self.TimeScale.value)
    azimuth = get_azimuth(self.city, date)
    elevation = get_elevation(self.city, date)

    r = g = b = 0.0;

    if (elevation > 0):
      r = g = b = elevation/90
      r = pow(r, 0.3) * 1.3
      g = pow(g, 0.4) * 1.2
      b = pow(b, 0.5) * 1.6

    self.SunColorOut.value = avango.gua.Color(r, g, b)
    self.MatrixOut.value = avango.gua.make_rot_mat(azimuth, 0, 1, 0) * avango.gua.make_rot_mat(-elevation, 1, 0, 0)
    direcion = self.MatrixOut.value * avango.gua.Vec3(0, 0, -1)
    self.DirectionOut.value = avango.gua.Vec3(direcion.x, direcion.y ,direcion.z)

    self.BlendFactorOut.value = 0.0

    if elevation/90 < 0:
      self.BlendFactorOut.value = abs(elevation/90) * 5

    if direcion.y > 0:
      val = max(0.2 - direcion.y, 0)
      self.GroundColorOut.value = avango.gua.Color(val, val, val)
    else:
      self.GroundColorOut.value = avango.gua.Color(0.5,0.5,0.5)

def start():
  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.TriMeshLoader()

  fps_size = avango.gua.Vec2(170, 55)

  fps = avango.gua.gui.nodes.GuiResource()
  fps.init("fps", "asset://gua/data/html/fps.html", fps_size)

  fps_quad = avango.gua.nodes.TexturedScreenSpaceQuadNode(
    Name = "fps_quad",
    Texture = "fps",
    Width = int(fps_size.x),
    Height = int(fps_size.y),
    Anchor = avango.gua.Vec2(1.0, 1.0)
  )
  graph.Root.value.Children.value.append(fps_quad)

  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE | avango.gua.MaterialCapabilities.ROUGHNESS_VALUE)
  fallback_mat.set_uniform("Roughness", 0.6)

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file(
          "cube" + str(x) + str(y) + str(z),
          "data/objects/monkey.obj",
          fallback_mat,
          avango.gua.LoaderFlags.DEFAULTS
        )

        new_cube.Transform.value = avango.gua.make_trans_mat(x*2, y*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)
        new_cube.ShadowMode.value = 1

  sun_light = avango.gua.nodes.LightNode(
    Name = "sun_light",
    Type = avango.gua.LightType.SUN,
    Color = avango.gua.Color(1.0, 1.0, 0.7),
    EnableShadows = True,
    ShadowMapSize = 1024,
    ShadowOffset = 0.0005,
    ShadowCascadedSplits = [0.1, 4, 7, 20],
    ShadowMaxDistance = 30,
    ShadowNearClippingInSunDirection = 100,
    ShadowFarClippingInSunDirection = 100,
    Brightness = 4
  )
  graph.Root.value.Children.value.append(sun_light)

  floor = loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    fallback_mat,
    avango.gua.LoaderFlags.DEFAULTS
  )
  floor.Transform.value = avango.gua.make_scale_mat(200, 1, 200) * avango.gua.make_trans_mat(-0.5, -0.2, -0.5)
  floor.ShadowMode.value = 0
  graph.Root.value.Children.value.append(floor)

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  screen = avango.gua.nodes.ScreenNode(
    Name = "screen",
    Width = 4.8,
    Height = 2.7,
    Transform = avango.gua.make_trans_mat(0.0, 0.0, -2.5)
  )

  camera = avango.gua.nodes.CameraNode(
    Name = "cam",
    LeftScreenPath = "/cam/screen",
    RightScreenPath = "/cam/screen",
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    EyeDistance = 0.2,
    EnableStereo = False,
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.EnableSSAO.value = True
  res_pass.SSAOIntensity.value = 3.0
  res_pass.SSAOFalloff.value = 20.0
  res_pass.SSAORadius.value = 10.0
  res_pass.EnvironmentLightingTexture.value = "day_skymap"
  res_pass.AlternativeEnvironmentLightingTexture.value = "night_skymap"
  res_pass.EnvironmentLightingMode.value = avango.gua.EnvironmentLightingMode.CUBEMAP
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 0.7)
  res_pass.VignetteCoverage.value = 0.0
  res_pass.EnableFog.value = True
  res_pass.FogStart.value = 30
  res_pass.FogEnd.value = 100
  res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
  res_pass.BackgroundTexture.value = "day_skymap"
  res_pass.AlternativeBackgroundTexture.value = "night_skymap"

  sky_pass = avango.gua.nodes.SkyMapPassDescription(
    OutputTextureName="day_skymap"
  )

  avango.gua.create_texture_cube(
    "night_skymap",
    "data/textures/stars/purple-nebula/purple-nebula_right1.jpg",
    "data/textures/stars/purple-nebula/purple-nebula_left2.jpg",
    "data/textures/stars/purple-nebula/purple-nebula_bottom4.jpg",
    "data/textures/stars/purple-nebula/purple-nebula_top3.jpg",
    "data/textures/stars/purple-nebula/purple-nebula_front5.jpg",
    "data/textures/stars/purple-nebula/purple-nebula_back6.jpg"
  )

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      sky_pass,
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
    ]
  )

  camera.PipelineDescription.value = pipeline_description

  graph.Root.value.Children.value.append(camera)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "shadows",
    LeftResolution = size,
    RightResolution = size,
    EnableVsync = False,
    StereoMode = avango.gua.StereoMode.MONO
  )

  avango.gua.register_window("window", window)

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = camera.Transform.value.get_translate()
  navigator.OutTransform.connect_from(camera.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  camera.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  fps_updater = FPSUpdater(
    FPSResource = fps,
    Window = window,
    Viewer = viewer
  )
  fps_updater.TimeIn.connect_from(timer.Time)

  sun_updater = SunUpdater(
    TimeScale = 3500
  )
  sun_updater.TimeIn.connect_from(timer.Time)
  sun_light.Transform.connect_from(sun_updater.MatrixOut)
  sun_light.Color.connect_from(sun_updater.SunColorOut)
  sky_pass.LightDirection.connect_from(sun_updater.DirectionOut)
  sky_pass.GroundColor.connect_from(sun_updater.GroundColorOut)
  res_pass.BackgroundTextureBlendFactor.connect_from(sun_updater.BlendFactorOut)
  res_pass.EnvironmentLightingTextureBlendFactor.connect_from(sun_updater.BlendFactorOut)

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

