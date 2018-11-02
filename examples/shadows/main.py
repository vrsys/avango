import avango
import avango.script
from avango.script import field_has_changed
import avango.gua
from examples_common.GuaVE import GuaVE
import examples_common.navigator
from examples_common.GuaVE import GuaVE

CUBE_COUNT_X = 20
CUBE_COUNT_Y = 2
CUBE_COUNT_Z = 20

def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.TriMeshLoader()

  fallback_mat = avango.gua.create_material(avango.gua.MaterialCapabilities.COLOR_VALUE)


  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file(
          "cube" + str(x) + str(y) + str(z),
          "data/objects/monkey.obj",
          fallback_mat,
          avango.gua.LoaderFlags.DEFAULTS
        )
        # new_cube.Material.value.set_uniform("Color", avango.gua.Vec4(1, 1, 1, 0.6))

        new_cube.Transform.value = avango.gua.make_trans_mat(x*2, y*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)
        new_cube.ShadowMode.value = 1


  spot_light_1 = avango.gua.nodes.LightNode(Name = "spot_light_1",
                                         Type = avango.gua.LightType.SPOT,
                                         Color = avango.gua.Color(1.0, 0.0, 0.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 512,
                                         ShadowOffset = 0.002,
                                         ShadowMaxDistance = 10,
                                         Falloff = 1.5,
                                         Softness = 2,
                                         Brightness = 10)
  spot_light_1.Transform.value = avango.gua.make_trans_mat(14.0, 3.0, 18.0) * avango.gua.make_rot_mat(-20, 1, 0, 0) * avango.gua.make_scale_mat(20)
  graph.Root.value.Children.value.append(spot_light_1)

  point_light1 = avango.gua.nodes.LightNode(
                                         Type = avango.gua.LightType.POINT,
                                         Name = "point_light1",
                                         Color = avango.gua.Color(0.2, 1.0, 1.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 128,
                                         ShadowMaxDistance = 10,
                                         ShadowOffset = 0.03,
                                         Falloff = 0.5,
                                         Brightness = 20)
  point_light1.Transform.value = avango.gua.make_trans_mat(4.5, 1.0, 4.5) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(point_light1)

  sun_light = avango.gua.nodes.LightNode(Name = "sun_light",
                                         Type = avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(1.0, 1.0, 0.7),
                                         EnableShadows = True,
                                         ShadowMapSize = 1024,
                                         ShadowOffset = 0.0005,
                                         ShadowCascadedSplits = [0.1, 4, 7, 20],
                                         ShadowMaxDistance = 30,
                                         ShadowNearClippingInSunDirection = 100,
                                         ShadowFarClippingInSunDirection = 100,
                                         Brightness = 2
                                         )
  sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(sun_light)

  floor = loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  floor.Transform.value = avango.gua.make_scale_mat(20, 1, 20) * avango.gua.make_trans_mat(1, -0.2, 1)
  floor.ShadowMode.value = 0
  graph.Root.value.Children.value.append(floor)

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)

  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

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
  res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.BackgroundMode.value = avango.gua.BackgroundMode.CUBEMAP_TEXTURE
  res_pass.BackgroundTexture.value = "awesome_skymap"
  # res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)
  res_pass.VignetteColor.value = avango.gua.Vec4(0, 0, 0, 1)

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.SkyMapPassDescription(
        OutputTextureName="awesome_skymap"
      ),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.BBoxPassDescription(),
      avango.gua.nodes.TexturedScreenSpaceQuadPassDescription()
      # avango.gua.nodes.DebugViewPassDescription()
    ],
    EnableABuffer = False
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
  # viewer.DesiredFPS.value = 200
  viewer.SceneGraphs.value = [graph]
  viewer.Windows.value = [window]

  timer = avango.nodes.TimeSensor()

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

