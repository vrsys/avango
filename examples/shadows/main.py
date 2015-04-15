import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

CUBE_COUNT_X = 20
CUBE_COUNT_Y = 2
CUBE_COUNT_Z = 20

def start():

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.TriMeshLoader()

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file(
          "cube" + str(x) + str(y) + str(z),
          "data/objects/monkey.obj",
          avango.gua.LoaderFlags.DEFAULTS
        )
        # new_cube.Material.value.set_uniform("Color", avango.gua.Vec4(1, 1, 1, 0.6))

        new_cube.Transform.value = avango.gua.make_trans_mat(x*2, y*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)


  spot_light_1 = avango.gua.nodes.LightNode(Name = "spot_light_1",
                                         Type=avango.gua.LightType.SPOT,
                                         Color = avango.gua.Color(1.0, 0.0, 0.7),
                                         # DisplayBoundingBox = True,
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 512,
                                         ShadowOffset = 0.002,
                                         Falloff = 1.5,
                                         Softness = 2,
                                         Brightness = 10)
  spot_light_1.Transform.value = avango.gua.make_trans_mat(14.0, 3.0, 18.0) * avango.gua.make_rot_mat(-20, 1, 0, 0) * avango.gua.make_scale_mat(20)
  graph.Root.value.Children.value.append(spot_light_1)

  spot_light_2 = avango.gua.nodes.LightNode(Name = "spot_light_2",
                                         Type=avango.gua.LightType.SPOT,
                                         Color = avango.gua.Color(0.0, 1.0, 0.7),
                                         # DisplayBoundingBox = True,
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 512,
                                         ShadowOffset = 0.002,
                                         Falloff = 1,
                                         Softness = 1,
                                         Brightness = 10)
  spot_light_2.Transform.value = avango.gua.make_trans_mat(14.0, 3.0, 20.0) * avango.gua.make_rot_mat(40, 1, 0, 0) * avango.gua.make_rot_mat(-170, 0, 1, 0) * avango.gua.make_scale_mat(10)
  graph.Root.value.Children.value.append(spot_light_2)

  point_light = avango.gua.nodes.LightNode(
                                         Type=avango.gua.LightType.POINT,
                                         Name = "point_light",
                                         Color = avango.gua.Color(0.2, 1.0, 0.7),
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 128,
                                         ShadowOffset = 0.03,
                                         Falloff = 0.5,
                                         Brightness = 10)
  point_light.Transform.value = avango.gua.make_trans_mat(4.0, 1.0, 4.0) * avango.gua.make_scale_mat(4)
  graph.Root.value.Children.value.append(point_light)

  sun_light = avango.gua.nodes.LightNode(Name = "sun_light",
                                         Type=avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(1.0, 1.0, 0.7),
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 512,
                                         ShadowOffset = 0.002,
                                         ShadowCascadedSplits = [0.1, 5, 20],
                                         ShadowNearClippingInSunDirection = 100,
                                         Brightness = 2
                                         )
  sun_light.Transform.value = avango.gua.make_rot_mat(210, 0, 1, 0) * avango.gua.make_rot_mat(-50.0, 1.0, 1.0, 0.0)
  graph.Root.value.Children.value.append(sun_light)


  top_light = avango.gua.nodes.LightNode(Name = "top_light",
                                         Type=avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(0.1, 0.2, 0.4),
                                         EnableSpecularShading = False,
                                         Brightness = 1)
  top_light.Transform.value = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(top_light)



  fill_light = avango.gua.nodes.LightNode(Name = "fill_light",
                                         Type=avango.gua.LightType.SUN,
                                         Color = avango.gua.Color(0.05, 0.1, 0.05),
                                         EnableSpecularShading = False,
                                         Brightness = 1)
  fill_light.Transform.value = avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(fill_light)


  floor = loader.create_geometry_from_file(
    "floor",
    "data/objects/plane.obj",
    avango.gua.LoaderFlags.DEFAULTS
  )
  floor.Transform.value = avango.gua.make_scale_mat(20, 1, 20) * avango.gua.make_trans_mat(1, -0.2, 1)
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
    SceneGraph = "scene",
    Resolution = size,
    OutputWindowName = "window",
    Children = [screen],
    Transform = avango.gua.make_trans_mat(0.0, 0.0, 7.0)
  )

  res_pass = avango.gua.nodes.ResolvePassDescription()
  res_pass.EnableSSAO.value = False
  res_pass.SSAOIntensity.value = 3.0
  res_pass.SSAOFalloff.value = 20.0
  res_pass.SSAORadius.value = 10.0
  res_pass.EnvironmentLightingColor.value = avango.gua.Color(0.1,0.1,0.1)
  res_pass.ToneMappingMode.value = avango.gua.ToneMappingMode.UNCHARTED
  res_pass.Exposure.value = 1.0
  res_pass.BackgroundColor.value = avango.gua.Color(0.45, 0.5, 0.6)

  pipeline_description = avango.gua.nodes.PipelineDescription(
    Passes = [
      avango.gua.nodes.TriMeshPassDescription(),
      avango.gua.nodes.TexturedQuadPassDescription(),
      avango.gua.nodes.LightVisibilityPassDescription(),
      res_pass,
      avango.gua.nodes.BBoxPassDescription(),
      avango.gua.nodes.DebugViewPassDescription()
    ],
    EnableABuffer = True
  )

  camera.PipelineDescription.value = pipeline_description

  graph.Root.value.Children.value.append(camera)

  window = avango.gua.nodes.Window(
    Size = size,
    Title = "shadows",
    LeftResolution = size
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

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

