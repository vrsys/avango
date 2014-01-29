#!/usr/bin/python

import avango
import avango.script
from avango.script import field_has_changed
import avango.gua

import examples_common.navigator
from examples_common.GuaVE import GuaVE

CUBE_COUNT_X = 30
CUBE_COUNT_Y = 3
CUBE_COUNT_Z = 30

def setup_pipe():
  camera = avango.gua.nodes.Camera(LeftEye = "/eye",
                                   RightEye = "/eye",
                                   LeftScreen = "/eye/screen",
                                   RightScreen = "/eye/screen",
                                   SceneGraph = "scene")

  width = 1920;
  height = int(width * 9.0 / 16.0)
  size = avango.gua.Vec2ui(width, height)

  window = avango.gua.nodes.Window(Size = size,
                                   Title = "weimar",
                                   LeftResolution = size)

  pipe = avango.gua.nodes.Pipeline(Camera = camera,
                                   Window = window,
                                   LeftResolution = size)


  avango.gua.create_texture("data/textures/skymap.jpg")

  pipe.OutputTextureName.value = "weimar_pipe"
  pipe.EnableSsao.value = True
  pipe.SsaoRadius.value = 5
  pipe.SsaoIntensity.value = 0.7


  pipe.EnableFog.value = True
  pipe.FogTexture.value = "data/textures/skymap.jpg"
  pipe.FogStart.value = 20.0
  pipe.FogEnd.value = 50.0

  pipe.BackgroundTexture.value = "data/textures/skymap.jpg"

  pipe.EnableVignette.value = True
  pipe.VignetteColor.value = avango.gua.Color()
  pipe.VignetteCoverage.value = 0.5
  pipe.VignetteSoftness.value = 0.6
  pipe.EnablePreviewDisplay.value = False

  pipe.FarClip.value = 1000
  pipe.NearClip.value = 0.01

  pipe.EnableFXAA.value = True
  pipe.EnableFrustumCulling.value = True

  pipe.EnableFPSDisplay.value = True
  pipe.EnableRayDisplay.value = True

  pipe.EnableBloom.value = False
  pipe.EnableHDR.value = False
  pipe.HDRKey.value = 0.1


  return pipe

def start():

  avango.gua.load_shading_models_from("data/materials")
  avango.gua.load_materials_from("data/materials")

  graph  = avango.gua.nodes.SceneGraph(Name = "scene")
  loader = avango.gua.nodes.GeometryLoader()

  for x in range(0, CUBE_COUNT_X):
    for y in range(0, CUBE_COUNT_Y):
      for z in range(0, CUBE_COUNT_Z):

        new_cube = loader.create_geometry_from_file("cube" + str(x) + str(y) + str(z),
                  "data/objects/monkey.obj",
                  "White",
                  avango.gua.LoaderFlags.DEFAULTS)

        new_cube.Transform.value = avango.gua.make_trans_mat(x*2, y*2, z*2) * \
                                   avango.gua.make_scale_mat(0.3, 0.3, 0.3)
        graph.Root.value.Children.value.append(new_cube)

  sun_light = avango.gua.nodes.SunLightNode(Name = "spot_light",
                                         Color = avango.gua.Color(1.0, 1.0, 0.7),
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 2048,
                                         ShadowOffset = 0.01,
                                         ShadowCascadedSplits = [0.01, 2, 7, 50, 1000],
                                         ShadowNearClippingInSunDirection = 50
                                         )
  sun_light.Transform.value = avango.gua.make_rot_mat(220, 0, 1, 0) * avango.gua.make_rot_mat(-20.0, 1.0, 1.0, 0.0)
  graph.Root.value.Children.value.append(sun_light)

  spot_light = avango.gua.nodes.SpotLightNode(Name = "spot_light",
                                         Color = avango.gua.Color(1.0, 0.0, 0.7),
                                         EnableGodrays = True,
                                         EnableShadows = True,
                                         ShadowMapSize = 256,
                                         ShadowOffset = 0.0005)
  spot_light.Transform.value = avango.gua.make_trans_mat(8.0, 3.0, 8.0) * avango.gua.make_rot_mat(-90, 1, 0, 0) * avango.gua.make_scale_mat(10, 10, 10)
  graph.Root.value.Children.value.append(spot_light)



  top_light = avango.gua.nodes.SunLightNode(Name = "top_light",
                                         Color = avango.gua.Color(0.1, 0.2, 0.4),
                                         EnableSpecularShading = False)
  top_light.Transform.value = avango.gua.make_rot_mat(-90.0, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(top_light)



  fill_light = avango.gua.nodes.SunLightNode(Name = "fill_light",
                                         Color = avango.gua.Color(0.05, 0.1, 0.05),
                                         EnableSpecularShading = False)
  fill_light.Transform.value = avango.gua.make_rot_mat(90.0, 1.0, 0.0, 0.0)
  graph.Root.value.Children.value.append(fill_light)

  floor = loader.create_geometry_from_file("floor",
                  "data/objects/plane.obj",
                  "White",
                  avango.gua.LoaderFlags.DEFAULTS)
  floor.Transform.value = avango.gua.make_scale_mat(30, 1, 30) * avango.gua.make_trans_mat(1, -0.5, 1)
  graph.Root.value.Children.value.append(floor)

  screen = avango.gua.nodes.ScreenNode(Name = "screen",
                                       Width = 4.8,
                                       Height = 2.7)


  screen.Transform.value = avango.gua.make_trans_mat(0.0, 0.0, -2.5)

  eye = avango.gua.nodes.TransformNode(Name = "eye")
  eye.Transform.value = avango.gua.make_trans_mat(0.0, 1.35, 20.0)
  eye.Children.value = [screen]

  graph.Root.value.Children.value.append(eye)

  pipe = setup_pipe()

  navigator = examples_common.navigator.Navigator()
  navigator.StartLocation.value = eye.Transform.value.get_translate()
  navigator.OutTransform.connect_from(eye.Transform)

  navigator.RotationSpeed.value = 0.2
  navigator.MotionSpeed.value = 0.04

  eye.Transform.connect_from(navigator.OutTransform)

  viewer = avango.gua.nodes.Viewer()
  viewer.Pipelines.value = [pipe]
  viewer.SceneGraphs.value = [graph]

  guaVE = GuaVE()
  guaVE.start(locals(), globals())

  viewer.run()


if __name__ == '__main__':
  start()

